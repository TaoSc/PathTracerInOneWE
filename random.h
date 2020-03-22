#pragma once

#include <cstdlib>

#include "vec3.h"

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline vec3 random_in_unit_sphere() {
    vec3 point;
    do {
        point = 2.0f * vec3(float(random_double()), float(random_double()), float(random_double())) - vec3(1.f, 1.f, 1.f);
    } while (point.squared_length() >= 1.0f);
    return point;
}
