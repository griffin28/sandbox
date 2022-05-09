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

    // Getter/setters
    float   getRadius() const { return m_radius; }
    float   getSectorCount() const { return m_radius * UNIT_SECTOR_COUNT; }
    float   getStackCount() const { return m_radius * UNIT_STACK_COUNT; }

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
    unsigned int    getTriangleCount() const { return getIndexCount() / 3; }
    
    unsigned int    sizeofVertices() const { return static_cast<unsigned int>(m_vertices.size() * sizeof(float)); }
    unsigned int    sizeofNormals() const { return static_cast<unsigned int>(m_normals.size() * sizeof(float)); }
    unsigned int    sizeofTexCoords() const { return static_cast<unsigned int>(m_texCoords.size() * sizeof(float)); } 
    unsigned int    sizeofIndices() const override { return static_cast<unsigned int>(m_indices.size() * sizeof(unsigned int)); }
    unsigned int    sizeofLineIndices() const override { return static_cast<unsigned int>(m_lineIndices.size() * sizeof(unsigned int)); }

    const float*        getVertices() const { return m_vertices.data(); }
    const float*        getNormals() const { return m_normals.data(); }
    const float*        getTexCoords() const { return m_texCoords.data(); }
    const unsigned int* getIndices() const override { return m_indices.data(); }
    const unsigned int* getLineIndices() const override { return m_lineIndices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int    getInterleavedVertexCount() const  { return getVertexCount(); }    // # of vertices
    unsigned int    getInterleavedVertexSize() const   { return static_cast<unsigned int>(m_interleavedVertices.size() * sizeof(float)); }    // # of bytes
    int             getInterleavedStride() const       { return 8 * sizeof(float); }   // should be 32 bytes
    const float*    getInterleavedVertices() const override { return m_interleavedVertices.data(); }

    void            printSelf() const;
private:
    void    buildVertices();
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
    std::vector<float>          m_normals;
    std::vector<float>          m_texCoords;
    std::vector<unsigned int>   m_indices;
    std::vector<unsigned int>   m_lineIndices;    

    const static int            UNIT_SECTOR_COUNT;  // longitude, # of slices
    const static int            UNIT_STACK_COUNT;   // latitude, # of stacks
};

#endif