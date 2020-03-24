#include "hittable_list.h"

bool hittable_list::hit(const ray& r, const float& t_min, const float& t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.time;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
