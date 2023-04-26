#ifndef INCLUDED_ORTHOGRAPHIC_CAMERA_H
#define INCLUDED_ORTHOGRAPHIC_CAMERA_H

#include "ProjectionCamera.h"

/// @class OrthographicCamera
/// @brief Represents the orthographic camera for parallel projections
///
/// TODO: details from book
/// The orthographic camera is commonly used in engineering. All objects appear at the same scale,
/// parallel lines remain parallel and a unit of length will appear to have the same length
/// everywhere in the rendering. This makes it easier to judge relative sizes and to align models.
///
class OrthographicCamera : public ProjectionCamera
{
public:
    /// @brief Constructor
    /// @param width screen width
    /// @param height screen height
    /// @param fovDistance scalar field of view in meters
    /// @param near the near clipping plane along the forward axis
    /// @param far the far clipping plane along the forward axis
    OrthographicCamera(int width,
                       int height,
                       float fovDistance=100.f,
                       float near=0.1f,
                       float far=1000.f);

    /// Default destructor.
    ~OrthographicCamera() = default;

    /// @see Camera::reset
    void reset() override;

    /// @brief Creates a ray for an orthographic projection in camera space from a screen pixel
    ///        location. Caller is responsible for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    Ray *generateRay(const glm::vec2 &pixel) override;

    /// @brief Creates a ray for an orthographic projection in world space from a screen pixel
    ///        location. Caller is responsible for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    Ray *generateWorldRay(const glm::vec2 &pixel) override;

    /// @brief translate in the z-direction
    /// @param factor
    void zoom(const float factor) override;
    float getZoomFactor() const override { return m_zoomFactor; }

    /// @brief Set the camera view angle. This will have no effect for an orthographic camera as
    ///        the effective view angle is always zero degrees.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const float angle) override { }
    float getViewAngle() const override { return 0.0f; }

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near the near clipping plane along the forward axis
    /// @param  far  the far clipping plane along the forward axis
    void setClippingRange(const float near, const float far) override;
    glm::vec2 getClippingRange() const override { return glm::vec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height) override;
    glm::vec2 getScreenSize() const override { return glm::vec2(m_width, m_height); }

    /// @brief Get the parallel projection matrix.
    /// @return the parallel projection matrix
    glm::mat4 getProjectionMatrix() const override { return m_orthographicMatrix; }

    //@{
    /// @brief Set/get the scalar field of view distance in meters?.
    /// @param  distance scalar field of view in meters
    void setFOVDistance(const float distance) { m_fovDistance = distance; }
    float getFOVDistance() const { return m_fovDistance; }
    //@}

    void copy(ProjectionCamera * const camera) override;

private:
    int m_width;
    int m_height;
    float m_scale;
    float m_zoomFactor;

    float m_fovDistance;
    float m_near;
    float m_far;

    glm::mat4 m_orthographicMatrix;
};

#endif