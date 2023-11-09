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

        bool intersection_neel(const ray& r) const {
            vec3 edge1 = vertices[1] - vertices[0];
            vec3 edge2 = vertices[2] - vertices[0];

            vec3 plane_normal = -cross(edge1, edge2);
            vec3 dir = r.direction();
            if (dot(plane_normal, dir) == 0.0) {
                return false; // the ray is parallel to the plane of the triangle, so how the hell would it ever intersect
            }

            // the scalar product is defined as dot(a, cross(b, c)) - can be seen as the determinant of the matrix [a, b, c]
            auto O_minus_v1 = r.origin() - vertices[0];
            auto det_a = dot(dir, plane_normal);
            auto det_a1 = dot(O_minus_v1, plane_normal);
            auto det_a2 = dot(dir, cross(O_minus_v1, edge2));
            auto det_a3 = dot(dir, cross(edge2, O_minus_v1));

            auto t = det_a1 / det_a;
            auto u = det_a2 / det_a;
            auto v = det_a3 / det_a;

            // print t,u,v to std::cerr
            std::cerr << "t: " << t << " u: " << u << " v: " << v << "\n";

            return !(u < 0 || v < 0 || u+v > 1 || t <= 0);
        }

        // stolen off wikipedia's page on the MT algorithm
        bool intersection(const ray&r) const {
            const float EPSILON = 0.000001;
            vec3 vertex0 = vertices[0];
            vec3 vertex1 = vertices[1];  
            vec3 vertex2 = vertices[2];
            vec3 edge1, edge2, h, s, q;
            double a, f, u, v;
            edge1 = vertex1 - vertex0;
            edge2 = vertex2 - vertex0;

            // h = r.direction().crossProduct(edge2);
            h = cross(r.direction(), edge2);
            // a = edge1.dotProduct(h);
            a = dot(edge1, h);

            if (a > -EPSILON && a < EPSILON)
                return false;    // This ray is parallel to this triangle.

            f = 1.0 / a;
            s = r.origin() - vertex0;
            // u = f * s.dotProduct(h);
            u = f * dot(s,h);

            if (u < 0.0 || u > 1.0)
                return false;

            // q = s.crossProduct(edge1);
            q = cross(s, edge1);
            // v = f * rayVector.dotProduct(q);
            v = f * dot(r.direction(), q);

            if (v < 0.0 || u + v > 1.0)
                return false;

            // At this stage we can compute t to find out where the intersection point is on the line.
            // float t = f * edge2.dotProduct(q);
            double t = f * dot(edge2, q);

            if (t > EPSILON) // ray intersection
            {
                // outIntersectionPoint = rayOrigin + rayVector * t;
                std::cerr << "YAY! t: " << t << " u: " << u << " v: " << v << "\n";
                return true;
            }
            else // This means that there is a line intersection but not a ray intersection.
                std::cerr << "t: " << t << " u: " << u << " v: " << v << "\n";
                return false;
        }
}; 

#endif