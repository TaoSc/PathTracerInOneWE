#pragma once

#include <memory>

#include "utilities/vec3.h"

class texture {
public:
    virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

class const_texture : public texture {
public:
    const_texture() {}
    const_texture(const vec3& c) : color(c) {}

    virtual vec3 value(double u, double v, const vec3& p) const override {
        return color;
    }

    vec3 color;
};

class checker_texture : public texture {
public:
    checker_texture() : frequency(50) {}
    checker_texture(std::shared_ptr<texture> tex_0, std::shared_ptr<texture> tex_1, double freq = 50) : even(tex_0), odd(tex_1), frequency(freq) {}

    virtual vec3 value(double u, double v, const vec3& p) const override {
        double sines = sin(frequency * u) * sin(frequency * v);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
    double frequency;
};
