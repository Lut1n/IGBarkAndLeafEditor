#ifndef GROUNDDESCRIPTOR_H
#define GROUNDDESCRIPTOR_H

#include "DataModel/geometrydescription.h"

class GroundDescriptor
        : public GeometryDescriptors
{
public:

    Meta_Class(GroundDescriptor)

    int _subdiv;
    float _sphericalN;

    int _noiseOctaves;
    float _noisePersist;
    float _noiseFreq;
    float _noiseForce;

    float _texCoordDensity;

    Vec3 _scale;
    Vec3 _pos;

    bool _transparent;

    GroundDescriptor()
        : GeometryDescriptors(Ty_Ground)
        , _subdiv(16)
        , _sphericalN(0.8)
        , _noiseOctaves(8)
        , _noisePersist(0.7)
        , _noiseFreq(2.0)
        , _noiseForce(0.3)
        , _texCoordDensity(16.0)
        , _scale(20.0,10.0,20.0)
        , _pos(0.0,-10.0,0.0)
        , _transparent(false)
    {}
};

#endif // GROUNDDESCRIPTOR_H
