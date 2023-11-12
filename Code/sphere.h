
#include "shape.h"
using std::sqrt;

// ALL GENERATED BY COPILOT
class sphere : public shape {
    // class definition
    private:
        point3 center;
        double radius;
    public:
        sphere() {}
        sphere(const point3& center, double radius)
            : center(center), radius(radius)
        {}

        point3 get_center() const { return center; }
        double get_radius() const { return radius; }

        bool intersection(const ray& r) const override {
            std::cerr << "THIS IS THE SPHERE INTERSECTION" << std::endl;
            vec3 oc = r.origin() - center;

            // we have an equation to solve: 
            auto a = dot(r.direction(), r.direction());
            auto b = 2.0 * dot(oc, r.direction());
            auto c = dot(oc, oc) - radius*radius;
            auto discriminant = b*b - 4*a*c; // the square root term in the quadratic formula

            return (discriminant >= 0);

            // auto t1 = -b + sqrt(discriminant) / (2.0*a);
            // auto t2 = -b - sqrt(discriminant) / (2.0*a);

            // if (t1 < 0 && t2 < 0) return false; // both solutions are negative, meaning the object is BEHIND the camera, so who cares lol
            // else return true;
        }
};
