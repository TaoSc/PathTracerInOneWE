#pragma once

#include <memory>
#include "utilities/vec3.h"
#include "utilities/ray.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"

class translate : public hittable {
public:
    translate(std::shared_ptr<hittable> p, const vec3& displacement) : hittable_ptr(p), offset(displacement) {}

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override {
        ray offset_ray(r.origin() - offset, r.direction(), r.time());
        if (!hittable_ptr->hit(offset_ray, t_min, t_max, rec))
            return false;

        rec.point += offset;
        rec.set_face_normal(offset_ray, rec.normal);
        return true;
    }
    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        if (!hittable_ptr->bounding_box(time0, time1, output_box))
            return false;
        
        output_box = aabb(output_box.min() + offset, output_box.max() + offset);
        return true;
    }

private:
    std::shared_ptr<hittable> hittable_ptr;
    vec3 offset;
};
