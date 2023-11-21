// Taken from RT in One Weekend
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}

// Common Headers

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

#endif