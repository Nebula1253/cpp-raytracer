// Taken from Raytracing in One Weekend: Listing 5
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using Color = Vec3;

Color clamp(Color color, double min = 0.0, double max = 1.0) {
    Color clamped_color = color;
    for (int i = 0; i < 3; i++) {
        if (clamped_color[i] < min) {
            clamped_color[i] = min;
        }
        if (clamped_color[i] > max) {
            clamped_color[i] = max;
        }
    }
    return clamped_color;
}

void writeColor(std::ostream &out, Color pixel_color, double max_color_value = 255.999) {
    // Write the translated [0,255] value of each color component - the pixel color here is a vec3 with values in [0,1]
    out << static_cast<int>(std::floor(max_color_value * clamp(pixel_color).x())) << ' '
        << static_cast<int>(std::floor(max_color_value * clamp(pixel_color).y())) << ' '
        << static_cast<int>(std::floor(max_color_value * clamp(pixel_color).z())) << '\n';
}

#endif