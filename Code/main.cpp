// Taken from Raytracing in One Weekend, modified from there
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "triangle.h"
#include "cylinder.h"
#include "camera.h"
#include <vector>
#include "shape.h"
#include "scene.h"

#include <iostream>

color ray_color(const ray& r, scene& s) {
    for (int i = 0; i < s.getShapes().size(); ++i) {
        bool intersect = s.getShapes()[i]->intersection(r);
        if (intersect) return color(1,0,0);
    }
    return s.getBackgroundColor();
}

int main() {
    
    // // Image
    // auto aspect_ratio = 16.0 / 9.0;
    // int image_width = 400;

    // // Calculate the image height, and ensure that it's at least 1.
    // int image_height = static_cast<int>(image_width / aspect_ratio);
    // image_height = (image_height < 1) ? 1 : image_height;

    // // Camera

    // auto focal_length = 1.0;
    // auto viewport_height = 2.0;
    // auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    // auto camera_center = point3(0, 0, 0);

    // // Calculate the vectors across the horizontal and down the vertical viewport edges.
    // auto viewport_u = vec3(viewport_width, 0, 0);
    // auto viewport_v = vec3(0, -viewport_height, 0);

    // // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    // auto pixel_delta_u = viewport_u / image_width;
    // auto pixel_delta_v = viewport_v / image_height;

    // // Calculate the location of the upper left pixel.
    // auto viewport_upper_left = camera_center
    //                          + vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // std::cerr << "viewport_upper_left: " << viewport_upper_left << "\n";
    // auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    camera cam = camera(400, static_cast<int>(400 / (16.0 / 9.0)), point3(0,0,0), vec3(0,0,1), vec3(0,1,0), 90, 1.0);

    // Scene
    sphere s(point3(2,0,1), 0.5);
    // triangle tri(point3(0,0,-1), point3(0,0.25,-1), point3(0.25,0.25,-1));
    triangle tri(point3(0.5,0.5,1), point3(0,0.5,1), point3(0,0,1));
    cylinder cyl(point3(0,0,1), vec3(0,0,1), 0.5, 0.25);
    std::vector<shape*> shapes;
    shapes.push_back(&s);
    shapes.push_back(&tri);
    shapes.push_back(&cyl);
    // shapes.push_back(cyl);

    scene sce = scene(color(0,0,0), shapes);

    std::cout << "P3\n" << cam.getWidth() << " " << cam.getHeight() << "\n255\n";

    for (int j = 0; j < cam.getHeight(); ++j) {
        std::clog << "\rScanlines remaining: " << (cam.getHeight() - j) << ' ' << std::flush;
        for (int i = 0; i < cam.getWidth(); ++i) {
            auto pixel_center = cam.getPixel00Loc() + (i * cam.getPixelDeltaU()) + (j * cam.getPixelDeltaV());
            auto ray_direction = pixel_center - cam.getPosition();
            ray r(cam.getPosition(), ray_direction);

            color pixel_color = ray_color(r, sce);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}