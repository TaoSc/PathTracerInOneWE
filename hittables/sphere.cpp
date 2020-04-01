#include "sphere.h"

#include "utilities/ray.h"
#include "utilities/vec3.h"
#include "utilities/aabb.h"
#include "hittables/hittable.h"

bool sphere::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction()); // half because the 2 will cancel out later
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c; // -4*ac (cancels out *2)

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.time = temp;
            rec.point = r.at(rec.time);
            vec3 outward_normal = (rec.point - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(outward_normal, rec.u, rec.v);
            return true;
        }
        temp = (-half_b + root) / a; // *2 cancelled out
        if (temp < t_max && temp > t_min) {
            rec.time = temp;
            rec.point = r.at(rec.time);
            vec3 outward_normal = (rec.point - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(outward_normal, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(const double& time0, const double& time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}
