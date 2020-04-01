#pragma once

#include "textures/texture.h"
#include "utilities/vec3.h"

class image_texture : public texture {
public:
    image_texture() : data(nullptr), width(0), height(0) {}
    image_texture(unsigned char* pixels, int w, int h) : data(pixels), width(w), height(h) {}

    ~image_texture() { delete data; }

    virtual vec3 value(const double&u, const double& v, const vec3& p) const override {
        if (data == nullptr) // If no data, return bright red
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

private:
    unsigned char* data;
    int width, height;
};