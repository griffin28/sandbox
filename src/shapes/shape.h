#ifndef INCLUDED_SHAPE_H
#define INCLUDED_SHAPE_H

#include "model.h"
#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

// Scoped enumeration
enum class ShapeType
{
    SPHERE = 0,
    CUBOID,
    MESH
};

class Shape {
public:
    Shape();
    virtual ~Shape() = default;

    virtual AxisAlignedBoundingBox objectBounds() const = 0;
    virtual AxisAlignedBoundingBox worldBounds() const = 0;

    virtual bool intersect(const Ray &ray) const = 0;

    // TODO: Make pure virtual and implement in concrete shape
    //@{
    /// @brief Set/get the model to world transform.
    /// @return
    virtual glm::mat4   getModelTransform() const { return m_transform; }
    virtual void        setModelTransform(glm::mat4 transform) { m_transform = transform; }
    //@}

    virtual const float *getColor() const { return glm::value_ptr(m_color); }
    virtual void        setColor(const float r, const float g, const float b, const float a=1.0f) { m_color = glm::vec4(r,g,b,a); }

    // Shading Model
    void            setShadingModel(ShadingModel *model) { m_shadingModel.reset(model); }
    ShadingModel    *getShadingModel() const { return m_shadingModel.get(); }

    // Geometry
    virtual unsigned int    getInterleavedVertexSize() const { return 0; }
    virtual const float     *getInterleavedVertices() const { return nullptr; }
    virtual int             getInterleavedStride() const { return 0; }

    // Bounding Box
    virtual unsigned int    getInterleavedBoundingBoxVertexSize() const { return 0; }
    virtual const float     *getInterleavedBoundingBoxVertices() const { return nullptr; };
    virtual int             getInterleavedBoundingBoxStride() const { return 0; }

    virtual unsigned int        sizeofIndices() const { return 0; }
    virtual const unsigned int  *getIndices() const { return nullptr; }
    virtual unsigned int        getIndexCount() const { return 0; }

    virtual unsigned int        sizeofLineIndices() const { return 0; }
    virtual const unsigned int  *getLineIndices() const { return nullptr; }
    virtual unsigned int        getLineIndexCount() const { return 0; }

    virtual unsigned int        sizeofBoundingBoxIndices() const { return 0; }
    virtual const unsigned int  *getBoundingBoxIndices() const { return nullptr; }
    virtual unsigned int        getBoundingBoxIndexCount() const { return 0; }

    ShapeType  getType() const { return m_type; }
    void       setType(const ShapeType type) { m_type = type; }

    /// @brief scale this shape
    /// @param factor the scale factor
    void scale(const glm::vec3 factor) { m_transform = glm::scale(m_transform, factor); }

    /// @brief translate this shape
    /// @param t the translation amount in the x, y and z directions
    void translate(const glm::vec3 t) { m_transform = glm::translate(m_transform, t); }

    //@{
    /// @brief rotate this shape around the x-, y-, or z-axis
    /// @param angle the rotation angle in degrees
    void rotateY(const float angle) { m_transform = glm::rotate(m_transform,
                                                                glm::radians(angle),
                                                                glm::vec3(0.0f, 1.0f, 0.0f)); }
    void rotateX(const float angle) { m_transform = glm::rotate(m_transform,
                                                                glm::radians(angle),
                                                                glm::vec3(1.0f, 0.0f, 0.0f)); }
    void rotateZ(const float angle) { m_transform = glm::rotate(m_transform,
                                                                glm::radians(angle),
                                                                glm::vec3(0.0f, 0.0f, 1.0f)); }
    //@}
private:
    ShapeType m_type;
    glm::mat4 m_transform;
    glm::vec4 m_color;

    std::unique_ptr<ShadingModel> m_shadingModel;
};

#endif
