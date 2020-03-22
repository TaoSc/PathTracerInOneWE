#pragma once

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"

class sphere : public hittable {
public:
    sphere() : center(vec3()), radius(1), mat_ptr(new lambertian(vec3(1.f, 1.f, 1.f))) {}
    sphere(const vec3& cen, const float& r, material* mat) : center(cen), radius(r), mat_ptr(mat) {}
    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const;

    vec3 center;
    float radius;
    material* mat_ptr;
};
