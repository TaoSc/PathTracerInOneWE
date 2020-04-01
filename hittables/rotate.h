#pragma once

#include <memory>
#include <algorithm>

#include "utilities/utilities.h"
#include "utilities/vec3.h"
#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"

class rotate_y : public hittable {
public:
    rotate_y(std::shared_ptr<hittable> p, double angle);

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        output_box = bbox;
        return has_box;
    }

    std::shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool has_box;
    aabb bbox;
};
