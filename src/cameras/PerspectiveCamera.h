#ifndef INCLUDED_PERSPECTIVE_CAMERA_H
#define INCLUDED_PERSPECTIVE_CAMERA_H

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
    /// @brief Constructor
    /// @param width screen width
    /// @param height screen height
    PerspectiveCamera(const int width, const int height);

    /// Default destructor.
    ~PerspectiveCamera() = default;

    virtual Ray *generateRay(const glm::vec2 &pixel) override;

    Ray *generateWorldRay(const glm::vec2 &pixel);

    /// @brief Decrease the view angle by the specified factor
    /// @param factor
    void zoom(const float factor) { setViewAngle(m_fovy / factor); }

    /// @brief Set the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const float angle);

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near the near clipping plane along the forward axis
    /// @param  far  the far clipping plane along the forward axis
    void setClippingRange(const float near, const float far);
    glm::vec2 getClippingRange() { return glm::vec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height);

    /// @brief Get the perspective matrix in the form of a frustrum matrix such that clip-space
    ///        takes the shape of a truncated rectangular pyramid.
    /// @return the projection matrix
    glm::mat4 getPerspectiveMatrix() const { return m_perspectiveMatrix; }

private:
    int m_width;
    int m_height;

    float m_fovy;
    float m_near;
    float m_far;

    glm::mat4 m_perspectiveMatrix;
};

#endif