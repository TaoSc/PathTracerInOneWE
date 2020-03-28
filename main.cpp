#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdlib>
#include <memory>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image_write.h"
#include "externals/stb_image.h"

#include "camera.h"
#include "utilities/vec3.h"
#include "utilities/random.h"
#include "utilities/ray.h"
#include "hittables/hittable.h"
#include "hittables/hittable_list.h"
#include "hittables/sphere.h"
#include "hittables/moving_sphere.h"
#include "hittables/aa_rect.h"
#include "hittables/box.h"
#include "hittables/flip_face.h"
#include "hittables/translate.h"
#include "hittables/rotate.h"
#include "hittables/bvh_node.h"
#include "materials/material.h"
#include "materials/dielectric.h"
#include "textures/texture.h"
#include "textures/image_texture.h"


constexpr float MAX_DIST = 1000.f;
unsigned int max_bounces = 5, max_samples = 20;
unsigned int width = 300, height = 300;

using std::make_shared;

bvh_node random_scene() {
    hittable_list world;

    int tex_width, tex_height, channels;
    auto texture = make_shared<image_texture>(stbi_load("earthmap.jpg", &tex_width, &tex_height, &channels, 0), tex_width, tex_height);
    auto checker = make_shared<checker_texture>(make_shared<const_texture>(vec3(0.2, 0.3, 0.1)), make_shared<const_texture>(vec3(0.9, 0.9, 0.9)), 300);
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

    world.add(make_shared<xy_rect>(3, 5, 1, 3, -2, make_shared<diffuse_light>(make_shared<const_texture>(vec3(8)))));
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<diffuse_light>(make_shared<const_texture>(vec3(7)))));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(texture, 0.3)));

    return bvh_node(world, 0, 1);
}

bvh_node cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(make_shared<const_texture>(vec3(0.65, 0.05, 0.05)));
    auto white = make_shared<lambertian>(make_shared<const_texture>(vec3(0.73, 0.73, 0.73)));
    auto green = make_shared<lambertian>(make_shared<const_texture>(vec3(0.12, 0.45, 0.15)));
    auto light = make_shared<diffuse_light>(make_shared<const_texture>(vec3(15)));

    objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<flip_face>(make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

    shared_ptr<hittable> box1 = make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);

    return bvh_node(objects, 0, 1);
}

vec3 compute_color(const ray& r, const vec3& background, const hittable& world, int bounces) {
    hit_record rec;

    if (bounces <= 0)
        return vec3(0, 0, 0);

    if (!world.hit(r, 0.001f, MAX_DIST, rec))
        return background;

    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.point);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * compute_color(scattered, background, world, bounces - 1);
}

void shoot_ray(unsigned char* result, const size_t& thread_no, const size_t& samples_per_thread, const camera& cam, const bvh_node& world) {
    srand(static_cast<unsigned int>(time(nullptr) * (thread_no + 1)));

    float u, v;
    ray r;
    vec3 color;
    size_t index = 0;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            color = vec3();
            for (size_t sample_i = 0; sample_i < samples_per_thread; sample_i++) {
                u = float(x + random_double()) / float(width);
                v = float(height - (y + random_double())) / float(height);
                r = cam.get_ray(u, v);
                color += compute_color(r, vec3(/*0.47, 0.29, 0.06*/), world, max_bounces);
            }
            color /= static_cast<float>(samples_per_thread);
            color.output(result, &index);
        }
    }
}

void show_help() {
    std::cout << "At one point this should be helpful.\nIn the meantime, the README will suffice." << std::endl;
}

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    constexpr int channels = 3;
    std::string filename = "output";
    double fov = 40, aperture = 0, focus_dist = 10;
    vec3 cam_pos(278, 278, -800), look_at(278, 278, 0);
    unsigned int max_threads = 0;
    for (size_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help") || !strcmp(argv[i], "-h")) {
            show_help();
            goto program_end;
        }

        else if (strcmp(argv[i], "-f") == 0)
            filename = argv[i + 1];

        else if (strcmp(argv[i], "-t") == 0)
            max_threads = atoi(argv[i + 1]);

        else if (strcmp(argv[i], "-w") == 0)
            width = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-h") == 0)
            height = atoi(argv[i + 1]);

        else if (strcmp(argv[i], "-s") == 0)
            max_samples = atoi(argv[i + 1]);
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

    {
        float aspect_ratio = width / static_cast<float>(height);
        if (!focus_dist)
            focus_dist = (cam_pos - look_at).length();
        camera cam(cam_pos, look_at, vec3(0., 1., 0.), fov, aspect_ratio, aperture, focus_dist, 0, 1);
        bvh_node world = cornell_box();

        unsigned int allocated_size = width * height * channels;
        if (!max_threads)
            max_threads = std::thread::hardware_concurrency();
        if (max_samples < max_threads)
            max_threads = max_samples;

        std::vector<std::unique_ptr<std::thread>> threads_queue;
        threads_queue.reserve(max_threads);
        std::vector<unsigned char*> results;
        results.reserve(max_threads);


        unsigned int samples_per_thread = max_samples / max_threads;
        std::cout << "Outputting to \"" << filename << ".png\" (" << width << "x" << height << ") at " << samples_per_thread * max_threads << " sppx." << std::endl
                  << max_bounces << " bounce(s) max." << std::endl
                  << max_threads << " working thread(s) with " << samples_per_thread << " sample(s) per thread." << std::endl;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::unique_ptr<std::thread> buffer = nullptr;
        for (size_t i = 0; i < max_threads; i++) {
            // allocate mem for each thread
            results.push_back(static_cast<unsigned char*>(malloc(allocated_size)));
            if (!results[i]) {
                std::cerr << "Failure to allocate on heap. Aborting." << std::endl;
                return EXIT_FAILURE;
            }

            buffer = std::make_unique<std::thread>(shoot_ray, results[i], i, samples_per_thread, cam, world);
            threads_queue.push_back(move(buffer));
        }

        // waiting for ray gen to finish on all threads
        for (size_t i = 0; i < max_threads; i++) {
            threads_queue[i]->join();
        }

        // sum computation of each threads
        unsigned char* data = static_cast<unsigned char*>(malloc(allocated_size));
        if (!data) {
            std::cerr << "Failure to allocate on heap. Aborting." << std::endl;
            return EXIT_FAILURE;
        }
        int index = 0;
        for (size_t i = 0; i < allocated_size; i++) {
            unsigned int temp = 0;
            for (size_t j = 0; j < max_threads; j++)
                temp += results[j][i];

            data[index++] = temp / max_threads;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << std::endl << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "s." << std::endl;

        stbi_write_png(filename.append(".png").c_str(), width, height, channels, data, 0);
        free(data);
    }

program_end:
    return EXIT_SUCCESS;
}
