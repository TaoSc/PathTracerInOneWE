#pragma once

#include <memory>

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"

class sphere : public hittable {
public:
    sphere() : center(vec3()), radius(1) {}
    sphere(const vec3& cen, double r, std::shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override;

    vec3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};
