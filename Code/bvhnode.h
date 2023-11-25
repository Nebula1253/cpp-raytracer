#ifndef BVHNODE_H
#define BVHNODE_H

#include "shape.h"
#include "boundingbox.h"
#include <algorithm>

class BVHNode {
public:
    BoundingBox box;
    BVHNode* left;
    BVHNode* right;
    Shape* shape;

    BVHNode() {
        left = right = nullptr;
        shape = nullptr;
    }
    BVHNode(std::vector<Shape*> shapes, int start, int end) {
        // Choose axis to sort on
        int axis = 2;

        // Sort shapes on selected axis
        auto comparator = [axis](Shape* a, Shape* b) {
            return a->getBoundingBox().leftBottomBack.e[axis] < b->getBoundingBox().leftBottomBack.e[axis];
        };
        std::sort(shapes.begin() + start, shapes.begin() + end, comparator);

        int size = end - start;
        int mid = start + size / 2;
        if (size == 1) {
            // Leaf node
            left = right = nullptr;
            shape = shapes[start];
            box = shape->getBoundingBox();
        } else {
            // Internal node
            shape = nullptr;
            left = new BVHNode(shapes, start, mid);
            right = new BVHNode(shapes, mid, end);
            box = BoundingBox(left->box, right->box);
        }
    }

    double hit(const Ray& r, Shape* hitShape) const {
        if (!box.hit(r)) {
            return -1;
        }

        if (shape) {
            hitShape = shape;
            return shape->intersection(r);
        }

        double leftSubtreeHit = left->hit(r, hitShape);
        double rightSubtreeHit = right->hit(r, hitShape);
        // std::cerr << "left subtree hit: " << leftSubtreeHit << std::endl;
        // std::cerr << "right subtree hit: " << rightSubtreeHit << std::endl;

        if (leftSubtreeHit == -1) {
            return rightSubtreeHit;
        } else if (rightSubtreeHit == -1) {
            return leftSubtreeHit;
        } else {
            return std::min(leftSubtreeHit, rightSubtreeHit);
        }
    }
};

#endif