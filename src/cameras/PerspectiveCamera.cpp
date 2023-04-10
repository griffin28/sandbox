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
    m_zoomFactor(1.0f),
    m_fovy(fovy),
    m_near(near),
    m_far(far)
{
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::reset()
{
    // TODO: save original values to reset to previous values
    this->Camera::reset();

    m_fovy = 45.0f;
    m_near = 0.1f;
    m_far = 1000.0f;
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::zoom(const float factor)
{
    m_zoomFactor = factor;
    this->setViewAngle(m_fovy / factor);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setViewAngle(const float angle)
{
    if(m_fovy != angle)
    {
        float min = 0.00000001f;
        float max = 179.0f;

        m_fovy = (angle < min ? min : (angle > max ? max : angle));
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setClippingRange(const float near, const float far)
{
    if(m_near != near || m_far != far)
    {
        m_near = near;
        m_far = far;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setScreenSize(const int width, const int height)
{
    if(m_width != width || m_height != height)
    {
        m_width = width;
        m_height = height;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }

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
    glm::mat4 cameraToWorldTransform = this->getCameraToWorldMatrix();

    ray->m_direction = glm::mat3(cameraToWorldTransform) * ray->m_direction;
    ray->m_origin = glm::vec3(cameraToWorldTransform[3]) + ray->m_origin;

    return ray;
}

//----------------------------------------------------------------------------------
void
PerspectiveCamera::copy(ProjectionCamera * const camera)
{
    if(camera != nullptr)
    {
        // Projection Camera
        this->zoom(camera->getZoomFactor());

        glm::vec2 clippingRange = camera->getClippingRange();
        this->setClippingRange(clippingRange[0], clippingRange[1]);

        glm::vec2 screenSize = camera->getScreenSize();
        this->setScreenSize(screenSize.x, screenSize.y);

        // Camera
        this->setPosition(camera->getPosition());
        this->setFocalPoint(camera->getFocalPoint());
        this->setViewUp(camera->getViewUp());

        this->setCameraToWorldMatrix(camera->getCameraToWorldMatrix());
    }
    else
    {
        this->reset();
    }
}