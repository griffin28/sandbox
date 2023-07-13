#ifndef INCLUDED_PERSPECTIVE_CAMERA_H
#define INCLUDED_PERSPECTIVE_CAMERA_H

#include "ProjectionCamera.h"

/// @class PerspectiveCamera
/// @brief Represents a projection camera for perspective projections
///
/// The perspective camera projects a volume of space onto a 2D plane. It also uses the effect
/// of foreshortening where objects that are far away are projected to be smaller than objects
/// of the same size that are closer. The perspective projection doesn't preserve distances or
/// angles and parallel lines no longer remain parallel. The perspective projection is a reasonably
/// close match to how an eye or camera lens generates images of the 3D world.
class PerspectiveCamera : public ProjectionCamera
{
public:
    /// @brief Constructor
    /// @param width screen width
    /// @param height screen height
    /// @param fovy the view angle in degrees
    /// @param near the distance to the near clipping plane along the forward axis
    /// @param far the distance to the far clipping plane along the forward axis
    PerspectiveCamera(int width,
                      int height,
                      float fovy=45.f,
                      float near=0.1f,
                      float far=1000.f);

    /// Default destructor.
    ~PerspectiveCamera() = default;

    /// @see Camera::reset
    void reset() override;

    /// @brief Creates a ray in camera space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    Ray *generateRay(const glm::vec2 &pixel) override;

    /// @brief Creates a ray in world space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    Ray *generateWorldRay(const glm::vec2 &pixel) override;

    /// @brief Change the view angle by the specified factor
    /// @param factor
    void zoom(const float factor) override;
    float getZoomFactor() const override { return m_zoomFactor; }

    //@{
    /// @brief Set/get the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const float angle) override;
    float getViewAngle() const override { return m_fovy; }
    //@}

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near distance to the near clipping plane along the forward axis
    /// @param  far  distance to the far clipping plane along the forward axis
    void setClippingRange(const float near, const float far) override;
    glm::vec2 getClippingRange() const override { return glm::vec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height) override;
    glm::vec2 getScreenSize() const override { return glm::vec2(m_width, m_height); }

    /// @brief Get the projection matrix in the form of a frustrum matrix such that clip-space
    ///        takes the shape of a truncated rectangular pyramid.
    /// @return the projection matrix
    glm::mat4 getProjectionMatrix() const override { return m_perspectiveMatrix; }

    void copy(ProjectionCamera * const camera) override;

private:
    int m_width;
    int m_height;
    float m_zoomFactor;

    float m_fovy;
    float m_near;
    float m_far;

    glm::mat4 m_perspectiveMatrix;
};

#endif