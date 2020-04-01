#pragma once

#include <memory>

#include "hittables/hittable.h"

class flip_face : public hittable {
public:
    flip_face(std::shared_ptr<hittable> p) : hittable_ptr(p) {}

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override {
        if (!hittable_ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        return hittable_ptr->bounding_box(time0, time1, output_box);
    }

    std::shared_ptr<hittable> hittable_ptr;
};