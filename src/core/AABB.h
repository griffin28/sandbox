#ifndef INCLUDED_AABB_H
#define INCLUDED_AABB_H

#include "Ray.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(const glm::vec3 &p1, const glm::vec3 &p2);
    ~AxisAlignedBoundingBox() = default;

    glm::vec3 corner(int);
    bool intersect(Ray * const);

    friend std::ostream &operator<<(std::ostream &os, const AxisAlignedBoundingBox &b)
    {
        os << "[ (" << b.pMin.x << "," << b.pMin.y << "," << b.pMin.z << ")"
           << " - (" << b.pMax.x << "," << b.pMax.y << "," << b.pMax.z << ") ]";

        return os;
    }

    glm::vec3 pMin;
    glm::vec3 pMax;
};

#endif
