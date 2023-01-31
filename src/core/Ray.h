#ifndef INCLUDED_RAY_H
#define INCLUDED_RAY_H

#include <iostream>
#include <limits>

#include <glm/vec3.hpp>

// core/geometry.h
// TODO: finish documentation, add unit test

/// @class Ray
/// @brief Ray Declaration
class Ray
{
public:
    /// @brief Default Ray constructor.
    Ray() : m_origin(), m_direction(), m_tMin(0.f), m_tMax(std::numeric_limits<float>::infinity()) {}

    /// @brief Ray constructor
    /// @param origin ray origin
    /// @param direction ray direction
    /// @param tMin minimum t-value to count as a hit
    /// @param tMax maximum t-value to count as a hit
    Ray(const glm::vec3 &origin,
        const glm::vec3 &direction,
        float tMin = 0.f,
        float tMax = std::numeric_limits<float>::infinity()) : m_origin(origin),
                                                               m_direction(direction),
                                                               m_tMin(tMin),
                                                               m_tMax(tMax) {}

    glm::vec3 operator()(const float t) const { return m_origin + m_direction * t; }

    friend std::ostream &operator<<(std::ostream &os, const Ray &r)
    {
        os << "[o=" << r.m_origin.x << "," << r.m_origin.y << "," << r.m_origin.z
           << ", d=" << r.m_direction.x << "," << r.m_direction.y << "," << r.m_direction.z
           << ", tMin=" << r.m_tMin << ", " << "tMax=" << r.m_tMax << "]";
        return os;
    }

    // Ray Public Data
    glm::vec3 m_origin;
    glm::vec3 m_direction;
    mutable float m_tMin;   // abs(near) / ray.m_direction.z
    mutable float m_tMax;   // abs(zFar) / ray.m_direction.z
};

#endif  // INCLUDED_RAY_H