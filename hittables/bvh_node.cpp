#include "bvh_node.h"

#include <memory>
#include <algorithm>
#include <iostream>

#include "utilities/aabb.h"
#include "utilities/random.h"
#include "utilities/ray.h"
#include "hittables/hittable.h"

using std::shared_ptr;

bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor." << std::endl;;

    return box_a.min().e[axis] < box_b.min().e[axis];
}


bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1) {
    int axis = random_int(0, 2);
    bool (*comparator)(shared_ptr<hittable>, shared_ptr<hittable>);
    comparator = (axis == 0) ? box_x_compare
               : (axis == 1) ? box_y_compare
                             : box_z_compare;

    // This algorithm is akin to a binary search algorithm
    size_t object_span = end - start;
    if (object_span == 1) {
        left = right = objects[start];
    }
    else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        size_t mid = start + object_span / 2;
        left = std::make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = std::make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in bvh_node constructor." << std::endl;

    box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.time : t_max, rec); // rec.time will be populated by the left node hit test

    return hit_left || hit_right;
}
