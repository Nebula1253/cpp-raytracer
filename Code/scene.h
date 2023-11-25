#ifndef SCENE_H
#define SCENE_H

#include "color.h"
#include "shape.h"
#include "pointlight.h"
#include <vector>

class Scene {
    private:
        Color background_color;
        std::vector<Shape*> shapes;
        std::vector<PointLight*> lightsources;

    public:
        Scene() {};
        
        Scene(Color background_color, std::vector<Shape*> shapes) : 
            background_color(background_color), 
            shapes(shapes){};

        Scene(Color background_color, std::vector<Shape*> shapes, std::vector<PointLight*> lightsources) : 
            background_color(background_color), 
            shapes(shapes),
            lightsources(lightsources){};

        std::vector<Shape*> getShapes() const { return shapes; };
        std::vector<PointLight*> getLights() const { return lightsources; }
        Color getBackgroundColor() const { return background_color; };
};

#endif