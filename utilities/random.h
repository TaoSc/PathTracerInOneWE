#pragma once

#include <cstdlib>

#include "utilities/utilities.h"
#include "utilities/vec3.h"

inline int random_int() {
    return rand();
}

inline int random_int(int min, int max) {
    return (random_int() % (max + 1 - min)) + min;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline vec3 random_in_unit_sphere() {
    vec3 point;
    do {
        point = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1., 1., 1.);
    } while (point.length_squared() >= 1.0);
    return point;
}

inline vec3 random_in_unit_disk() {
    vec3 point;
    do {
        point = 2.0 * vec3(random_double(), random_double(), 0.) - vec3(1., 1., 0.);
    } while (point.length_squared() >= 1.0);
    return point;
}

inline vec3 random_unit_vector() {
    double a = random_double(0, 2. * PI);
    double z = random_double(-1, 1);
    double r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}
