#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>	 // translate, rotate, scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

//----------------------------------------------------------------------------------
Camera::Camera() : m_position(glm::vec3(0.f, 0.f, 1.f)),
                   m_focalPoint(glm::vec3(0.f, 0.f, 0.f)),
                   m_viewUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                   m_modelMatrix(glm::mat4(1.0f))
{
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::reset()
{
    m_position = glm::vec3(0.0f, 0.0f, 1.0f);
    m_focalPoint = glm::vec3(0.f, 0.f, 0.f);
    m_viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_modelMatrix = glm::mat4(1.0f);

    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::roll(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getForwardAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::tilt(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getHorizontalAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::pan(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getVerticalAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::dolly(const float value)
{
    float distance = glm::distance(m_focalPoint, m_position);
    float d = distance + value;
    d = (d <= 0.0f) ? 0.1f : d;

    auto forwardAxis = this->getForwardAxis();
    auto delta = forwardAxis * d;

    this->setPosition(m_focalPoint - delta);
}

//----------------------------------------------------------------------------------
void Camera::boom(const float value)
{
    auto verticalAxis = this->getVerticalAxis();
    auto delta = verticalAxis * value;

    this->setPosition(m_position - delta);
}

//----------------------------------------------------------------------------------
void Camera::setPosition(const glm::vec3 &position)
{
    m_position = position;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::setFocalPoint(const glm::vec3 &focalPoint)
{
    m_focalPoint = focalPoint;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::setViewUp(const glm::vec3 &up)
{
    m_viewUp = glm::normalize(up);
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
glm::vec3 Camera::getForwardAxis()
{
    return glm::normalize(m_focalPoint - m_position);
}

//----------------------------------------------------------------------------------
glm::vec3 Camera::getHorizontalAxis()
{
    auto forwardAxis = this->getForwardAxis();
    return glm::cross(forwardAxis, m_viewUp);
}

//----------------------------------------------------------------------------------
glm::vec3 Camera::getVerticalAxis()
{
    auto forwardAxis = this->getForwardAxis();
    auto horizontalAxis = glm::cross(m_viewUp, forwardAxis);
    return glm::cross(horizontalAxis, forwardAxis);
}

//----------------------------------------------------------------------------------
void Camera::setCameraToWorldMatrix(const glm::mat4 &matrix)
{
    m_modelMatrix = matrix;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::updateViewMatrix()
{
    auto position = m_modelMatrix * glm::vec4(m_position, 1.0f);
    auto viewUp = m_modelMatrix * glm::vec4(m_viewUp, 1.0f);

    auto pos = glm::vec3(position[0]/position[3],
                         position[1]/position[3],
                         position[2]/position[3]);

    auto up = glm::normalize(glm::vec3(viewUp[0]/viewUp[3],
                                       viewUp[1]/viewUp[3],
                                       viewUp[2]/viewUp[3]));

    m_viewMatrix = glm::lookAt(pos, m_focalPoint, up);
}