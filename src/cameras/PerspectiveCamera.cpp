#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(int width,
                                     int height,
                                     float fovy,
                                     float near,
                                     float far) :
    m_width(width),
    m_height(height),
    m_fovy(fovy),
    m_near(near),
    m_far(far)
{
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setViewAngle(const float angle)
{
    m_fovy = angle;
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setClippingRange(const float near, const float far)
{
    m_near = near;
    m_far = far;
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setScreenSize(const int width, const int height)
{
    m_width = width;
    m_height = height;
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateRay(const glm::vec2 &pixel)
{
    float tanHalfAngle = glm::tan(m_fovy / 2.f);
    glm::vec3 direction = glm::normalize(glm::vec3(glm::vec2(pixel.x, -pixel.y) * (tanHalfAngle / m_height), -1));
    glm::vec2 clippingRange = this->getClippingRange();

    Ray *ray = new Ray(glm::vec3(0.f),
                       direction,
                       std::fabs(clippingRange[0] / direction.z),
                       std::fabs(clippingRange[1] / direction.z));

    return ray;
}

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateWorldRay(const glm::vec2 &pixel)
{
    Ray *ray = this->generateRay(pixel);
    glm::mat4 cameraToWorldTransform = this->getCameraToWorldTransform();

    ray->m_direction = glm::mat3(cameraToWorldTransform) * ray->m_direction;
    ray->m_origin = glm::vec3(cameraToWorldTransform[3]) + ray->m_origin;

    return ray;
}