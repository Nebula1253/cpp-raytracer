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
        material mat;

    public:
        cylinder() {}
        cylinder(const point3& center, const vec3& axis, double radius, double height) :
            center(center), axis(unit_vector(axis)), radius(radius), height(height) {}
        cylinder(const point3& center, const vec3& axis, const material& mat, double radius, double height) :
            center(center), axis(unit_vector(axis)), radius(radius), height(height), mat(mat) {}

        point3 get_center() const {return center;}
        vec3 get_axis() const {return axis;}
        double get_radius() const {return radius;}
        double get_height() const {return height;}
        material get_material() const override {return mat;}

        // generated by copilot, prompt: write a method for checking if a ray intersects with a cylinder
        double intersection(const ray& r) const override {
            // std::cerr << "THIS IS THE CYLINDER INTERSECTION" << std::endl;
            bool curvedSurfaceIntersection = false, capsIntersection = false;
            vec3 bottom = center - (axis * height);

            vec3 oc = r.origin() - bottom;

            // not part of the original copilot output: added by copilot later once 
            // I had a different reference for cylinders with an arbitrary axis open in Chrome: oh god copilot is going to kill us all
            vec3 dir = r.direction();
            double a = dot(dir, dir) - (dot(dir, axis) * dot(dir, axis));
            double b = 2.0 * (dot(oc, dir) - (dot(oc, axis) * dot(dir, axis)));
            double c = dot(oc, oc) - (dot(oc, axis) * dot(oc, axis)) - (radius * radius);
            

            double discriminant = b*b - 4*a*c;
            if (discriminant >= 0) {
                double sqrt_discriminant = sqrt(discriminant);
                double t0 = (-b - sqrt_discriminant) / (2.0 * a);
                double t1 = (-b + sqrt_discriminant) / (2.0 * a);

                // co-pilot generated, yet again: not part of the original response though
                double m1 = dot(r.direction(), axis) * t0 + dot(oc, axis);
                double m2 = dot(r.direction(), axis) * t1 + dot(oc, axis);

                curvedSurfaceIntersection = ((m1 <= 2*height && m1 >= 0) || (m2 <= 2*height && m2 >= 0));
                if (curvedSurfaceIntersection) {
                    //copilot again
                    if (t0 < 0) return t1;
                    if (t1 < 0) return t0;
                    return (t0 < t1) ? t0 : t1;
                }
            }
            
            // IMPLEMENT PLANE INTERSECTIONS AT CAPS LOL
            // largely generated line-by-line by copilot, when i had a ray-disk intersection tutorial open in chrome
            auto plane1point = bottom;
            auto plane2point = center + (axis * height);

            auto plane1normal = -axis;
            auto plane2normal = axis;

            auto t1 = dot(plane1normal, plane1point - r.origin()) / dot(plane1normal, r.direction());
            auto t2 = dot(plane2normal, plane2point - r.origin()) / dot(plane2normal, r.direction());

            auto plane1intersection = r.origin() + t1 * r.direction();
            auto plane2intersection = r.origin() + t2 * r.direction();

            auto plane1dist = (plane1intersection - plane1point).length();
            auto plane2dist = (plane2intersection - plane2point).length();

            capsIntersection = ((plane1dist <= radius && t1 >= 0) || (plane2dist <= radius && t2 >= 0));

            if (capsIntersection) {
                if (t1 < 0) return t2;
                if (t2 < 0) return t1;
                return (t1 < t2) ? t1 : t2;
            }
            // return (curvedSurfaceIntersection || capsIntersection);
        }

        vec3 get_normal(point3 point) const override {
            // Top and bottom cap intersection
            auto top = center + (axis * height);
            auto bottom = center - (axis * height);

            if ((top - point).length() < radius) {
                return axis;
            }
            if ((bottom - point).length() < radius) {
                return -axis;
            }

            // Curved surface intersection
            // generated by copilot when https://stackoverflow.com/questions/36266357/how-can-i-compute-normal-on-the-surface-of-a-cylinder was open in my browser
            auto t = dot(point - bottom, axis);
            auto p = bottom + t * axis;
            auto n = unit_vector(point - p);
            return n;
        }
};

#endif