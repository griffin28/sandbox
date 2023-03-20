#include "shape.h"

Shape::Shape() : m_type(ShapeType::MESH),
                 m_transform(glm::mat4(1.0f)),
                 m_color(glm::vec4(1.0f)),
                 m_shadingModel(nullptr) {}