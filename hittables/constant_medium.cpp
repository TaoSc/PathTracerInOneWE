#include "constant_medium.h"

bool constant_medium::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.time + 0.0001, infinity, rec2))
        return false;

    if (rec1.time < t_min) rec1.time = t_min;
    if (rec2.time > t_max) rec2.time = t_max;

    if (rec1.time >= rec2.time)
        return false;

    if (rec1.time < 0)
        rec1.time = 0;

    const double ray_length = r.direction().length();
    const double distance_inside_boundary = (rec2.time - rec1.time) * ray_length;
    const double hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.time = rec1.time + hit_distance / ray_length;
    rec.point = r.at(rec.time);

    rec.normal = vec3(1, 0, 0); // arbitrary as it won't be used for shading
    rec.front_face = true;      // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}
