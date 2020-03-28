#pragma once

#include "hittables/hittable.h"
#include "hittables/hittable_list.h"
#include "hittables/aa_rect.h"
#include "hittables/flip_face.h"

using std::make_shared;

class box : public hittable {
public:
    box() {}

    box(const vec3& p0, const vec3& p1, std::shared_ptr<material> mp) : box_min(p0), box_max(p1), material(mp) {
        sides.add(                       make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material)); // back
        sides.add(make_shared<flip_face>(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material))); // front

        sides.add(                       make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material)); // top
        sides.add(make_shared<flip_face>(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material))); // bottom

        // X-axis is RTL
        sides.add(                       make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material)); // left
        sides.add(make_shared<flip_face>(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material))); // right
    }

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override {
        return sides.hit(r, t_min, t_max, rec);
    }

    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

    vec3 box_min;
    vec3 box_max;
    hittable_list sides;
    std::shared_ptr<material> material;
};