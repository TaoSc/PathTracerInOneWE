#pragma once

constexpr float M_PI = 3.1415f;

#include "ray.h"
#include "random.h"

class camera {
public:
    camera(const vec3& lookfrom, const vec3& lookat, const vec3& up_vector, const float& vertical_fov, const float& aspect_ratio, const float& aperture, const float& focus_dist) {
        lens_radius = aperture / 2;
        float theta = vertical_fov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up_vector, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(const float& s, const float& t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};
