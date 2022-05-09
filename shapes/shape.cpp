#include "shape.h"

Shape::Shape() : m_type(ShapeType::MESH), 
                 m_transform(glm::mat4(1.0f)), 
                 m_shadingModel(nullptr) {}

Shape::Shape(ShapeType type)
{
    Shape(type, glm::mat4(1.0f));
}

Shape::Shape(ShapeType type, glm::mat4 transform): m_type(type), 
                                                   m_transform(transform),
                                                   m_shadingModel(nullptr)
{
}