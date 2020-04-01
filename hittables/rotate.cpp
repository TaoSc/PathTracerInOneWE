#include "hittables/rotate.h"

#include "utilities/vec3.h"
#include "utilities/ray.h"

rotate_y::rotate_y(std::shared_ptr<hittable> p, double angle) : ptr(p) {
    double radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    has_box = ptr->bounding_box(0, 1, bbox);

    vec3 min(infinity, infinity, infinity);
    vec3 max(-infinity, -infinity, -infinity);

    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < 2; j++) {
            for (unsigned int k = 0; k < 2; k++) {
#pragma warning(disable : 26451)
                double x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                double y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                double z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                double newx = cos_theta * x + sin_theta * z;
                double newz = -sin_theta * x + cos_theta * z;

                vec3 tester(newx, y, newz);

                for (size_t c = 0; c < 3; c++) {
                    min[c] = std::min(min[c], tester[c]);
                    max[c] = std::max(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    vec3 origin = r.origin();
    vec3 direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    vec3 p = rec.point;
    vec3 normal = rec.normal;

    p[0] = cos_theta * rec.point[0] + sin_theta * rec.point[2];
    p[2] = -sin_theta * rec.point[0] + cos_theta * rec.point[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.point = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}
