#ifndef INCLUDED_PROJECTION_CAMERA_H
#define INCLUDED_PROJECTION_CAMERA_H

#include "Camera.h"
#include "Ray.h"

/// @class ProjectionCamera
/// @brief Abstract projection camera base class
///
/// The abstract base class holds camera options and defines the interface that all projection
/// camera implementations must provide.
class ProjectionCamera : public Camera
{
public:
    ProjectionCamera() = default;
    virtual ~ProjectionCamera() = default;

    /// Creates a ray in camera space from a screen pixel location. Caller is responsible or
    /// managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return camera ray
    virtual Ray *generateRay(const glm::vec2 &pixel) = 0;

    //@{
    /// Change the zoom level. For a perspective camera this will change the view angle by
    /// the specified factor. For an orthographic camera this will change the parallel scale
    /// by the specified factor.
    /// @param factor A value greater than 1 is a zoom-in, a value less than 1 is a zoom-out.
    virtual void zoom(const float factor) = 0;
    virtual float getZoomFactor() const = 0;
    //@}

    //@{
    /// Set the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    virtual void setViewAngle(const float angle) = 0;
    virtual float getViewAngle() const = 0;
    //@}

    //@{
    /// Set/get the location of the near and far clipping planes along the forward axis direction.
    /// Both of these values must be positive. The default clipping range is (0.1,1000) and is
    /// measured in world coordinates.
    /// @param  near the near clipping plane along the forward axis
    /// @param  far  the far clipping plane along the forward axis
    virtual void setClippingRange(const float near, const float far) = 0;
    virtual glm::vec2 getClippingRange() const = 0;
    //@}

    //@{
    /// Update the screen size.
    /// @param width screen width
    /// @param height screen height
    virtual void setScreenSize(const int width, const int height) = 0;
    virtual glm::vec2 getScreenSize() const = 0;
    //@}

    /// Get the projection matrix
    /// @return the projection matrix
    virtual glm::mat4 getProjectionMatrix() const = 0;

    virtual void copy(ProjectionCamera * const camera) = 0;
};

#endif