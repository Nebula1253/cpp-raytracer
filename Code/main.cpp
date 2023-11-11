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
#include "json.hpp"
using json = nlohmann::json;
#include <fstream>

#include <iostream>

color ray_color(const ray& r, scene& s) {
    for (int i = 0; i < s.getShapes().size(); ++i) {
        bool intersect = s.getShapes()[i]->intersection(r);
        if (intersect) return color(1,0,0);
    }
    return s.getBackgroundColor();
}

int main(int argc, char *argv[]) {
    // assuming the file name of the JSON file is passed as the first argument
    std::string filepath = __FILE__;
    filepath.erase(filepath.length() - 13);

    std::ifstream jsonFile(filepath + argv[1]);
    json j = json::parse(jsonFile);

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