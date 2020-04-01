#pragma once

#include <vector>
#include <memory>

#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"

using std::shared_ptr;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override;

    std::vector<shared_ptr<hittable>> objects;
};
