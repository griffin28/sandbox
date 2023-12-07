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
                m_shapeIndex.emplace_back(index);
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
{
    if(sceneObjects.size() == 0)
    {
        return;
    }

    // Initialize info for each shape
    size_t shapeCount = m_sceneObjects.size();
    std::cout << "shapeCount = " << shapeCount << std::endl;
    std::vector<sandbox::BVHShapeInfo> shapeInfo;

    for(size_t i=0; i<shapeCount; i++)
    {
        auto worldBounds = m_sceneObjects[i]->shape->worldBounds();
        std::cout << "pMin: [" << worldBounds.m_pMin[0] << " , " << worldBounds.m_pMin[1] << " , " << worldBounds.m_pMin[2] << "]\n"
                     "pMax: [" << worldBounds.m_pMax[0] << " , " << worldBounds.m_pMax[1] << " , " << worldBounds.m_pMax[2] << "]\n";
        shapeInfo.emplace_back(sandbox::BVHShapeInfo{i, worldBounds});
    }

    // Build BVH tree
    m_root = build(shapeInfo, 0, shapeInfo.size());
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
        if(node->m_leaf)
        {
            for(size_t index : node->m_shapeIndex)
            {
                if(m_sceneObjects[index]->shape->worldBounds().intersect(ray))
                {
                    return index;
                }
            }

            return -1;
        }
        else
        {
            return std::max(recursiveIntersect(node->m_children[0], ray), recursiveIntersect(node->m_children[1], ray));
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
// long BVH::intersect(const Ray &ray) const
// {
//     std::cout << ray << std::endl;
//     std::stack<sandbox::BVHNode> nodeStack;

//     if(m_root != nullptr && m_root->m_bounds.intersect(ray))
//     {
//         std::cout << "Detected in bounds " << std::endl;
//         nodeStack.push(*m_root);
//     }

//     while(!nodeStack.empty())
//     {
//         auto currentNode = nodeStack.top();

//         if(currentNode.m_leaf)
//         {
//             for(size_t index : currentNode.m_shapeIndex)
//             {
//                 if(m_sceneObjects[index]->shape->worldBounds().intersect(ray))
//                 {
//                     return index;
//                 }
//             }

//             nodeStack.pop();
//         }
//         else
//         {
//             auto child0 = currentNode.m_children[0];
//             auto child1 = currentNode.m_children[1];
//             nodeStack.pop();

//             if(child0 != nullptr && child0->m_bounds.intersect(ray))
//             {
//                 nodeStack.push(*child0);
//             }

//             if(child1 != nullptr && child1->m_bounds.intersect(ray))
//             {
//                 nodeStack.push(*child1);
//             }
//         }
//     }

//     return -1;
// }

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
        std::vector<size_t> shapeIndex;
        shapeIndex.emplace_back(shapeInfo[0].m_shapeNum);
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

        if(centroidBounds.m_pMax[dim] == centroidBounds.m_pMin[dim])    // Create leaf
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
            std::sort(&shapeInfo[start], &shapeInfo[end], [dim](const sandbox::BVHShapeInfo &a, const sandbox::BVHShapeInfo &b)
                                                          {
                                                            return a.m_centroid[dim] < b.m_centroid[dim];
                                                          }
                     );
            int mid = (start + end) / 2;
            node->initInteriorNode(dim,
                                   build(shapeInfo, start, mid),
                                   build(shapeInfo, mid, end));
        }
     }

    return node;
}