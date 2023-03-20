#include "OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::ortho

//----------------------------------------------------------------------------------
OrthographicCamera::OrthographicCamera(int width,
                                      int height,
                                      float fovDistance,
                                      float near,
                                      float far) :
    m_width(width),
    m_height(height),
    m_fovDistance(fovDistance),
    m_near(near),
    m_far(far)
{
    m_orthographicMatrix = glm::ortho(0.f, static_cast<float>(width),
                                      static_cast<float>(height), 0.f,
                                      near, far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setClippingRange(const float near, const float far)
{
    m_near = near;
    m_far = far;

    m_orthographicMatrix = glm::ortho(0.f, static_cast<float>(m_width),
                                      static_cast<float>(m_height), 0.f,
                                      near, far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setScreenSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    m_orthographicMatrix = glm::ortho(0.f, static_cast<float>(width),
                                      static_cast<float>(height), 0.f,
                                      m_near, m_far);
}

//----------------------------------------------------------------------------------
Ray *
OrthographicCamera::generateRay(const glm::vec2 &pixel)
{
    auto p = pixel * m_fovDistance;
    auto origin = glm::vec3(p, 0.f);

    Ray *ray = new Ray(origin,
                       glm::vec3(0.f, 0.f, -1.f),
                       std::abs(m_near),
                       std::abs(m_far));

    return ray;
}

//----------------------------------------------------------------------------------
Ray *
OrthographicCamera::generateWorldRay(const glm::vec2 &pixel)
{
    Ray *ray = this->generateRay(pixel);
    glm::mat4 cameraToWorldTransform = this->getCameraToWorldMatrix();

    ray->m_direction = glm::mat3(cameraToWorldTransform) * ray->m_direction;
    ray->m_origin = glm::vec3(cameraToWorldTransform[3]) + ray->m_origin;

    return ray;
}