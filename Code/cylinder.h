#ifndef CYLINDER_H
#define CYLINDER_H
#include "shape.h"
#include "vec3.h"
#include <vector>

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
            vec3 bottom = center - (axis * height);

            vec3 oc = r.origin() - bottom;

            // not part of the original copilot output: added by copilot later once 
            // I had a different reference for cylinders with an arbitrary axis open in Chrome: oh god copilot is going to kill us all
            vec3 dir = r.direction();
            double a = dot(dir, dir) - (dot(dir, axis) * dot(dir, axis));
            double b = 2.0 * (dot(oc, dir) - (dot(oc, axis) * dot(dir, axis)));
            double c = dot(oc, oc) - (dot(oc, axis) * dot(oc, axis)) - (radius * radius);

            std::vector<double> t_values;

            double discriminant = b*b - 4*a*c;
            if (discriminant >= 0) {
                double sqrt_discriminant = sqrt(discriminant);
                double t = (-b - sqrt_discriminant) / (2.0 * a);

                // co-pilot generated, yet again: not part of the original response though
                double m = dot(r.direction(), axis) * t + dot(oc, axis);

                if (m <= 2*height && m >= 0) {
                    if (t >= 0) t_values.push_back(t);
                }
            }
            
            // Top and bottom cap intersection
            // largely generated line-by-line by copilot, when i had a ray-disk intersection tutorial open in chrome
            // refactored during blinn phong implementation, largely done manually
            auto bottomCapCentre = bottom;
            auto topCapCentre = center + (axis * height);

            auto bottomCapNormal = -axis;
            auto topCapNormal = axis;

            auto bottom_t = dot(bottomCapNormal, bottomCapCentre - r.origin()) / dot(bottomCapNormal, r.direction());
            auto top_t = dot(topCapNormal, topCapCentre - r.origin()) / dot(topCapNormal, r.direction());

            auto bottomCapIntersectionPt = r.origin() + bottom_t * r.direction();
            auto topCapIntersectionPt = r.origin() + top_t * r.direction();

            auto bottomCapRadius = (bottomCapIntersectionPt - bottomCapCentre).length();
            auto topCapRadius = (topCapIntersectionPt - topCapCentre).length();

            if (bottomCapRadius <= radius && bottom_t >= 0) t_values.push_back(bottom_t);
            if (topCapRadius <= radius && top_t >= 0) t_values.push_back(top_t);

            if (t_values.size() == 0) return -1;
            double min_t = t_values[0];
            for (int i = 1; i < t_values.size(); i++) {
                if (t_values[i] < min_t) min_t = t_values[i];
            }
            return min_t;
        }

        vec3 get_normal(point3 point) const override {
            // Top and bottom cap intersection
            auto top = center + (axis * height);
            auto bottom = center - (axis * height);

            if ((top - point).length() <= radius) {
                return axis;
            }
            if ((bottom - point).length() <= radius) {
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