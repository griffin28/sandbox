#include "sphere.h"

#include <iostream>
#include <glm/ext/scalar_constants.hpp>


using namespace mog;

// The sector and stack counts per 1 unit of radius
const int Sphere::UNIT_SECTOR_COUNT = 10;
const int Sphere::UNIT_STACK_COUNT = 5;

Sphere::Sphere() : Shape(SHAPE_TYPE::SPHERE), 
    m_radius(-1.0f),
    m_center(-1.0f, -1.0f, -1.0f),
    m_smooth(true),
    m_buildGeometry(true),
    m_vertices(),
    m_interleavedVertices(),
    m_normals(),
    m_texCoords(),
    m_indices(),
    m_lineIndices()
{
    set(1.0f, 0.0f, 0.0f, 0.0f, true, true);
}

Sphere::Sphere(float radius, float x, float y, const float z, bool smooth, bool buildGeometry) : Shape(SHAPE_TYPE::SPHERE),
    m_radius(-1.0f),
    m_center(-1.0f, -1.0f, -1.0f),
    m_smooth(true),
    m_buildGeometry(true),
    m_vertices(),
    m_interleavedVertices(),
    m_normals(),
    m_texCoords(),
    m_indices(),
    m_lineIndices()
{
    set(radius, x, y, z, smooth, buildGeometry);
}

void
Sphere::set(const float radius, const float x, const float y, const float z, const bool smooth, const bool buildGeometry) 
{
    if(radius == m_radius && 
       smooth == m_smooth && 
       buildGeometry == m_buildGeometry &&
       m_center.x == x &&
       m_center.y == y &&
       m_center.z == z)
    {
        return;
    }

    m_radius = radius;
    m_smooth = smooth;
    m_center.x = x;
    m_center.y = y;
    m_center.z = z;
    
    if(buildGeometry) 
    {
        m_buildGeometry = buildGeometry;
        buildVertices();
    }
    else 
    {
        if(buildGeometry != m_buildGeometry) 
        {
            m_buildGeometry = buildGeometry;
            clearArrays();
        }
    } 
}

void 
Sphere::setRadius(const float radius) 
{
    if(radius != m_radius) 
    {
        set(radius, m_center.x, m_center.y, m_center.z, m_smooth, m_buildGeometry);
    }
}

void
Sphere::setBuildGeometry(const bool build) 
{
    if(build != m_buildGeometry) 
    {
        set(m_radius, m_center.x, m_center.y, m_center.z, m_smooth, build);
    }
}

void
Sphere::setSmooth(const bool smooth)
{
    if(smooth != m_smooth) 
    {
        set(m_radius, m_center.x, m_center.y, m_center.z, smooth, m_buildGeometry);
    }
}

void
Sphere::setCenter(const float x, const float y, const float z)
{
    if(m_center.x != x || m_center.y != y || m_center.z != z)
    {
        set(m_radius, x, y, z, m_smooth, m_buildGeometry);
    }
}

void
Sphere::clearArrays() 
{
    // vector::clear doesn't guarantee a reallocation, this way
    // we can force a reallocation
    std::vector<float>().swap(m_vertices); 
    std::vector<float>().swap(m_normals);
    std::vector<float>().swap(m_texCoords);
    std::vector<unsigned int>().swap(m_indices);
    std::vector<unsigned int>().swap(m_lineIndices);
}

void
Sphere::buildVertices() 
{
    if(m_smooth) {
        buildVerticesSmooth();
    } else {
        buildVerticesFlat();
    }
}

///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation
// x = r * cos(u) * cos(v)
// y = r * cos(u) * sin(v)
// z = r * sin(u)
// where u: stack(latitude) angle (-90 <= u <= 90)
//       v: sector(longitude) angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void
Sphere::buildVerticesSmooth() 
{
    const float PI = acos(-1); // glm::pi<float>();

    clearArrays();

    // vertex position
    float x, y, z, xy;
    // normal
    float nx, ny, nz, lengthInv = 1.0f / m_radius;
    // texture coords
    float s, t;

    int sectorCount = getSectorCount();
    int stackCount = getStackCount();

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i=0; i<=stackCount; i++) 
    {
        stackAngle = PI / 2 - i * stackStep;
        xy = m_radius * cosf(stackAngle);
        z = m_radius * sinf(stackAngle) + m_center.z;

        // add sectorCount+1 vertices per stack
        // first and last vertices have same position and normal but different texcoords
        for(int j=0; j<=sectorCount; j++) 
        {
            sectorAngle = j * sectorStep;   // 0 to 2PI
            // vertex position
            x = xy * cosf(sectorAngle) + m_center.x;
            y = xy * sinf(sectorAngle) + m_center.y;
            addVertex(x,y,z);
            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);
            // vertex texture coords between [0,1]
            s = static_cast<float>(j) / sectorCount;
            t = static_cast<float>(i) / stackCount;
            addTextureCoordinate(s, t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i=0; i<stackCount; i++) 
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for(int j=0; j<sectorCount; ++j, ++k1, ++k2) 
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0) {
                addIndices(k1, k2, k1+1);
            }

            if(i != (stackCount -1)) {
                addIndices(k1+1, k2, k2+1); 
            }

            // vertical lines for all stacks
            m_lineIndices.push_back(k1);
            m_lineIndices.push_back(k2);

            if(i != 0) {
                m_lineIndices.push_back(k1);
                m_lineIndices.push_back(k1 + 1);
            }
        }
    }

    buildInterleavedVertices();
}

void
Sphere::buildVerticesFlat() 
{
    // TODO: Implement
}

void
Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(m_interleavedVertices);

    std::size_t i, j;
    std::size_t count = m_vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        m_interleavedVertices.push_back(m_vertices[i]);
        m_interleavedVertices.push_back(m_vertices[i+1]);
        m_interleavedVertices.push_back(m_vertices[i+2]);

        m_interleavedVertices.push_back(m_normals[i]);
        m_interleavedVertices.push_back(m_normals[i+1]);
        m_interleavedVertices.push_back(m_normals[i+2]);

        m_interleavedVertices.push_back(m_texCoords[j]);
        m_interleavedVertices.push_back(m_texCoords[j+1]);
    }

}

void
Sphere::addVertex(const float x, const float y, const float z) 
{
    // m_vertices.push_back(glm::vec3(x,y,z));
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void
Sphere::addNormal(const float nx, const float ny, const float nz) 
{
    m_normals.push_back(nx);
    m_normals.push_back(ny);
    m_normals.push_back(nz);
}

void
Sphere::addTextureCoordinate(const float s, const float t)
{
    m_texCoords.push_back(s);
    m_texCoords.push_back(t);
}

void
Sphere::addIndices(const unsigned int i1, const unsigned int i2, const unsigned int i3) 
{
    m_indices.push_back(i1);
    m_indices.push_back(i2);
    m_indices.push_back(i3);
}

glm::vec3
Sphere::computeFaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) 
{
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v1;

    glm::vec3 normal = glm::cross(e1, e2);
    normal = glm::normalize(normal);

    return normal;
}

void
Sphere::printSelf() const
{
    std::cout << "==== Sphere ===="
              << "        Radius: " << m_radius << "\n"
              << "  Sector Count: " << getSectorCount() << "\n"
              << "   Stack Count: " << getStackCount() << "\n"
              << "Smooth Shading: " << (m_smooth ? "true" : "false") << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n"
              << "  Vertex Count: " << getVertexCount() << "\n"
              << "  Normal Count: " << getNormalCount() << "\n"
              << "TexCoord Count: " << getTexCoordCount() << std::endl;
}


// TODO: add to a material class
const char *
Sphere::getVertexShaderSource() 
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
Sphere::getFragmentShaderSource()
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
            color = materialColor;
        }
        else
        {
            color = vec4(tempColor, materialDiffuse.a);
        }
    }
    )";

    return source;
}

