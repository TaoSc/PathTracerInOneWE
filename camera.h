#pragma once

constexpr float M_PI = 3.1415f;

#include "ray.h"

class camera {
public:
    camera(const vec3& lookfrom, const vec3& lookat, const vec3& up_vector, const float& vertical_fov, const float& aspect_ratio) {
        vec3 u, v, w;

        float theta = vertical_fov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up_vector, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }

    ray get_ray(const float& u, const float& v) {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
