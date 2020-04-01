#pragma once

#include <memory>

#include "utilities/vec3.h"
#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"
#include "materials/material.h"

class moving_sphere : public hittable {
public:
    moving_sphere() : time0(0), time1(0), radius(1) {}
    moving_sphere(const vec3& cen0, const vec3& cen1, double t0, double t1, double r, std::shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override;
    vec3 center(const double& time) const;

    vec3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<material> mat_ptr;
};
