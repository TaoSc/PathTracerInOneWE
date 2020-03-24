#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdlib>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vec3.h"
#include "random.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


constexpr float MAX_FLOAT = 100.f;
int max_bounces = 5;


vec3 compute_color(const ray& r, hittable_list world, int bounces = 0) {
    hit_record rec;

    // hit an object
    if (world.hit(r, 0.001f, MAX_FLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (bounces < max_bounces && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * compute_color(scattered, world, ++bounces);
        } else {
            return vec3();
        }
    }
    // sky
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + // white contribution
            t * vec3(0.5f, 0.7f, 1.0f); // blue contribution
    }
}

int main(int argc, char* argv[]) {
    constexpr int channels = 3;
    std::string filename = "output";
    int width = 400; int height = 200, samples = 50;
    float fov = 40.f, aperture = 0.0f, focus_dist = 0.f;
    vec3 cam_pos(0.f, 0.f, 1.5f), look_at(0.f, 0.f, -1.f);
    for (size_t i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0)
            filename = argv[i + 1];

        else if (strcmp(argv[i], "-w") == 0)
            width = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-h") == 0)
            height = atoi(argv[i + 1]);

        else if (strcmp(argv[i], "-s") == 0)
            samples = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-b") == 0)
            max_bounces = atoi(argv[i + 1]);

        else if (strcmp(argv[i], "-fov") == 0)
            fov = std::stof(argv[i + 1]);

        else if (strcmp(argv[i], "-cam") == 0)
            cam_pos = vec3(std::stof(argv[i + 1]), std::stof(argv[i + 2]), std::stof(argv[i + 3]));
        else if (strcmp(argv[i], "-look") == 0)
            look_at = vec3(std::stof(argv[i + 1]), std::stof(argv[i + 2]), std::stof(argv[i + 3]));

        else if (strcmp(argv[i], "-a") == 0)
            aperture = std::stof(argv[i + 1]);
        else if (strcmp(argv[i], "-focus") == 0)
            focus_dist = std::stof(argv[i + 1]);
    }

    unsigned char* data = static_cast<unsigned char*>(malloc(static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels)));
    if (!data) {
        std::cerr << "Failure to allocate on heap. Aborting." << std::endl;
        return -1;
    }
    int index = 0;

    std::cout << "Outputting to \"" << filename << ".png\" (" << width << "x" << height << ") at " << samples << " sppx." << std::endl
              << max_bounces << " bounce(s) max." << std::endl;


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    float aspect_ratio = width / static_cast<float>(height);
    float u, v;
    ray r;
    vec3 color;

    hittable* list[4];
    list[0] = new sphere(vec3(0.f, -100.5f, -1.f), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[1] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[2] = new sphere(vec3(1.f, 0.f, -1.f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.0f));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.5, 0.3, 0.8), 1.0));
    hittable_list world(list, 4);

    if (!focus_dist)
        focus_dist = (cam_pos - look_at).length();
    camera cam(cam_pos, look_at, vec3(0.f, 1.f, 0.f), fov, aspect_ratio, aperture, focus_dist);

    for (size_t y = height; y > 0; y--) {
        for (size_t x = 0; x < width; x++) {
            color = vec3();
            for (size_t sample_i = 0; sample_i < samples; sample_i++) {
                u = float(x + random_double()) / float(width);
                v = float(y + random_double()) / float(height);
                r = cam.get_ray(u, v);
                color += compute_color(r, world, 0);
            }
            color /= static_cast<float>(samples);


            data[index++] = static_cast<unsigned char>(255.99f * sqrt(color[0]));
            data[index++] = static_cast<unsigned char>(255.99f * sqrt(color[1]));
            data[index++] = static_cast<unsigned char>(255.99f * sqrt(color[2]));
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::endl << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "s." << std::endl;

    stbi_write_png(filename.append(".png").c_str(), width, height, channels, data, 0);
}
