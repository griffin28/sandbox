#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>	 // translate, rotate, scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

//----------------------------------------------------------------------------------
Camera::Camera() : m_position(glm::vec3(0.f)),
                   m_focalPoint(glm::vec3(0.f)),
                   m_viewUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                   m_modelMatrix(glm::mat4(1.0f)),
                   m_viewMatrix(glm::mat4(1.0f)) {}

//----------------------------------------------------------------------------------
void Camera::roll(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                glm::vec3(0.f,0.f,1.f));
}

//----------------------------------------------------------------------------------
void Camera::tilt(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                glm::vec3(1.f,0.f,0.f));
}

//----------------------------------------------------------------------------------
void Camera::pan(const float angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                glm::vec3(0.f,1.f,0.f));
}

//----------------------------------------------------------------------------------
void Camera::dolly(const float value)
{
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.f, 0.f, value));
}

//----------------------------------------------------------------------------------
void Camera::boom(const float value)
{
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.f, value, 0.f));
}

//----------------------------------------------------------------------------------
void Camera::setPosition(const glm::vec3 &position)
{
    m_position = position;
    m_viewMatrix = glm::lookAt(m_position, m_focalPoint, m_viewUp);
}

//----------------------------------------------------------------------------------
void Camera::setFocalPoint(const glm::vec3 &focalPoint)
{
    m_focalPoint = focalPoint;
    m_viewMatrix = glm::lookAt(m_position, m_focalPoint, m_viewUp);
}

//----------------------------------------------------------------------------------
void Camera::setViewUp(const glm::vec3 &up)
{
    m_viewUp = up;
    m_viewMatrix = glm::lookAt(m_position, m_focalPoint, m_viewUp);
}