#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "vec3.h"

class triangle : public Shape {
    private:
        Point3 vertices[3];
        Material mat;

        void get_barycentric_coordinates (double& alpha, double& beta, double& gamma, const Point3& point) const{
            // auto x1 = vertices[0].x();
            // auto y1 = vertices[0].y();
            // auto z1 = vertices[0].z();

            // auto x2 = vertices[1].x();
            // auto y2 = vertices[1].y();
            // auto z2 = vertices[1].z();

            // auto x3 = vertices[2].x();
            // auto y3 = vertices[2].y();
            // auto z3 = vertices[2].z();

            // auto x = point.x();
            // auto y = point.y();

            // auto denom = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);

            // alpha = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denom;
            // beta = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denom;
            // gamma = 1.0f - alpha - beta;

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
    public:
        triangle() {}
        triangle(const Point3& v1, const Point3& v2, const Point3& v3)
            : vertices{v1, v2, v3}
        {
            auto minX = std::min(v1.x(), std::min(v2.x(), v3.x())); // prompt was std::min(v1.x(), rest auto generated based on pattern
            auto minY = std::min(v1.y(), std::min(v2.y(), v3.y()));
            auto minZ = std::min(v1.z(), std::min(v2.z(), v3.z()));

            auto maxX = std::max(v1.x(), std::max(v2.x(), v3.x()));
            auto maxY = std::max(v1.y(), std::max(v2.y(), v3.y()));
            auto maxZ = std::max(v1.z(), std::max(v2.z(), v3.z()));

            boundingBox = BoundingBox(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
        }
        triangle(const Point3& v1, const Point3& v2, const Point3& v3, const Material& mat)
            : vertices{v1, v2, v3}, mat(mat)
        {
            auto minX = std::min(v1.x(), std::min(v2.x(), v3.x())); // prompt was std::min(v1.x(), rest auto generated based on pattern
            auto minY = std::min(v1.y(), std::min(v2.y(), v3.y()));
            auto minZ = std::min(v1.z(), std::min(v2.z(), v3.z()));

            auto maxX = std::max(v1.x(), std::max(v2.x(), v3.x()));
            auto maxY = std::max(v1.y(), std::max(v2.y(), v3.y()));
            auto maxZ = std::max(v1.z(), std::max(v2.z(), v3.z()));

            boundingBox = BoundingBox(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
        }

        // get function for vertices
        // point3* get_vertices() const { return vertices; }
        Point3 get_vertex(int i) const { return vertices[i]; }

        bool intersection_neel(const Ray& r) const {
            const float EPSILON = 0.000001;
            Vec3 v1_minus_v0 = vertices[1] - vertices[0];
            Vec3 v2_minus_v0 = vertices[2] - vertices[0];

            Vec3 plane_normal = unit_vector(cross(v1_minus_v0, v2_minus_v0));
            Vec3 dir = unit_vector(r.direction());
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

        Material getMaterial() const override {return mat;}

        // stolen off wikipedia's page on the MT algorithm, adapted to fit the vec3 and ray implementation
        double intersection(const Ray&r) const override {
            // std::cerr << "THIS IS THE TRIANGLE INTERSECTION" << std::endl;
            const float EPSILON = 0.000001;
            Vec3 vertex0 = vertices[0];
            Vec3 vertex1 = vertices[1];  
            Vec3 vertex2 = vertices[2];
            Vec3 edge1, edge2, h, s, q;
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
                
                // std::cerr << "alpha: " << alpha << " beta: " << beta << " gamma: " << gamma << "\n";

                auto u1 = 0.0;
                auto v1 = 0.0;

                auto u2 = 0.0;
                auto v2 = 1.0;

                auto u3 = 1.0;
                auto v3 = 0.0;

                auto u = alpha*u1 + beta*u2 + gamma*u3;
                auto v = alpha*v1 + beta*v2 + gamma*v3;
                // std::cerr << "u: " << u << " v: " << v << "\n";

                return mat.getTexture().getColorAtPixel(u, v);
            }
        }
}; 

#endif