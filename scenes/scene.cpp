#include "scene.h"

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

#include "hittables/hittable.h"
#include "hittables/hittable_list.h"
#include "hittables/sphere.h"
#include "hittables/moving_sphere.h"
#include "hittables/aa_rect.h"
#include "hittables/box.h"
#include "hittables/constant_medium.h"
#include "hittables/flip_face.h"
#include "hittables/translate.h"
#include "hittables/rotate.h"
#include "hittables/bvh_node.h"
#include "materials/material.h"
#include "materials/dielectric.h"
#include "textures/image_texture.h"

using std::make_shared;

bvh_node random_scene::descr() const {
    hittable_list objects;

    int tex_width, tex_height, channels;
    auto texture = make_shared<image_texture>(stbi_load("earthmap.jpg", &tex_width, &tex_height, &channels, 0), tex_width, tex_height);
    auto checker = make_shared<checker_texture>(make_shared<const_texture>(vec3(0.2, 0.3, 0.1)), make_shared<const_texture>(vec3(0.9, 0.9, 0.9)), 300);
    objects.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) { // diffuse
                    auto albedo = make_shared<const_texture>(vec3(random_double(), random_double(), random_double()) * vec3(random_double(), random_double(), random_double()));

                    if (random_double() > 0.5)
                        objects.add(make_shared<sphere>(center, 0.2, make_shared<lambertian>(albedo)));
                    else
                        objects.add(make_shared<moving_sphere>(center, center + vec3(0, random_double(0, .5), 0), 0.0, 1.0, 0.2, make_shared<lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) { // metal
                    auto albedo = make_shared<const_texture>(vec3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)));
                    auto fuzz = random_double(0, .5);
                    objects.add(make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
                }
                else { // glass
                    objects.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, make_shared<diffuse_light>(make_shared<const_texture>(vec3(8)))));
    objects.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<diffuse_light>(make_shared<const_texture>(vec3(7)))));
    objects.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(texture, 0.3)));

    return bvh_node(objects, 0, 1);
}

bvh_node cornell_box::descr() const {
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
    //objects.add(box1);
    objects.add(make_shared<constant_medium>(box1, 0.01, make_shared<const_texture>(vec3(0))));

    shared_ptr<hittable> box2 = make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    //objects.add(box2);
    objects.add(make_shared<constant_medium>(box2, 0.01, make_shared<const_texture>(vec3(1))));

    return bvh_node(objects, 0, 1);
}

bvh_node basic_scene::descr() const {
    hittable_list objects;

    objects.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(make_shared<const_texture>(vec3(0.7, 0.3, 0.3)))));

    objects.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(make_shared<const_texture>(vec3(0.8, 0.8, 0.0)))));

    objects.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(make_shared<const_texture>(vec3(0.8, 0.6, 0.2)), 0.3)));
    objects.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(make_shared<const_texture>(vec3(0.8, 0.8, 0.8)), 1)));

    return bvh_node(objects, 0, 1);
}

bvh_node book2_scene::descr() const {
    hittable_list objects;

    hittable_list boxes1;
    auto ground = make_shared<lambertian>(make_shared<const_texture>(vec3(0.48, 0.83, 0.53)));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(vec3(x0, y0, z0), vec3(x1, y1, z1), ground));
        }
    }

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(make_shared<const_texture>(vec3(7, 7, 7)));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = vec3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(make_shared<const_texture>(vec3(0.7, 0.3, 0.1)));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(vec3(0, 150, 145), 50, make_shared<metal>(make_shared<const_texture>(vec3(0.8, 0.8, 0.9)), 10.0)));

    auto boundary = make_shared<sphere>(vec3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, make_shared<const_texture>(vec3(0.2, 0.4, 0.9))));
    boundary = make_shared<sphere>(vec3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, make_shared<const_texture>(vec3(1, 1, 1))));

    int nx, ny, nn;
    auto tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    auto emat = make_shared<lambertian>(make_shared<image_texture>(tex_data, nx, ny));
    objects.add(make_shared<sphere>(vec3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(vec3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(make_shared<const_texture>(vec3(0.73, 0.73, 0.73)));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
        boxes2.add(make_shared<sphere>(vec3(random_int(0, 165), random_int(0, 165), random_int(0, 165)), 10, white));

    objects.add(make_shared<translate>(make_shared<rotate_y>(make_shared<bvh_node>(boxes2, 0.0, 1.0), 15), vec3(-100, 270, 395)));

    return bvh_node(objects, 0, 1);
}

bvh_node perlin_scene::descr() const {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return bvh_node(objects, 0, 1);
}
