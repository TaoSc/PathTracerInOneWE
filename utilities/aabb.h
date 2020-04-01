#pragma once

#include <algorithm>

#include "utilities/vec3.h"
#include "utilities/ray.h"

class aabb {
public:
    aabb() {}
    aabb(const vec3& a, const vec3& b) : _min(a), _max(b) {}

    vec3 min() const { return _min; }
    vec3 max() const { return _max; }

    bool hit(const ray& r, double tmin, double tmax) const {
        for (int a = 0; a < 3; a++) {
            double inv_dir = 1.0 / r.direction()[a];
            double t0 = (min()[a] - r.origin()[a]) * inv_dir;
            double t1 = (max()[a] - r.origin()[a]) * inv_dir;
            if (inv_dir < 0.0)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }
        return true;
    }

private:
    vec3 _min;
    vec3 _max;
};

inline aabb surrounding_box(const aabb& box0, const aabb& box1) {
    vec3 bottom(std::min(box0.min().x(), box1.min().x()),
                std::min(box0.min().y(), box1.min().y()),
                std::min(box0.min().z(), box1.min().z()));
    vec3 top(std::max(box0.max().x(), box1.max().x()),
             std::max(box0.max().y(), box1.max().y()),
             std::max(box0.max().z(), box1.max().z()));
    return aabb(bottom, top);
}
