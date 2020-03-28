#pragma once

#include <limits>

constexpr double PI = 3.14159265358979323846;
constexpr double infinity = std::numeric_limits<double>::infinity();

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180;
}
