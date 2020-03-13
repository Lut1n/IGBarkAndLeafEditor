#ifndef LEAFRENDERER_H
#define LEAFRENDERER_H

#include "DataModel/leafdescription.h"
#include <SceneGraph/RenderToSamplerNode.h>
#include <SceneGraph/QuadNode.h>

#include <QFile>

using namespace imp;

class LeafRenderer : public RenderToSamplerNode
{
    public:

    Meta_Class(LeafRenderer)

    Uniform::Ptr u_branchDepth;
    Uniform::Ptr u_branchCount;
    Uniform::Ptr u_texOctave;
    Uniform::Ptr u_texPersist;
    Uniform::Ptr u_texFreq;
    Uniform::Ptr u_ShapeFq1;
    Uniform::Ptr u_ShapeFq2;
    Uniform::Ptr u_ShapeMin;
    Uniform::Ptr u_ShapeMax;
    Uniform::Ptr u_Tilt;
    Uniform::Ptr u_Len;
    Uniform::Ptr u_LenFct;
    Uniform::Ptr u_Thickness;

    LeafDescription desc;

    LeafRenderer();

    void updateDesc(const LeafDescription& ld);
};


#endif // LEAFRENDERER_H
