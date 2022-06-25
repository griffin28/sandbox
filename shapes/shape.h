#ifndef INCLUDED_SHAPE_H
#define INCLUDED_SHAPE_H

#include "shading/model.h"
// TODO: #include "aabb.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

// TODO:
// - bounding box
// - ray intersection 

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
    Shape(ShapeType);
    Shape(ShapeType, glm::mat4 transform);
    virtual ~Shape() = default;

    virtual glm::mat4   getTransform() const { return m_transform; }
    virtual void        setTransform(glm::mat4 transform) { m_transform = transform; }

    virtual const float *getColor() const { return glm::value_ptr(m_color); }
    virtual void        setColor(const float r, const float g, const float b, const float a=1.0f) { m_color = glm::vec4(r,g,b,a); }

    // Shading Model
    void            setShadingModel(ShadingModel *model) { m_shadingModel.reset(model); }
    ShadingModel    *getShadingModel() const { return m_shadingModel.get(); }

    // Geometry
    virtual unsigned int    getInterleavedVertexSize() const { return 0; }
    virtual const float     *getInterleavedVertices() const { return nullptr; };

    virtual unsigned int        sizeofIndices() const { return 0; }
    virtual const unsigned int  *getIndices() const { return nullptr; }
    virtual unsigned int        getIndexCount() const { return 0; }

    virtual unsigned int        sizeofLineIndices() const { return 0; }
    virtual const unsigned int  *getLineIndices() const { return nullptr; }
    virtual unsigned int        getLineIndexCount() const { return 0; }

    virtual ShapeType  getType() const { return m_type; }

    //virtual AxisAlignedBB objectBounds() const = 0;
    //virtual AxisAlignedBB worldBounds() const = 0;

    //virtual bool intersect() const = 0;
private:
    ShapeType m_type;
    glm::mat4 m_transform;
    glm::vec4 m_color;

    std::unique_ptr<ShadingModel> m_shadingModel;
};

#endif
