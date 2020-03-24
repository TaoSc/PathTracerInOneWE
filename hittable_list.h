#pragma once

#include <vector>
#include <memory>

#include "ray.h"
#include "hittable.h"

using std::shared_ptr;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const override;

    std::vector<shared_ptr<hittable>> objects;
};
