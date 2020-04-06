#pragma once

#include "hittables/hittable.h"
#include "hittables/bvh_node.h"

class scene {
public:
    scene(const vec3& cam_pos, const vec3& look, double field_of_view, double a, double dist, const vec3& bg)
        : m_background(bg), m_cam_pos(cam_pos), m_lookat(look), m_fov(field_of_view), m_aperture(a), m_focus_dist(dist) {}

    virtual bvh_node descr() const = 0;
    vec3 background() const { return m_background; }
    vec3 cam() const { return m_cam_pos; }
    vec3 lookat() const { return m_lookat; }
    double fov() const { return m_fov; }
    double aperture() const { return m_aperture; }
    double focus_dist() const { return m_focus_dist; }

private:
    vec3 m_background;
    vec3 m_cam_pos;
    vec3 m_lookat;
    double m_fov;
    double m_aperture;
    double m_focus_dist;
};

class random_scene : public scene {
public:
    random_scene() : scene(vec3(13, 2, 3), vec3(0, 0, 0), 20, 0.1, 10, vec3(0.47, 0.29, 0.06)) {}

    virtual bvh_node descr() const override;
};

class cornell_box : public scene {
public:
    cornell_box() : scene(vec3(278, 278, -800), vec3(278, 278, 0), 40, 0, 10, vec3(0)) {}

    virtual bvh_node descr() const override;
};

class basic_scene : public scene {
public:
    basic_scene() : scene(vec3(0, 0, 0), vec3(0, 0, -1), 90, 0, 1, vec3(0.5, 0.7, 1.0)) {}

    virtual bvh_node descr() const override;
};

class book2_scene : public scene {
public:
    book2_scene() : scene(vec3(478, 278, -600), vec3(278, 278, 0), 40, 0, 10, vec3(0)) {}

    virtual bvh_node descr() const override;
};

class perlin_scene : public scene {
public:
    perlin_scene() : scene(vec3(13, 2, 3), vec3(0), 20, 0, 10, vec3(0.5, 0.7, 1.0)) {}

    virtual bvh_node descr() const override;
};
