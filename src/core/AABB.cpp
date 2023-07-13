#include "AABB.h"

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : m_pMin(glm::vec3(0.f)),
      m_pMax(glm::vec3(0.f))
{}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3 &p1, const glm::vec3 &p2)
    : m_pMin(glm::vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z))),
      m_pMax(glm::vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)))
{}

//----------------------------------------------------------------------------------
glm::vec3 AxisAlignedBoundingBox::corner(const int c) const
{
    if(c >= 8 || c < 0)
    {
        std::string msg = "Invalid corner index (";
        msg += c;
        msg += "). Expecting index between 0-7 inclusive.";
        throw std::runtime_error(msg);
    }

    float x = (c & 1) ? m_pMax.x : m_pMin.x;
    float y = (c & 2) ? m_pMax.y : m_pMin.y;
    float z = (c & 4) ? m_pMax.z : m_pMin.z;

    return glm::vec3(x,y,z);
}

//----------------------------------------------------------------------------------
bool AxisAlignedBoundingBox::intersect(Ray * const ray)
{
    glm::vec3 invRayDir = glm::vec3(1.f / ray->m_direction.x,
                                    1.f / ray->m_direction.y,
                                    1.f / ray->m_direction.z);

    // Absolute distance to lower and upper box coordinates
    glm::vec3 tLower = (m_pMin - ray->m_origin) * invRayDir;
    glm::vec3 tUpper = (m_pMax - ray->m_origin) * invRayDir;

    // The four t-intervals (for x-/y-/z-slabs, and ray p(t))
    glm::vec4 tMins = glm::vec4(glm::min(tLower, tUpper), ray->m_tMin);
    glm::vec4 tMaxes = glm::vec4(glm::max(tLower, tUpper), ray->m_tMax);

    float tBoxMin = std::max(std::max(std::max(tMins[0], tMins[1]), tMins[2]), tMins[3]);
    float tBoxMax = std::min(std::min(std::min(tMaxes[0], tMaxes[1]), tMaxes[2]), tMaxes[3]);

    return tBoxMin <= tBoxMax;
}