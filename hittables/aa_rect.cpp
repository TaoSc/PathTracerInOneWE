#include "aa_rect.h"

void set_rect_hit_record(hit_record& rec, const ray& r, const double& t, const std::shared_ptr<material>& mp, const vec3& out_norm) {
    rec.time = t;
    rec.set_face_normal(r, out_norm);
    rec.mat_ptr = mp;
    rec.point = r.at(t);
}

bool xy_rect::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    double t = (z - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    double x = r.origin().x() + t * r.direction().x();
    double y = r.origin().y() + t * r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    set_rect_hit_record(rec, r, t, mp, vec3(0, 0, 1));

    return true;
}

bool xz_rect::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    double t = (y - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    double x = r.origin().x() + t * r.direction().x();
    double z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    set_rect_hit_record(rec, r, t, mp, vec3(0, 1, 0));

    return true;
}

bool yz_rect::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const {
    double t = (x - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    double z = r.origin().z() + t * r.direction().z();
    double y = r.origin().y() + t * r.direction().y();
    if (z < z0 || z > z1 || y < y0 || y > y1)
        return false;

    rec.u = (z - z0) / (z1 - z0);
    rec.v = (y - y0) / (y1 - y0);
    set_rect_hit_record(rec, r, t, mp, vec3(1, 0, 0));

    return true;
}
