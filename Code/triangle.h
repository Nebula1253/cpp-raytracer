#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"

class triangle : public shape {
    private:
        point3 vertices[3];
    public:
        triangle() {}
        triangle(const point3& v1, const point3& v2, const point3& v3)
            : vertices{v1, v2, v3}
        {}

        // get function for vertices
        // point3* get_vertices() const { return vertices; }
        point3 get_vertex(int i) const { return vertices[i]; }

        // TODO: intersection
        bool intersection(const ray& r) const {
            vec3 edge1 = vertices[1] - vertices[0];
            vec3 edge2 = vertices[2] - vertices[1];

            vec3 plane_normal = cross(edge1, edge2);
            if (dot(plane_normal, r.direction())) {
                return false; // the ray is parallel to the plane of the triangle, so how the hell would it ever intersect
            }

            auto T = r.origin() - vertices[0];
            auto e2 = vertices[2] - vertices[1];

            auto scalar = 1 / dot(cross(r.direction(), e2), edge1);
            vec3 vector = vec3(dot(cross(T, edge1), e2), dot(cross(r.direction(), e2), T), dot(cross(T, edge1), r.direction()));

            vec3 tuv = (1/scalar) * vector;
            double t = tuv.x();
            double u = tuv.y();
            double v = tuv.z();

            return !(u < 0 || u > 1 || v < 0 || u+v > 1 || t <= 0);

            // if (tuv.x() > 0) return true;
            // else return false;
        }
}; 

#endif