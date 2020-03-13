#ifndef ROCKGENERATOR_H
#define ROCKGENERATOR_H

#include <SceneGraph/GeoNode.h>

#include "DataModel/rockdescriptor.h"

using namespace imp;

class RockGenerator
{
public:

    Geometry::Ptr _cyl;
    Geometry::Ptr _sph;

    RockGenerator();


    Geometry buildRockGeo(int index,
                             float radius, float height, Vec3 position, Vec3 rotation,
                             const RockDescriptor& desc);


    GeoNode::Ptr generateRock(const RockDescriptor& desc,
                              Material::Ptr mat, ReflexionModel::Ptr reflexion, bool wireframe);

};

#endif // ROCKGENERATOR_H
