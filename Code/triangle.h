#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"

class triangle : public Shape {
    private:
        Point3 vertices[3];
        Material mat;

        void get_barycentric_coordinates (double& alpha, double& beta, double& gamma, const Point3& point) const{
            // taken from chatgpt and modified
            Vec3 v0 = vertices[1] - vertices[0];
            Vec3 v1 = vertices[2] - vertices[0];
            Vec3 v2 = point - vertices[0];

            double d00 = dot(v0, v0);
            double d01 = dot(v0, v1);
            double d11 = dot(v1, v1);
            double d20 = dot(v2, v0);
            double d21 = dot(v2, v1);

            double denom = d00 * d11 - d01 * d01;

            alpha = (d11 * d20 - d01 * d21) / denom;
            beta = (d00 * d21 - d01 * d20) / denom;
            gamma = 1.0 - alpha - beta;
        }

        void initialiseBoundingBox() {
            auto v1 = vertices[0];
            auto v2 = vertices[1];
            auto v3 = vertices[2];

            auto minX = std::min(v1.x(), std::min(v2.x(), v3.x())); // prompt was std::min(v1.x(), rest auto generated based on pattern
            auto minY = std::min(v1.y(), std::min(v2.y(), v3.y()));
            auto minZ = std::min(v1.z(), std::min(v2.z(), v3.z()));

            auto maxX = std::max(v1.x(), std::max(v2.x(), v3.x()));
            auto maxY = std::max(v1.y(), std::max(v2.y(), v3.y()));
            auto maxZ = std::max(v1.z(), std::max(v2.z(), v3.z()));

            boundingBox = BoundingBox(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
        }
    public:
        triangle() {}
        triangle(const Point3& v1, const Point3& v2, const Point3& v3)
            : vertices{v1, v2, v3}
        {
            initialiseBoundingBox();
        }
        triangle(const Point3& v1, const Point3& v2, const Point3& v3, const Material& mat)
            : vertices{v1, v2, v3}, mat(mat)
        {
            initialiseBoundingBox();
        }

        // get function for vertices
        Point3 get_vertex(int i) const { return vertices[i]; }

        Material getMaterial() const override {return mat;}

        // stolen off wikipedia's page on the MT algorithm, adapted to fit the vec3 and ray implementation
        double intersection(const Ray&r) const override {
            const float EPSILON = 0.000001;
            Vec3 vertex0 = vertices[0];
            Vec3 vertex1 = vertices[1];  
            Vec3 vertex2 = vertices[2];
            Vec3 edge1, edge2, h, s, q;
            double a, f, u, v;
            edge1 = vertex1 - vertex0;
            edge2 = vertex2 - vertex0;

            h = cross(r.direction(), edge2);
            a = dot(edge1, h);

            if (a > -EPSILON && a < EPSILON)
                return -1;    // This ray is parallel to this triangle.

            f = 1.0 / a;
            s = r.origin() - vertex0;
            u = f * dot(s,h);

            if (u < 0.0 || u > 1.0)
                return -1;

            q = cross(s, edge1);
            v = f * dot(r.direction(), q);

            if (v < 0.0 || u + v > 1.0)
                return -1;

            // At this stage we can compute t to find out where the intersection point is on the line.
            double t = f * dot(edge2, q);

            if (t > EPSILON) // ray intersection
            {
                return t;
            }
            else // This means that there is a line intersection but not a ray intersection.
                return -1;
        }

        // COPILOT BABEY
        Vec3 getNormal(Point3 point) const override {
            Vec3 v1_minus_v0 = vertices[1] - vertices[0];
            Vec3 v2_minus_v0 = vertices[2] - vertices[0];
            return unit_vector(cross(v1_minus_v0, v2_minus_v0));
        }

        Color getDiffuseColor(Point3 point) const override {
            if (!mat.getHasTexture()) {
                return mat.getDiffuseColor();
            }
            else {
                // calculate barycentric coordinates for point
                double alpha, beta, gamma;
                get_barycentric_coordinates(alpha, beta, gamma, point);
                
                auto u1 = 0.0;
                auto v1 = 0.0;

                auto u2 = 0.0;
                auto v2 = 1.0;

                auto u3 = 1.0;
                auto v3 = 0.0;

                auto u = alpha*u1 + beta*u2 + gamma*u3;
                auto v = alpha*v1 + beta*v2 + gamma*v3;

                return mat.getTexture().getColorAtPixel(u, v);
            }
        }

        void getType() const override {
            std::cerr << "triangle" << std::endl;
        }
}; 

#endif