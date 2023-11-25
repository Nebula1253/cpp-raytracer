// Taken from Raytracing in One Weekend: Listing 5
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using Color = Vec3;

void writeColor(std::ostream &out, Color pixel_color, double max_color_value = 255.999) {
    // Write the translated [0,255] value of each color component - the pixel color here is a vec3 with values in [0,1]
    out << static_cast<int>(max_color_value * pixel_color.x()) << ' '
        << static_cast<int>(max_color_value * pixel_color.y()) << ' '
        << static_cast<int>(max_color_value * pixel_color.z()) << '\n';
}

#endif