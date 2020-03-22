#pragma once

#include "ray.h"

class camera {
public:
    camera(const float& aspect_ratio) {
        lower_left_corner = vec3(-1.0f * aspect_ratio, -1.0, -1.0);
        horizontal = vec3(2.0f * aspect_ratio, 0.0, 0.0);
        vertical = vec3(0.0, 2.0, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }
    ray get_ray(const float& u, const float& v) {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
