#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"

class triangle : public shape {
    private:
        point3 vertices[3];
        material mat;
    public:
        triangle() {}
        triangle(const point3& v1, const point3& v2, const point3& v3)
            : vertices{v1, v2, v3}
        {}
        triangle(const point3& v1, const point3& v2, const point3& v3, const material& mat)
            : vertices{v1, v2, v3}, mat(mat)
        {}

        // get function for vertices
        // point3* get_vertices() const { return vertices; }
        point3 get_vertex(int i) const { return vertices[i]; }

        bool intersection_neel(const ray& r) const {
            const float EPSILON = 0.000001;
            vec3 v1_minus_v0 = vertices[1] - vertices[0];
            vec3 v2_minus_v0 = vertices[2] - vertices[0];

            vec3 plane_normal = unit_vector(cross(v1_minus_v0, v2_minus_v0));
            vec3 dir = unit_vector(r.direction());
            if (dot(plane_normal, dir) > -EPSILON && dot(plane_normal, dir) < EPSILON) {
                return false; // the ray is parallel to the plane of the triangle, so how the hell would it ever intersect
            }
            // else std::cerr << "plane normal: " << plane_normal << "\n";

            // Cramer's rule: the solution to the system of equations Ax = b, where A is a 3x3 matrix, and both x and b are 3-vectors,
            // can be written as x = (det(A1)/det(A), det(A2)/det(A), det(A3)/det(A)), where A1, A2, and A3 are the matrices formed by
            // replacing the first, second, and third columns of A with b, respectively.
            // Here, A is the matrix [-dir, v1_minus_v0, v2_minus_v0], x is the vector [t,u,v] and b is the vector O_minus_v1.
            auto O_minus_v1 = r.origin() - vertices[0];

            // the scalar product is defined as dot(a, cross(b, c)) - can be seen as the determinant of the matrix [a, b, c]
            auto det_a = dot(-dir, cross(v1_minus_v0, v2_minus_v0));
            auto det_a1 = dot(O_minus_v1, cross(v1_minus_v0, v2_minus_v0));
            auto det_a2 = dot(-dir, cross(O_minus_v1, v2_minus_v0));
            auto det_a3 = dot(-dir, cross(v2_minus_v0, O_minus_v1));

            auto t = det_a1 / det_a;
            auto u = det_a2 / det_a;
            auto v = det_a3 / det_a;

            // print t,u,v to std::cerr
            // std::cerr << "t: " << t << " u: " << u << " v: " << v << "\n";

            return !(u < 0 || v < 0 || u+v > 1 || t <= EPSILON);
        }

        material get_material() const override {return mat;}

        // stolen off wikipedia's page on the MT algorithm, adapted to fit the vec3 and ray implementation
        double intersection(const ray&r) const override {
            // std::cerr << "THIS IS THE TRIANGLE INTERSECTION" << std::endl;
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
                return -1;    // This ray is parallel to this triangle.

            f = 1.0 / a;
            s = r.origin() - vertex0;
            // u = f * s.dotProduct(h);
            u = f * dot(s,h);

            if (u < 0.0 || u > 1.0)
                return -1;

            // q = s.crossProduct(edge1);
            q = cross(s, edge1);
            // v = f * rayVector.dotProduct(q);
            v = f * dot(r.direction(), q);

            if (v < 0.0 || u + v > 1.0)
                return -1;

            // At this stage we can compute t to find out where the intersection point is on the line.
            // float t = f * edge2.dotProduct(q);
            double t = f * dot(edge2, q);

            if (t > EPSILON) // ray intersection
            {
                // outIntersectionPoint = rayOrigin + rayVector * t;
                // std::cerr << "YAY! t: " << t << " u: " << u << " v: " << v << "\n";
                return t;
            }
            else // This means that there is a line intersection but not a ray intersection.
                // std::cerr << "t: " << t << " u: " << u << " v: " << v << "\n";
                return -1;
        }

        // COPILOT BABEY
        vec3 get_normal(point3 point) const override {
            vec3 v1_minus_v0 = vertices[1] - vertices[0];
            vec3 v2_minus_v0 = vertices[2] - vertices[0];
            return unit_vector(cross(v1_minus_v0, v2_minus_v0));
        }
}; 

#endif