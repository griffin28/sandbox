#ifndef INCLUDED_SPHERE_H
#define INCLUDED_SPHERE_H

#include "shape.h"

#include <vector>

class Sphere : public Shape
{
public:
    Sphere();
    Sphere(float r, float x, float y, float z, bool smooth=true, bool buildGeometry=true);
    ~Sphere() = default;

    AxisAlignedBoundingBox objectBounds() const override;
    AxisAlignedBoundingBox worldBounds() const override;

    bool intersect(const Ray &ray) const override;
    void rotate(const float angle) override;

    // Getter/setters
    float   getRadius() const { return m_radius; }
    float   getSectorCount() const { return m_radius * UNIT_SECTOR_COUNT; }
    float   getStackCount() const { return m_radius * UNIT_STACK_COUNT; }
    bool    getBuildGeometry() const { return m_buildGeometry; }

    void    set(const float, const float, const float, const float, const bool, const bool);
    void    setRadius(const float);
    void    setSmooth(const bool);
    void    setBuildGeometry(const bool);
    void    setCenter(const float, const float, const float);

    // For vertex data
    unsigned int    getVertexCount() const { return static_cast<unsigned int>(m_vertices.size()) / 3; }
    unsigned int    getNormalCount() const { return static_cast<unsigned int>(m_normals.size()) / 3; }
    unsigned int    getTexCoordCount() const { return static_cast<unsigned int>(m_texCoords.size()) / 2; }
    unsigned int    getIndexCount() const override { return static_cast<unsigned int>(m_indices.size()); }
    unsigned int    getLineIndexCount() const override { return static_cast<unsigned int>(m_lineIndices.size()); }
    unsigned int    getBoundingBoxIndexCount() const override { return static_cast<unsigned int>(m_boundingBoxIndices.size()); }
    unsigned int    getTriangleCount() const { return getIndexCount() / 3; }

    unsigned int    sizeofVertices() const { return static_cast<unsigned int>(m_vertices.size() * sizeof(float)); }
    unsigned int    sizeofNormals() const { return static_cast<unsigned int>(m_normals.size() * sizeof(float)); }
    unsigned int    sizeofTexCoords() const { return static_cast<unsigned int>(m_texCoords.size() * sizeof(float)); }
    unsigned int    sizeofIndices() const override { return static_cast<unsigned int>(m_indices.size() * sizeof(unsigned int)); }
    unsigned int    sizeofLineIndices() const override { return static_cast<unsigned int>(m_lineIndices.size() * sizeof(unsigned int)); }
    unsigned int    sizeofBoundingBoxIndices() const override { return static_cast<unsigned int>(m_boundingBoxIndices.size() * sizeof(unsigned int)); }

    const float*        getVertices() const { return m_vertices.data(); }
    const float*        getNormals() const { return m_normals.data(); }
    const float*        getTexCoords() const { return m_texCoords.data(); }
    const unsigned int* getIndices() const override { return m_indices.data(); }
    const unsigned int* getLineIndices() const override { return m_lineIndices.data(); }
    const unsigned int* getBoundingBoxIndices() const override { return m_boundingBoxIndices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int    getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    unsigned int    getInterleavedVertexSize() const override { return static_cast<unsigned int>(m_interleavedVertices.size() * sizeof(float)); }    // # of bytes
    int             getInterleavedStride() const override { return 8 * sizeof(float); }   // should be 32 bytes
    const float*    getInterleavedVertices() const override { return m_interleavedVertices.data(); }

    // For interleaved bounding box vertices: V/N
    unsigned int    getInterleavedBoundingBoxVertexCount() const  { return 8; }
    unsigned int    getInterleavedBoundingBoxVertexSize() const override { return static_cast<unsigned int>(m_interleavedBoundingBoxVertices.size() * sizeof(float)); }    // # of bytes
    int             getInterleavedBoundingBoxStride() const override { return 6 * sizeof(float); }
    const float*    getInterleavedBoundingBoxVertices() const override { return m_interleavedBoundingBoxVertices.data(); }

    void            printSelf() const;
private:
    void    buildVertices();
    void    buildBoundingBoxVertices();
    void    buildVerticesSmooth();
    void    buildVerticesFlat();
    void    buildInterleavedVertices();
    void    clearArrays();
    void    addVertex(const float, const float, const float);
    void    addNormal(const float, const float, const float);
    void    addTextureCoordinate(const float, const float);
    void    addIndices(const unsigned int, const unsigned int, const unsigned int);
    glm::vec3  computeFaceNormal(glm::vec3, glm::vec3, glm::vec3);

    float       m_radius;
    glm::vec3   m_center;
    bool        m_smooth;
    bool        m_buildGeometry;

    std::vector<float>          m_vertices;
    std::vector<float>          m_interleavedVertices;
    std::vector<float>          m_interleavedBoundingBoxVertices;
    std::vector<float>          m_normals;
    std::vector<float>          m_texCoords;
    std::vector<unsigned int>   m_indices;
    std::vector<unsigned int>   m_lineIndices;
    std::vector<unsigned int>   m_boundingBoxIndices;

    constexpr static int        UNIT_SECTOR_COUNT = 10;  // longitude, # of slices
    constexpr static int        UNIT_STACK_COUNT = 5;   // latitude, # of stacks
};

#endif