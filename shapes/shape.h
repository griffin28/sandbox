#ifndef INCLUDED_SHAPE_H
#define INCLUDED_SHAPE_H

#include <glm/glm.hpp>

namespace mog {
    
// Scoped enumeration
enum class SHAPE_TYPE 
{
    SPHERE = 0,
    CUBOID,
    MESH
};

class Shape {
public:
    Shape(SHAPE_TYPE);
    Shape(SHAPE_TYPE, glm::mat4 transform);
    virtual ~Shape() = default;

    virtual glm::mat4   getTransform() const { return m_transform; }
    virtual void        setTransform(glm::mat4 transform) { m_transform = transform; };

    virtual SHAPE_TYPE  getType() const { return m_type; }
private:
    SHAPE_TYPE  m_type;
    glm::mat4   m_transform;
};

}   // namespace mog

#endif