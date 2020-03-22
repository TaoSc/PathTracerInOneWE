#pragma once

#include "ray.h"
#include "hittable.h"

class hittable_list : public hittable {
public:
    hittable_list() : list(nullptr), list_size(0) {}
    hittable_list(hittable** l, int n) : list(l), list_size(n) {};
    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const;

    hittable** list;
    int list_size;
};
