#pragma once

#include <algorithm>
#include <memory>

#include "utilities/ray.h"
#include "utilities/random.h"
#include "utilities/vec3.h"
#include "hittables/hittable.h"
#include "textures/texture.h"

vec3 reflect(const vec3& incident, const vec3& normal);
vec3 refract(const vec3& incident, const vec3& normal, double etai_over_etat);
double schlick(double cosine, double ref_idx);

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
    virtual vec3 emitted(double u, double v, const vec3& p) const { return vec3(0, 0, 0); }
};

class lambertian : public material {
public:
    lambertian(std::shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.point, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.point);
        return true;
    }

private:
    std::shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(std::shared_ptr<texture> a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.point, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.point);
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    std::shared_ptr<texture> albedo;
    double fuzz;
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> a) : emit(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered ) const override { return false; }

    virtual vec3 emitted(double u, double v, const vec3& p) const override {
        return emit->value(u, v, p);
    }

private:
    std::shared_ptr<texture> emit;
};
