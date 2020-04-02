#pragma once

#include <memory>

#include "textures/texture.h"
#include "materials/material.h"
#include "hittables/hittable.h"

using std::shared_ptr;
using std::make_shared;

class constant_medium : public hittable {
public:
    constant_medium(shared_ptr<hittable> b, double density, shared_ptr<texture> a) : boundary(b), phase_function(make_shared<isotropic>(a)), neg_inv_density(-1 / density) {}

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;

    virtual bool bounding_box(const double& t0, const double& t1, aabb& output_box) const override {
        return boundary->bounding_box(t0, t1, output_box);
    }

private:
    shared_ptr<hittable> boundary;
    shared_ptr<material> phase_function;
    double neg_inv_density;
};
