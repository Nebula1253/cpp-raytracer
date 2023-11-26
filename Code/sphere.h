
#include "shape.h"
#include "material.h"
using std::sqrt;

// ALL GENERATED BY COPILOT
class sphere : public Shape {
    // class definition
    private:
        Point3 center;
        double radius;
        Material mat;
    public:
        sphere() {}
        sphere(const Point3& center, double radius)
            : center(center), radius(radius)
        {
            boundingBox = BoundingBox(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
        }
        sphere(const Point3& center,  const Material& mat, double radius)
            : center(center), radius(radius), mat(mat)
        {
            boundingBox = BoundingBox(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
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
                auto v = asin(normalVector.y()) / pi + 0.5;

                return mat.getTexture().getColorAtPixel(u, v);
            }
        }
};
