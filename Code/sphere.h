#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
#include "material.h"
#include "utility.h"
using std::sqrt;


class sphere : public Shape {
    private:
        Point3 center;
        double radius;
        Material mat;

        void initialiseBoundingBox() {
            boundingBox = BoundingBox(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
        }
    public:
        sphere() {}
        sphere(const Point3& center, double radius)
            : center(center), radius(radius)
        {
            initialiseBoundingBox();
        }
        sphere(const Point3& center,  const Material& mat, double radius)
            : center(center), radius(radius), mat(mat)
        {
            initialiseBoundingBox();
        }

        Point3 get_center() const { return center; }
        double get_radius() const { return radius; }

        Material getMaterial() const override {return mat;}

        double intersection(const Ray& r) const override {
            Vec3 oc = r.origin() - center;

            // we have an equation to solve: 
            auto a = dot(r.direction(), r.direction());
            auto b = 2.0 * dot(oc, r.direction());
            auto c = dot(oc, oc) - radius*radius;
            auto discriminant = b*b - 4*a*c;

            if (discriminant < 0) return -1; // no intersection
            else {
                auto small_t = (-b - sqrt(discriminant)) / (2.0 * a);
                if (small_t < 0) return -1; // no intersection
                else return small_t;
            }
        }

        // copilot babey
        Vec3 getNormal(Point3 point) const override {
            return unit_vector(point - center);
        }

        Color getDiffuseColor(Point3 point) const override {
            if (!mat.getHasTexture()) {
                return mat.getDiffuseColor();
            }
            else {
                Vec3 normalVector = getNormal(point);
                auto u = asin(normalVector.x()) / pi + 0.5;
                auto v = asin(-normalVector.y()) / pi + 0.5;

                return mat.getTexture().getColorAtPixel(u, v);
            }
        }

        void getType() const override {
            std::cerr << "sphere" << std::endl;
        }
};
#endif