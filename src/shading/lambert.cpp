#include "lambert.h"

LambertShadingModel::LambertShadingModel() : ShadingModel(ModelType::LAMBERT) {}

const char *
LambertShadingModel::getVertexShaderSource()
{
    const char *source = R"(
    #version 430 core
    // uniforms
    uniform mat4 mvMatrix;
    uniform mat4 mvNormalMatrix;
    uniform mat4 mvpMatrix;
    // vertex attribs (input)
    in vec3 position;
    in vec3 normal;
    in vec2 texCoord;
    // output (varyings)
    out VS_OUT
    {
        vec3 position;
        vec3 N;
        vec2 tc;
    } vs_out;
    void main()
    {
        vs_out.position = vec3(mvMatrix * vec4(position, 1.0));
        vs_out.N = vec3(mvNormalMatrix * vec4(normal, 1.0));
        vs_out.tc = texCoord;
        gl_Position = mvpMatrix * vec4(position, 1.0);
    }
    )";

    return source;
}

const char *
LambertShadingModel::getTessControlShaderSource()
{
    return nullptr;
}

const char *
LambertShadingModel::getTessEvaluationShaderSource()
{
    return nullptr;
}

const char  *
LambertShadingModel::getGeometryShaderSource()
{
    return nullptr;
}

const char *
LambertShadingModel::getFragmentShaderSource()
{
    const char *source = R"(
    #version 430 core
    // uniforms
    uniform vec4 lightPosition;
    uniform vec4 lightAmbient;
    uniform vec4 lightDiffuse;
    uniform vec4 lightSpecular;
    uniform vec4 materialAmbient;
    uniform vec4 materialDiffuse;
    uniform vec4 materialSpecular;
    uniform float materialShininess;
    uniform vec4 materialColor;
    uniform sampler2D s2D;
    uniform bool textureUsed;
    uniform bool colorUsed;
    // varyings
    in VS_OUT
    {
        vec3 position;
        vec3 N;
        vec2 tc;
    } fs_in;
    out vec4 color;
    void main()
    {
        vec3 N = normalize(fs_in.N);
        vec3 light;
        if(lightPosition.w == 0.0)
        {
            light = normalize(lightPosition.xyz);
        }
        else
        {
            light = normalize(lightPosition.xyz - fs_in.position);
        }
        vec3 view = normalize(-fs_in.position);
        vec3 halfv = normalize(light + view);

        vec3 tempColor = lightAmbient.rgb * materialAmbient.rgb;
        float dotNL = max(dot(N, light), 0.0);
        tempColor += lightDiffuse.rgb * materialDiffuse.rgb * dotNL;
        if(textureUsed)
            tempColor *= texture(s2D, fs_in.tc).rgb;
        float dotNH = max(dot(N, halfv), 0.0);
        tempColor += pow(dotNH, materialShininess) * lightSpecular.rgb * materialSpecular.rgb;

        // set fragment shader color
        if(colorUsed)
        {
            // selection color;
            color = vec4(1.0,0.0,0.0,1.0);
        }
        else
        {
            color = vec4(tempColor, materialDiffuse.a);
        }
    }
    )";

    return source;
}