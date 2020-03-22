#pragma once

#include "ray.h"

class material;

struct hit_record {
    float t = 0.f;
    vec3 point;
    vec3 normal;
    material* mat_ptr;
};

class hittable {
public:
    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const = 0;
};
