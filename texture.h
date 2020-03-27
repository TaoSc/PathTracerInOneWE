#pragma once

#include <memory>

#include "vec3.h"

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

class image_texture : public texture {
public:
    image_texture() : data(nullptr), width(0), height(0) {}
    image_texture(unsigned char* pixels, int w, int h) : data(pixels), width(w), height(h) {}

    ~image_texture() { delete data; }

    virtual vec3 value(double u, double v, const vec3& p) const override {
        if (data == nullptr) // If no data return bright red
            return vec3(1, 0, 0);

        #pragma warning(disable : 26451)
        int i = static_cast<int>(u * (width - 1));
        int j = static_cast<int>((1 - v) * (height - 1));

        // These cases should never happen
        if (i < 0) i = 0;
        if (j < 0) j = 0;

        double r = static_cast<int>(data[3 * i + 3 * width * j + 0]) / 255.0;
        double g = static_cast<int>(data[3 * i + 3 * width * j + 1]) / 255.0;
        double b = static_cast<int>(data[3 * i + 3 * width * j + 2]) / 255.0;

        return vec3(r, g, b);
    }

    unsigned char* data;
    int width, height;
};
