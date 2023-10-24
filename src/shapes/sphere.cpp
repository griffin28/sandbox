#include "sphere.h"

#include <iostream>
#include <glm/ext/scalar_constants.hpp>

//----------------------------------------------------------------------------------
Sphere::Sphere() :
    m_radius(-1.0f),
    m_center(-1.0f, -1.0f, -1.0f),
    m_smooth(true),
    m_buildGeometry(true),
    m_vertices(),
    m_interleavedVertices(),
    m_interleavedBoundingBoxVertices(),
    m_normals(),
    m_texCoords(),
    m_indices(),
    m_lineIndices(),
    m_boundingBoxIndices()
{
    set(1.0f, 0.0f, 0.0f, 0.0f, true, true);
}

//----------------------------------------------------------------------------------
Sphere::Sphere(float radius, float x, float y, const float z, bool smooth, bool buildGeometry) :
    m_radius(-1.0f),
    m_center(-1.0f, -1.0f, -1.0f),
    m_smooth(true),
    m_buildGeometry(true),
    m_vertices(),
    m_interleavedVertices(),
    m_interleavedBoundingBoxVertices(),
    m_normals(),
    m_texCoords(),
    m_indices(),
    m_lineIndices(),
    m_boundingBoxIndices()
{
    set(radius, x, y, z, smooth, buildGeometry);
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Sphere::objectBounds() const
{
    glm::vec3 p0 = glm::vec3(m_center.x - m_radius,
                             m_center.y - m_radius,
                             m_center.z - m_radius);
    glm::vec3 p1 = glm::vec3(m_radius + m_center.x,
                             m_radius + m_center.y,
                             m_radius + m_center.z);

    return AxisAlignedBoundingBox(p0, p1);
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Sphere::worldBounds() const
{
   glm::vec3 p0 = glm::vec3(m_center.x - m_radius,
                             m_center.y - m_radius,
                             m_center.z - m_radius);
    glm::vec3 p1 = glm::vec3(m_radius + m_center.x,
                             m_radius + m_center.y,
                             m_radius + m_center.z);

    glm::vec4 worldp0 = glm::vec4(p0, 1.f) * this->getModelTransform();
    glm::vec4 worldp1 = glm::vec4(p1, 1.f) * this->getModelTransform();

    return AxisAlignedBoundingBox(glm::vec3(worldp0), glm::vec3(worldp1));
}

//----------------------------------------------------------------------------------
void Sphere::set(const float radius, const float x, const float y, const float z, const bool smooth, const bool buildGeometry)
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

    buildBoundingBoxVertices();
}

//----------------------------------------------------------------------------------
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
Sphere::buildBoundingBoxVertices()
{
    auto bounds = this->objectBounds();

    for(int i=0; i<8; i++)
    {
        auto corner = bounds.corner(i);
        m_interleavedBoundingBoxVertices.emplace_back(corner.x);
        // std::cout << corner.x << std::endl;
        m_interleavedBoundingBoxVertices.emplace_back(corner.y);
        // std::cout << corner.y << std::endl;
        m_interleavedBoundingBoxVertices.emplace_back(corner.z);
        // std::cout << corner.z << std::endl;

        auto normal = glm::normalize(corner);
        m_interleavedBoundingBoxVertices.emplace_back(normal.x);
        m_interleavedBoundingBoxVertices.emplace_back(normal.y);
        m_interleavedBoundingBoxVertices.emplace_back(normal.z);
    }

    m_boundingBoxIndices.emplace_back(0); m_boundingBoxIndices.emplace_back(1);
    m_boundingBoxIndices.emplace_back(0); m_boundingBoxIndices.emplace_back(2);
    m_boundingBoxIndices.emplace_back(3); m_boundingBoxIndices.emplace_back(1);
    m_boundingBoxIndices.emplace_back(3); m_boundingBoxIndices.emplace_back(2);

    m_boundingBoxIndices.emplace_back(4); m_boundingBoxIndices.emplace_back(5);
    m_boundingBoxIndices.emplace_back(4); m_boundingBoxIndices.emplace_back(6);
    m_boundingBoxIndices.emplace_back(7); m_boundingBoxIndices.emplace_back(5);
    m_boundingBoxIndices.emplace_back(7); m_boundingBoxIndices.emplace_back(6);

    m_boundingBoxIndices.emplace_back(4); m_boundingBoxIndices.emplace_back(0);
    m_boundingBoxIndices.emplace_back(2); m_boundingBoxIndices.emplace_back(6);
    m_boundingBoxIndices.emplace_back(1); m_boundingBoxIndices.emplace_back(5);
    m_boundingBoxIndices.emplace_back(7); m_boundingBoxIndices.emplace_back(3);
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