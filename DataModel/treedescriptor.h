#ifndef TREEDESCRIPTOR_H
#define TREEDESCRIPTOR_H

#include "DataModel/geometrydescription.h"


class TreeDescriptor
        : public GeometryDescriptors
{
public:

    Meta_Class(TreeDescriptor)

    float _baseRadius;
    float _baseHeight;
    int _nodeCount;
    int _divisionCount;
    float _theta;
    float _radiusFactor;
    float _heightFactor;
    float _leafSize;
    float _staggering;
    int _leafCount;
    imp::Vec3 _leafColor;
    imp::Vec3 _barkColor;

    TreeDescriptor()
        : GeometryDescriptors(Ty_Tree)
        , _baseRadius(1.0)
        , _baseHeight(7.0)
        , _nodeCount(5)
        , _divisionCount(4)
        , _theta(0.4)
        , _radiusFactor(0.5)
        , _heightFactor(0.7)
        , _leafSize(1.7)
        , _staggering(0.4)
        , _leafCount(7)
        , _leafColor(0.588,0.7568,0.3)
        , _barkColor(0.34,0.2,0.07)
    {}
};

#endif // TREEDESCRIPTOR_H
