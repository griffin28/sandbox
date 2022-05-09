#ifndef INCLUDED_MODEL_LAMBERT_H
#define INCLUDED_MODEL_LAMBERT_H

#include "model.h"

class LambertShadingModel : public ShadingModel
{
public:
    LambertShadingModel();
    ~LambertShadingModel() = default;

    const char  *getVertexShaderSource() override;
    const char  *getTessControlShaderSource() override;
    const char  *getTessEvaluationShaderSource() override;
    const char  *getGeometryShaderSource() override;
    const char  *getFragmentShaderSource() override;  
};

#endif