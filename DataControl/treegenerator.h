#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H

#include <SceneGraph/GeoNode.h>
#include <Geometry/InstancedGeometry.h>

#include "DataModel/treedescriptor.h"

using namespace imp;

class TreeGenerator
{
public:

    Geometry::Ptr _leafBase;
    Geometry::Ptr _cyl;
    Geometry::Ptr _sph;
    Geometry::Ptr _leaf;

    TreeGenerator();

    void buildInstancedBranch(int index, float radius, float height, Vec3 position, Vec3 rotation,
                              const TreeDescriptor& desc, std::vector<Matrix4>& transforms, const Matrix4& parent);

    Node::Ptr buildInstancedTree(const TreeDescriptor& desc, Material::Ptr mat, ReflexionModel::Ptr refl);

    void buildInstancedLeaf_sub(int index, float radius, float height, Vec3 position, Vec3 rotation,
                                const TreeDescriptor& desc, std::vector<Matrix4>& transforms, const Matrix4& parent);

    Node::Ptr buildInstancedLeaf(const TreeDescriptor& desc, Material::Ptr mat, ReflexionModel::Ptr refl);

};

#endif // TREEGENERATOR_H
