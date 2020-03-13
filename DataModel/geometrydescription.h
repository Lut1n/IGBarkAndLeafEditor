#ifndef GEOMETRYDESCRIPTION_H
#define GEOMETRYDESCRIPTION_H

#include <Core/Vec3.h>
#include <SceneGraph/Material.h>
#include <SceneGraph/GeoNode.h>

using namespace imp;

class GeometryDescriptors : public Object
{
public:

    Meta_Class(GeometryDescriptors)

    enum Ty
    {
      Ty_Tree,
      Ty_Leaf,
      Ty_Rock,
      Ty_Ground,
      Ty_Water,
      Ty_Sky,
    };

    GeometryDescriptors(Ty t);

    virtual ~GeometryDescriptors(){}

    Ty _ty;
};


#endif // GEOMETRYDESCRIPTION_H
