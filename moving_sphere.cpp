#include "moving_sphere.h"

bool moving_sphere::hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction()); // half because the 2 will cancel out later
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c; // -4*ac (cancels out *2)

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.time = static_cast<float>(temp);
            rec.point = r.at(rec.time);
            vec3 outward_normal = (rec.point - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b + root) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.time = static_cast<float>(temp);
            rec.point = r.at(rec.time);
            vec3 outward_normal = (rec.point - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 moving_sphere::center(const double& time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}
