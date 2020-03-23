#pragma once

#include "ray.h"
#include "hittable.h"
#include "random.h"

vec3 reflect(const vec3& v, const vec3& n);

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};


class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.point + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.point, target - rec.point);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};


class metal : public material {
public:
    metal(const vec3& a, const float& f) : albedo(a) {
        fuzz = (f > 1 || f < 0) ? 1 : f;
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.point, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};
