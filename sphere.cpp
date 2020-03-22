#include "sphere.h"

bool sphere::hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;

    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction()); // *2 but it cancels out later
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c; // -4*ac (cancels out *2)

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.point = r.point_at_parameter(rec.t);
            rec.normal = (rec.point - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.point = r.point_at_parameter(rec.t);
            rec.normal = (rec.point - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}
