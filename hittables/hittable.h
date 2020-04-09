#pragma once

#include <memory>

#include "utilities/vec3.h"
#include "utilities/ray.h"
#include "utilities/aabb.h"

class material;

struct hit_record {
    double time = 0.;
    double u = 0., v = 0.;
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
    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const = 0;
};
