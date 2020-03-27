#pragma once

#include <memory>

#include "utilities/vec3.h"
#include "utilities/ray.h"
#include "utilities/utilities.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"
#include "materials/material.h"

class sphere : public hittable {
public:
    sphere() : radius(1) {}
    sphere(const vec3& cen, double r, std::shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override;
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override;

    vec3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

inline void get_sphere_uv(const vec3& p, double& u, double& v) {
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    u = 1 - (phi + PI) / (2 * PI);
    v = (theta + PI / 2) / PI;
}
