#pragma once

#include "vec3.h"

class ray {
public:
    ray() : tm(0) {}
    ray(const vec3& origin, const vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const { return tm; }

    vec3 at(double position) const { return orig + position * dir; }

    vec3 orig;
    vec3 dir;
    double tm;
};
