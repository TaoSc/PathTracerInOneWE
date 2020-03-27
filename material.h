#pragma once

#include <algorithm>
#include <memory>

#include "ray.h"
#include "hittable.h"
#include "random.h"
#include "vec3.h"
#include "texture.h"

vec3 reflect(const vec3& incident, const vec3& normal);
vec3 refract(const vec3& incident, const vec3& normal, double etai_over_etat);
double schlick(double cosine, double ref_idx);

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
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

    std::shared_ptr<texture> albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override {
        attenuation = vec3(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.point, reflected, r_in.time());
            return true;
        }
        double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.point, reflected, r_in.time());
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.point, refracted, r_in.time());
        return true;
    }

    double ref_idx;
};
