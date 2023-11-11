#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class camera {
    private:
        int width;
        int height;
        point3 position;
        vec3 lookAt;
        vec3 upVector;
        double fov;
        double exposure;

    public:
        camera() :
            width(1920), height(1080), lookAt(0, 0, 1), upVector(0, 1, 0) {}
        camera(int width, int height, point3 position, vec3 lookAt, vec3 upVector, double fov, double exposure) :
            width(width), height(height), position(position), lookAt(lookAt), upVector(upVector), fov(fov), exposure(exposure) {}

        // implement the actual code from main.cpp here later

};


#endif