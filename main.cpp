#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdlib>
#include <memory>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "externals/stb_image_write.h"

#include "camera.h"
#include "utilities/vec3.h"
#include "utilities/random.h"
#include "utilities/ray.h"
#include "hittables/hittable.h"
#include "hittables/bvh_node.h"
#include "textures/texture.h"
#include "materials/material.h"
#include "scenes/scene.h"


constexpr float MAX_DIST = 1000.f;
unsigned int max_bounces = 3, max_samples = 20;
unsigned int width = 300, height = 300;

vec3 compute_color(const ray& r, const vec3& background, const hittable& world, int bounces) {
    hit_record rec;

    if (bounces <= 0)
        return vec3(0, 0, 0);

    if (!world.hit(r, 0.001, MAX_DIST, rec))
        return background;

    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.point);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * compute_color(scattered, background, world, bounces - 1);
}

void shoot_ray(unsigned char* result, const size_t& thread_no, const size_t& samples_per_thread, const camera& cam, const bvh_node& world, const vec3& background) {
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
                color += compute_color(r, background, world, max_bounces);
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

    book2_scene cur_scene;
    double fov = cur_scene.fov(), aperture = cur_scene.aperture(), focus_dist = cur_scene.focus_dist();
    vec3 cam_pos = cur_scene.cam(), look_at = cur_scene.lookat();
    vec3 background = cur_scene.background();

    unsigned int max_threads = 0;
    for (size_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
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

        unsigned int allocated_size = width * height * channels;
        if (!max_threads)
            max_threads = std::thread::hardware_concurrency();
        if (max_samples < max_threads)
            max_threads = max_samples;

        std::vector<std::unique_ptr<std::thread>> threads_queue;
        threads_queue.reserve(max_threads);
        std::vector<unsigned char*> results;
        results.reserve(max_threads);
        bvh_node world = cur_scene.descr();

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

            buffer = std::make_unique<std::thread>(shoot_ray, results[i], i, samples_per_thread, cam, world, background);
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
