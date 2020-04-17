#include "hittable_list.h"

#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"

bool hittable_list::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.time;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(const double& time0, const double& time1, aabb& output_box) const {
    if (objects.empty()) return false;

    // We retrieve the first object's bounding box OOTL because we need to pass two BB to the surrounding_box func
    aabb temp_box;
    if (!objects[0]->bounding_box(time0, time1, temp_box))
        return false;

    output_box = temp_box;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box))
            return false;
        output_box = surrounding_box(output_box, temp_box);
    }

    return true;
}
