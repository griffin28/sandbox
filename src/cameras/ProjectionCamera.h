#ifndef INCLUDED_PROJECTION_CAMERA_H
#define INCLUDED_PROJECTION_CAMERA_H

#include "Camera.h"
#include "Ray.h"

class ProjectionCamera : public Camera
{
public:
    /// @brief Constructor
    ProjectionCamera() = default;

    /// Default destructor.
    virtual ~ProjectionCamera() = default;

    /// @brief Creates a ray in camera space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return camera ray
    virtual Ray *generateRay(const glm::vec2 &pixel) = 0;

    /// @brief Creates a ray in world space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    virtual Ray *generateWorldRay(const glm::vec2 &pixel) = 0;

    /// @brief Change the view angle by the specified factor
    /// @param factor
    virtual void zoom(const float factor) = 0;

    /// @brief Set the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    virtual void setViewAngle(const float angle) = 0;

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near the near clipping plane along the forward axis
    /// @param  far  the far clipping plane along the forward axis
    virtual void setClippingRange(const float near, const float far) = 0;
    virtual glm::vec2 getClippingRange() = 0;
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    virtual void setScreenSize(const int width, const int height) = 0;

    /// @brief Get the projection matrix
    /// @return the projection matrix
    virtual glm::mat4 getProjectionMatrix() const = 0;
};

#endif