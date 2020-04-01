#pragma once

#include <memory>

#include "hittables/hittable.h"

class xy_rect : public hittable {
public:
    xy_rect() : x0(0), x1(0), y0(0), y1(0), z(0) {}

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _z, std::shared_ptr<material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), z(_z), mp(mat) {};

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;

    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        output_box = aabb(vec3(x0, y0, z - 0.0001), vec3(x1, y1, z + 0.0001));
        return true;
    }

    double x0, x1, y0, y1, z;
    std::shared_ptr<material> mp;
};

class xz_rect : public hittable {
public:
    xz_rect() : x0(0), x1(0), z0(0), z1(0), y(0) {}

    xz_rect(double _x0, double _x1, double _z0, double _z1, double _y, std::shared_ptr<material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), y(_y), mp(mat) {};

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;

    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        output_box = aabb(vec3(x0, y - 0.0001, z0), vec3(x1, y + 0.0001, z1));
        return true;
    }

    double x0, x1, z0, z1, y;
    std::shared_ptr<material> mp;
};

class yz_rect : public hittable {
public:
    yz_rect() : y0(0), y1(0), z0(0), z1(0), x(0) {}

    yz_rect(double _y0, double _y1, double _z0, double _z1, double _z, std::shared_ptr<material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), x(_z), mp(mat) {};

    virtual bool hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const override;

    virtual bool bounding_box(const double& time0, const double& time1, aabb& output_box) const override {
        output_box = aabb(vec3(x - 0.0001, y0, z0), vec3(x + 0.0001, y1, z1));
        return true;
    }

    double y0, y1, z0, z1, x;
    std::shared_ptr<material> mp;
};
