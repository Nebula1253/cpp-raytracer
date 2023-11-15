#ifndef SCENE_H
#define SCENE_H

#include "color.h"
#include "shape.h"
#include "pointlight.h"
#include <vector>

class scene {
    private:
        color background_color;
        std::vector<shape*> shapes;
        std::vector<pointlight*> lightsources;

    public:
        scene(color background_color, std::vector<shape*> shapes) : 
            background_color(background_color), 
            shapes(shapes){};

        scene(color background_color, std::vector<shape*> shapes, std::vector<pointlight*> lightsources) : 
            background_color(background_color), 
            shapes(shapes),
            lightsources(lightsources){};

        std::vector<shape*> getShapes() const { return shapes; };
        std::vector<pointlight*> getLights() const { return lightsources; }
        color getBackgroundColor() const { return background_color; };
};

#endif