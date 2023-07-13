#ifndef INCLUDED_BVH_H
#define INCLUDED_BVH_H

/// @class BVH
/// @brief Bounding Volume Heirarchy
///
/// Bounding volume heirarchy (BVH) is an approach for ray intersection acceleration based on
/// primitive subdivision, where the primitives are partitioned into a hierarchy of disjoint sets.
/// Primitives are stored in the leaves and each node stores a bounding box of the primitives
/// in the nodes beneath it. Thus, as a ray traverses through the tree, any time it doesn't
/// intersect a node's bounds, the subtree beneath that node can be skipped.
class BVH
{
public:
    BVH();

};

#endif