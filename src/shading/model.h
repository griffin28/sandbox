#ifndef INCLUDED_MODEL_H
#define INCLUDED_MODEL_H

enum class ModelType
{
    LAMBERT,
    PHONG,
    BLINN_PHONG
};

class ShadingModel
{
public:
            ShadingModel(ModelType type) : m_type(type) {}
    virtual ~ShadingModel() = default;

    ModelType        getModelType() const { return m_type; }

    virtual const char  *getVertexShaderSource() = 0;
    virtual const char  *getTessControlShaderSource() = 0;
    virtual const char  *getTessEvaluationShaderSource() = 0;
    virtual const char  *getGeometryShaderSource() = 0;
    virtual const char  *getFragmentShaderSource() = 0;

private:
    ModelType m_type;
};

#endif