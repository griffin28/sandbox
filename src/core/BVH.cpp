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
        void initLeafNode(std::vector<size_t> shapeIndex, const AxisAlignedBoundingBox &bounds)
        {
            m_leaf = true;
            m_shapeIndex = shapeIndex;
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
BVH::BVH(std::vector<std::shared_ptr<sandbox::SceneObject>> sceneObjects, int maxShapesPerNode=1)
    : m_sceneObject(std::move(sceneObjects))
    , m_maxShapesPerNode(maxShapesPerNode)
    , m_root(nullptr)
{
    if(sceneObjects.size() == 0)
    {
        return;
    }

    // Initialize info for each shape
    size_t shapeCount = m_sceneObject.size();
    std::vector<sandbox::BVHShapeInfo> shapeInfo(shapeCount);

    for(size_t i=0; i<shapeCount; i++)
    {
        shapeInfo[i] = {i, m_sceneObject[i]->shape->worldBounds()};
    }

    // Build BVH tree
    m_root = build(shapeInfo, 0, shapeInfo.size());
}

//----------------------------------------------------------------------------------
BVH::~BVH()
{
    if(m_root != nullptr)
    {
        deleteBVH(m_root);
    }
}

//----------------------------------------------------------------------------------
void BVH::deleteBVH(sandbox::BVHNode *root)
{
    sandbox::BVHNode *currentRoot = root;
    sandbox::BVHNode *child1 = currentRoot->m_children[0];
    sandbox::BVHNode *child2 = currentRoot->m_children[1];

    if(child1 != nullptr)
    {
        deleteBVH(child1);
    }

    if(child2 != nullptr)
    {
        deleteBVH(child2);
    }

    delete currentRoot;
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox BVH::worldBound() const
{
    return m_root ? m_root->m_bounds : AxisAlignedBoundingBox();
}

//----------------------------------------------------------------------------------
int BVH::intersect(const Ray &ray) const
{
    std::stack<sandbox::BVHNode *> nodeStack;
    // TODO: may need to copy data before placing on the stack
    if(m_root != nullptr && m_root->m_bounds.intersect(ray))
    {
        nodeStack.push(m_root);
    }

    while(!nodeStack.empty())
    {
        auto currentNode = nodeStack.top();

        if(currentNode->m_leaf)
        {
            for(size_t index : currentNode->m_shapeIndex)
            {
                if(m_sceneObject[index]->shape->worldBounds().intersect(ray))
                {
                    return index;
                }
            }

            nodeStack.pop();
        }
        else
        {
            auto child1 = currentNode->m_children[0];
            auto child2 = currentNode->m_children[1];
            nodeStack.pop();

            if(child1 != nullptr && child1->m_bounds.intersect(ray))
            {
                nodeStack.push(child1);
            }

            if(child2 != nullptr && child2->m_bounds.intersect(ray))
            {
                nodeStack.push(child2);
            }
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------
sandbox::BVHNode *BVH::build(std::vector<sandbox::BVHShapeInfo> &shapeInfo, int start, int end)
{
    sandbox::BVHNode *node = new sandbox::BVHNode();

    // Compute bounds
    AxisAlignedBoundingBox bounds;
    for(int i=start; i<end; i++)
    {
        bounds = AxisAlignedBoundingBox::combine(bounds, shapeInfo[i].m_bounds);
    }

     int nShapes = end - start;

     if(nShapes == 1)
     {
        std::vector<size_t> shapeIndex(1);
        shapeIndex[0] = shapeInfo[0].m_shapeNum;
        node->initLeafNode(shapeIndex, bounds);
     }
     else
     {
        AxisAlignedBoundingBox centroidBounds;

        for(int i=start; i<end; i++)
        {
            centroidBounds = AxisAlignedBoundingBox::combine(centroidBounds, shapeInfo[i].m_centroid);
        }

        int dim = centroidBounds.maxExtent();

        if(centroidBounds.m_pMax[dim] == centroidBounds.m_pMin[dim])
        {
            std::vector<size_t> shapeIndex;

            for(int i=start; i<end; i++)
            {
                shapeIndex.emplace_back(shapeInfo[i].m_shapeNum);
            }

            node->initLeafNode(shapeIndex, bounds);
        }
        else
        {
            std::sort(&shapeInfo[start], &shapeInfo[end], [dim](const sandbox::BVHShapeInfo &a,
                                                                const sandbox::BVHShapeInfo &b)
                                                                {
                                                                    return a.m_centroid[dim] < b.m_centroid[dim];
                                                                });
            int mid = (start + end) / 2;
            node->initInteriorNode(dim,
                                   build(shapeInfo, start, mid),
                                   build(shapeInfo, mid, end));
        }
     }

    return node;
}