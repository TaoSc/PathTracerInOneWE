#pragma once

#include <memory>

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record {
    float time = 0.f;
    vec3 point;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    bool front_face = false;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const = 0;
};
