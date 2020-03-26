#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdlib>
#include <memory>

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
#include "moving_sphere.h"
#include "bvh_node.h"
#include "texture.h"


constexpr float MAX_FLOAT = 100.f;
int max_bounces = 5;

using std::make_shared;

bvh_node two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(
        make_shared<const_texture>(vec3(0.2, 0.3, 0.1)),
        make_shared<const_texture>(vec3(0.9, 0.9, 0.9))
        );

    objects.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return  bvh_node(objects, 0, 1);
}

bvh_node random_scene() {
    hittable_list world;
    auto checker = make_shared<checker_texture>(make_shared<const_texture>(vec3(0.2, 0.3, 0.1)), make_shared<const_texture>(vec3(0.9, 0.9, 0.9)), 15);
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) { // diffuse
                    auto albedo = make_shared<const_texture>(vec3(random_double(), random_double(), random_double()) * vec3(random_double(), random_double(), random_double()));

                    if (random_double() > 0.5) {
                        world.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
                    } else {
                        world.add(make_shared<moving_sphere>(center, center + vec3(0, random_double(0, .5), 0), 0.0, 1.0, 0.2, make_shared<lambertian>(albedo)));
                    }
                }
                else if (choose_mat < 0.95) { // metal
                    auto albedo = make_shared<const_texture>(vec3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)));
                    auto fuzz = random_double(0, .5);
                    world.add(make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
                }
                else { // glass
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(make_shared<const_texture>(vec3(0.4, 0.2, 0.1)))));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(make_shared<const_texture>(vec3(0.7, 0.6, 0.5)), 0.0)));

    return bvh_node(world, 0, 1);
}


vec3 compute_color(const ray& r, const hittable& world, int bounces = 0) {
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
        double t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + // white contribution
                        t * vec3(0.5, 0.7, 1.0);  // blue contribution
    }
}

int main(int argc, char* argv[]) {
    constexpr int channels = 3;
    std::string filename = "output";
    int width = 400; int height = 200, samples = 20;
    double fov = 20, aperture = 0.1, focus_dist = 10;
    vec3 cam_pos(13, 2, 3), look_at(0, 0, 0);
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

    if (!focus_dist)
        focus_dist = (cam_pos - look_at).length();
    camera cam(cam_pos, look_at, vec3(0., 1., 0.), fov, aspect_ratio, aperture, focus_dist, 0, 1);
    auto world = two_spheres();


    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            color = vec3();
            for (size_t sample_i = 0; sample_i < samples; sample_i++) {
                u = float(x + random_double()) / float(width);
                v = float(height - (y + random_double())) / float(height);
                r = cam.get_ray(u, v);
                color += compute_color(r, world, 0);
            }
            color /= static_cast<float>(samples);
            color.output(data, &index);
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::endl << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "s." << std::endl;

    stbi_write_png(filename.append(".png").c_str(), width, height, channels, data, 0);
}
