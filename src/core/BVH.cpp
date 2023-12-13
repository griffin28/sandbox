#include "BVH.h"
#include "shape.h"
#include "AABB.h"
#include "Ray.h"
#include "RasterizationScene.h"

#include <algorithm>
#include <stack>

namespace sandbox
{
    struct BVHShapeInfo
    {
        BVHShapeInfo() {}
        BVHShapeInfo(size_t shapeNum, const AxisAlignedBoundingBox &bounds)
            : m_shapeNum(shapeNum)
            , m_bounds(bounds)
            , m_centroid(.5f * bounds.m_pMin + .5f * bounds.m_pMax) {}

        size_t m_shapeNum;
        AxisAlignedBoundingBox m_bounds;
        glm::vec3 m_centroid;
    };

    struct BVHNode
    {
        BVHNode() {}
        ~BVHNode() { ; }

        void initLeafNode(const std::vector<size_t> &shapeIndex, const AxisAlignedBoundingBox &bounds)
        {
            for(auto index : shapeIndex)
            {
                m_shapeIndex.push_back(index);
            }

            m_leaf = true;
            m_bounds = bounds;
            m_children[0] = m_children[1] = nullptr;
            m_splitAxis = -1;
        }

        void initInteriorNode(const int axis, BVHNode *child0, BVHNode *child1)
        {
            m_leaf = false;
            m_children[0] = child0;
            m_children[1] = child1;
            m_bounds = AxisAlignedBoundingBox::combine(child0->m_bounds, child1->m_bounds);
            m_splitAxis = axis;
        }

        bool m_leaf;
        AxisAlignedBoundingBox m_bounds;
        int m_splitAxis;
        std::vector<size_t> m_shapeIndex;
        BVHNode *m_children[2];
    };
}

//----------------------------------------------------------------------------------
BVH::BVH(const std::vector<sandbox::SceneObject *> &sceneObjects, int maxShapesPerNode)
    : m_sceneObjects(sceneObjects)
    , m_maxShapesPerNode(maxShapesPerNode)
    , m_root(nullptr)
    , m_totalNodes(0)
    , m_totalLeafNodes(0)
    , m_totalInteriorNodes(0)
{
    if(sceneObjects.size() == 0)
    {
        return;
    }

    // Initialize info for each shape
    size_t shapeCount = m_sceneObjects.size();
    std::vector<sandbox::BVHShapeInfo> shapeInfo;

    for(size_t i=0; i<shapeCount; i++)
    {
        auto worldBounds = m_sceneObjects[i]->shape->worldBounds();
        shapeInfo.push_back(sandbox::BVHShapeInfo{i, worldBounds});
    }

    // Build BVH tree
    std::cout << "Building BVH..." << std::endl;
    m_root = build(shapeInfo, 0, shapeInfo.size());
    std::cout << "Done building BVH. Total nodes = " << m_totalNodes
              << " Total leaf nodes = " << m_totalLeafNodes
              << " Total interior nodes = " << m_totalInteriorNodes << std::endl;

    auto worldBounds = m_root->m_bounds;
    std::cout << "Root Bounds" << std::endl;
    std::cout << "pMin: [" << worldBounds.m_pMin[0] << " , " << worldBounds.m_pMin[1] << " , " << worldBounds.m_pMin[2] << "]\n"
                 "pMax: [" << worldBounds.m_pMax[0] << " , " << worldBounds.m_pMax[1] << " , " << worldBounds.m_pMax[2] << "]\n";
}

//----------------------------------------------------------------------------------
BVH::~BVH()
{
    deleteBVH(m_root);
}

//----------------------------------------------------------------------------------
void BVH::deleteBVH(sandbox::BVHNode *root)
{
    if(root != nullptr)
    {
        auto child0 = root->m_children[0];
        auto child1 = root->m_children[1];

        if(child0 != nullptr)
        {
            deleteBVH(child0);
        }

        if(child1 != nullptr)
        {
            deleteBVH(child1);
        }

        delete root;
    }
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox BVH::getBounds() const
{
    return m_root ? m_root->m_bounds : AxisAlignedBoundingBox();
}

//----------------------------------------------------------------------------------
long BVH::recursiveIntersect(sandbox::BVHNode *node, const Ray &ray) const
{
    if(node != nullptr && node->m_bounds.intersect(ray))
    {
        if(node->m_leaf || (node->m_children[0] == nullptr && node->m_children[1] == nullptr))
        {
            for(size_t index : node->m_shapeIndex)
            {
                if(m_sceneObjects[index]->shape->intersect(ray))
                {
                    return index;
                }
            }

            return -1;
        }
        else
        {
            return std::max((node->m_children[0] != nullptr ? recursiveIntersect(node->m_children[0], ray) : -1),
                            (node->m_children[1] != nullptr ? recursiveIntersect(node->m_children[1], ray) : -1));
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------
long BVH::intersect(const Ray &ray) const
{
    return recursiveIntersect(m_root, ray);
}

//----------------------------------------------------------------------------------
sandbox::BVHNode *BVH::build(std::vector<sandbox::BVHShapeInfo> &shapeInfo, std::size_t start, std::size_t end)
{
    std::size_t nShapes = end - start;

    if(nShapes <= 0)
    {
        return nullptr;
    }

    sandbox::BVHNode *node = new sandbox::BVHNode();
    ++m_totalNodes;

    // Compute bounds
    AxisAlignedBoundingBox bounds;

    for(std::size_t i=start; i<end; i++)
    {
        bounds = AxisAlignedBoundingBox::combine(bounds, shapeInfo[i].m_bounds);
    }

    if(nShapes == 1)
    {
        std::vector<size_t> shapeIndex;

        for(std::size_t i=start; i<end; i++)
        {
            shapeIndex.push_back(shapeInfo[i].m_shapeNum);
        }

        node->initLeafNode(shapeIndex, bounds);
        ++m_totalLeafNodes;
    }
    else
    {
        AxisAlignedBoundingBox centroidBounds;

        for(std::size_t i=start; i<end; i++)
        {
            centroidBounds = AxisAlignedBoundingBox::combine(centroidBounds, shapeInfo[i].m_centroid);
        }

        const int dim = centroidBounds.maxExtent();

        if((centroidBounds.m_pMax[dim] - centroidBounds.m_pMin[dim]) <= 0.1f) // Create leaf
        {
            std::vector<size_t> shapeIndex;

            for(std::size_t i=start; i<end; i++)
            {
                shapeIndex.push_back(shapeInfo[i].m_shapeNum);
            }

            node->initLeafNode(shapeIndex, bounds);
            ++m_totalLeafNodes;
        }
        else
        {
            std::sort(&shapeInfo[start], &shapeInfo[end], [dim](const sandbox::BVHShapeInfo &a, const sandbox::BVHShapeInfo &b)
                                                          {
                                                            return a.m_centroid[dim] < b.m_centroid[dim];
                                                          });
            std::size_t mid = (start + end) / 2;
            ++m_totalInteriorNodes;
            node->initInteriorNode(dim, build(shapeInfo, start, mid), build(shapeInfo, mid, end));
        }
    }

    return node;
}