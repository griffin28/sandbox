#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(const int width, const int height) :
    m_width(width),
    m_height(height),
    m_fovy(30.0f),
    m_near(0.1f),
    m_far(1000.f),
    m_perspectiveMatrix(glm::mat4(1.0f))
{ }

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
                       clippingRange[0] / direction.z,
                       clippingRange[1] / direction.z);

    return ray;
}

Ray *
PerspectiveCamera::generateWorldRay(const glm::vec2 &pixel)
{
    Ray *ray = this->generateRay(pixel);

    // ray.m_direction = matmul(rotation, ray.m_direction);
    // ray.m_origin = translation + ray.m_origin;
}