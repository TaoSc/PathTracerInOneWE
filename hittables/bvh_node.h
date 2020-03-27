#pragma once

#include <memory>
#include <vector>

#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"
#include "hittables/hittable_list.h"

class bvh_node : public hittable {
public:
    bvh_node() {};
    bvh_node(hittable_list& list, double time0, double time1) : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}
    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1);

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override;

    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;
};

inline bool bvh_node::bounding_box(const double& time0, const double& time1, aabb& output_box) const {
    output_box = box;
    return true;
}
