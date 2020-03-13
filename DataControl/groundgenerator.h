#ifndef GROUNDGENERATOR_H
#define GROUNDGENERATOR_H

#include <SceneGraph/GeoNode.h>

#include "DataModel/grounddescriptor.h"

class GroundGenerator
{
public:

    static imp::GeoNode::Ptr generate(GroundDescriptor::Ptr d, imp::ReflexionModel::Ptr refl, imp::Material::Ptr mat);
};

#endif // GROUNDGENERATOR_H
