#pragma once

#include <memory>

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"

class moving_sphere : public hittable {
public:
    moving_sphere() : radius(1), time0(0), time1(0) {}
    moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, std::shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override;
    vec3 center(const double& time) const;

    vec3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<material> mat_ptr;
};
