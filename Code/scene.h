#ifndef SCENE_H
#define SCENE_H

#include "color.h"
#include "shape.h"
#include <vector>

class scene {
    private:
        color background_color;
        std::vector<shape*> shapes;

    public:
        scene(color background_color, std::vector<shape*> shapes) : background_color(background_color), shapes(shapes) {};

        std::vector<shape*> getShapes() const { return shapes; };
        color getBackgroundColor() const { return background_color; };
};

#endif