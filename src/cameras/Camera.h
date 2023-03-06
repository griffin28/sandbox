#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include "Ray.h"
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    /// @brief Create a camera ray in camera space.
    /// @param pixel the x- and y- pixel coordinate in raster space
    /// @return camera ray
    virtual Ray *generateRay(const glm::vec2 &pixel) = 0;

    //@{
    // Camera Rotation

    /// @brief A rotation of the camera about its forward axis.
    /// @param angle rotation angle in degrees
    void roll(const float angle);

    /// @brief Rotate the camera about the horizontal axis.
    /// @param angle rotation angle in degrees
    void tilt(const float angle);

    /// @brief Rotate the camera about its vertical axis.
    /// @param angle rotation angle in degrees
    void pan(const float angle);
    //@}

    //@{
    // Camera Translation

    /// @brief Translation of the camera along its forward axis
    /// @param value the amount to mvoe the camera
    void dolly(const float value);

    /// @brief Translation of the camera along its vertical axis.
    /// @param value
    void boom(const float value);
    //@}

    //@{
    /// @brief Set/Get the position of the camera in world coordinates.
    /// @param position position of the camera
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const { return m_position; }
    //@}

    //@{
    /// @brief Set/Get the focal of the camera in world coordinates. The default
    ///        focal point is the origin.
    /// @param focalPoint
    void setFocalPoint(const glm::vec3 &focalPoint);
    glm::vec3 getFocalPoint() const { return m_focalPoint; }
    //@}

    void setViewUp(const glm::vec3 &up);
    glm::vec3 getViewUp() const { return m_viewUp; }

    /// @brief The viewing transformation for moving objects from world space to view space.
    /// @return camera to world transformation matrix
    glm::mat4 getViewTransform() { return  m_viewMatrix * m_modelMatrix; }

private:
    glm::vec3 m_position;
    glm::vec3 m_focalPoint;
    glm::vec3 m_viewUp;

    glm::mat4 m_modelMatrix;
    glm::mat4 m_viewMatrix;
};

#endif