#ifndef INCLUDED_AABB_H
#define INCLUDED_AABB_H

#include "Ray.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/// @class AxisAlignedBoundingBox
/// @brief Axis aligned bounding box
///
/// Axis aligned bounding box where the box edges are mutually perpendicular and aligned with
/// the coordinate system axes.
class AxisAlignedBoundingBox
{
public:
    /// Default constructor
    AxisAlignedBoundingBox();

    /// @brief Constructor
    ///
    /// @param p1 Minimum point of the bounding box
    /// @param p2 Maximum point of the bounding box
    AxisAlignedBoundingBox(const glm::vec3 &p1, const glm::vec3 &p2);

    /// Default destructor
    ~AxisAlignedBoundingBox() = default;

    /// @brief Get one corner of the bounding box
    /// @param  c the corner index which can be one of eight values ranging from 0 to 7.
    /// @return the coordinates of one of the eight corners of the bounding box.
    /// @throws std::runtime_error if the corner index is not in the range of 0 to 7 inclusive.
    glm::vec3 corner(const int c) const;

    /// @brief Compute the surface area of the six faces of the bounding box.
    /// @return the surface area of the bounding box
    float surfaceArea() const;

    /// @brief Compute the volume of the bounding box.
    /// @return the volume of this bounding box
    float volume() const;

    /// @brief Uses the slab test to determine if a ray intersects the bounding box.
    /// @param ray the ray to test for intersection
    /// @return true if the ray intersects this bounding box, otherwise false
    bool intersect(const Ray &ray);

    /// @brief Get the maximum extent of the bounding box
    /// @return the maximum extent
    int maxExtent() const;

    /// @brief Construct a new bounding box that bounds the space encompassed by the two bounding boxes.
    /// @param box1 the first bounding box
    /// @param box2 the second bounding box
    /// @return  A new bounding box bounding the space encompassed by box1 and box2
    static AxisAlignedBoundingBox combine(const AxisAlignedBoundingBox &box1, const AxisAlignedBoundingBox &box2);

    /// @brief Construct a new bounding box that bounds the space encompassed by the bounding box
    ///        and the point.
    /// @param box the bounding box
    /// @param point the point
    /// @return  A new bounding box bounding the space encompassed by box and point
    static AxisAlignedBoundingBox combine(const AxisAlignedBoundingBox &box, const glm::vec3 &point);

    friend std::ostream &operator<<(std::ostream &os, const AxisAlignedBoundingBox &b)
    {
        os << "[ (" << b.m_pMin.x << "," << b.m_pMin.y << "," << b.m_pMin.z << ")"
           << " - (" << b.m_pMax.x << "," << b.m_pMax.y << "," << b.m_pMax.z << ") ]";

        return os;
    }

    /// @brief minimum point of the bounding box
    glm::vec3 m_pMin;
    /// @brief maximum point of the bounding box
    glm::vec3 m_pMax;
};

#endif
