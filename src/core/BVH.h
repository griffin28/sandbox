#ifndef INCLUDED_BVH_H
#define INCLUDED_BVH_H

#include <vector>
#include <memory>

class Shape;
class AxisAlignedBoundingBox;
class Ray;

namespace sandbox
{
    struct BVHShapeInfo;
    struct BVHNode;
    struct SceneObject;
}

/// @class BVH
/// @brief Bounding Volume Heirarchy
///
/// Bounding volume heirarchy (BVH) is an approach for ray intersection acceleration based on
/// primitive subdivision, where the primitives are partitioned into a hierarchy of disjoint sets.
/// Primitives are stored in the leaves and each node stores a bounding box of the primitives
/// in the nodes beneath it. Thus, as a ray traverses through the tree, any time it doesn't
/// intersect a node's bounds, the subtree beneath that node can be skipped.
///
/// This implementation uses equal counts to partition primitives into equally sized subsets
/// when building the tree.
class BVH
{
public:
    BVH(std::vector<std::shared_ptr<sandbox::SceneObject>> sceneObject, int maxShapesPerNode=1);
    ~BVH();

    /// @brief Get the world space bounds for this axis-aligned bounding box.
    /// @return Bounding box using world space coordinates
    AxisAlignedBoundingBox worldBound() const;

    /// @brief Determines if a ray intersects this bounding box.
    /// @param ray the ray to test for intersection
    /// @return index of the primitive that the ray intersects, otherwise -1
    int intersect(const Ray &ray) const;
private:
    sandbox::BVHNode *build(std::vector<sandbox::BVHShapeInfo> &shapeInfo, int start, int end);
    void deleteBVH(sandbox::BVHNode *root);

    const int m_maxShapesPerNode;;
    std::vector<std::shared_ptr<sandbox::SceneObject>> m_sceneObject;
    sandbox::BVHNode *m_root;
};

#endif