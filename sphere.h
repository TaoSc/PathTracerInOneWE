#pragma once

#include <memory>

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "material.h"
#include "aabb.h"

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
