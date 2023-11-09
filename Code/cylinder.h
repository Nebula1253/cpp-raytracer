#ifndef CYLINDER_H
#define CYLINDER_H
#include "shape.h"
#include "vec3.h"

class cylinder : public shape {
    private:
        point3 center;
        vec3 axis;
        double radius;
        double height;

    public:
        cylinder() {}
        cylinder(const point3& center, const vec3& axis, double radius, double height) :
            center(center), axis(axis), radius(radius), height(height) {}

        point3 get_center() const {return center;}
        vec3 get_axis() const {return axis;}
        double get_radius() const {return radius;}
        double get_radius() const {return height;}

        bool intersection(const ray& r) const {
            
            return true;
        }
};

#endif