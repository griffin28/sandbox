#include "shape.h"

Shape::Shape(SHAPE_TYPE type)
{
    Shape(type, glm::mat4(1.0f));
}

Shape::Shape(SHAPE_TYPE type, glm::mat4 transform): m_type(type), m_transform(transform)
{

}