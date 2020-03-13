#ifndef ROCKDESCRIPTOR_H
#define ROCKDESCRIPTOR_H

#include "DataModel/geometrydescription.h"


class RockDescriptor
        : public GeometryDescriptors
{
public:

    Meta_Class(RockDescriptor)

    float _baseRadius;
    // float _baseHeight;
    int _nodeCount;
    int _divisionCount;
    // float _theta;
    float _radiusFactor;
    // float _heightFactor;

    RockDescriptor()
        : GeometryDescriptors(Ty_Rock)
        , _baseRadius(3.0)
        // , _baseHeight(5.0)
        , _nodeCount(2)
        , _divisionCount(3)
        // , _theta(3.0/4.0)
        , _radiusFactor(0.7)
        // , _heightFactor(0.7)
    {}
};

#endif // ROCKDESCRIPTOR_H
