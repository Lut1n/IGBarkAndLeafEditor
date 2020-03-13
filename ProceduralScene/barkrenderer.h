#ifndef BARKRENDERER_H
#define BARKRENDERER_H

#include "DataModel/barkdescription.h"
#include <SceneGraph/RenderToSamplerNode.h>
#include <SceneGraph/QuadNode.h>

#include <QFile>

using namespace imp;

class BarkRenderer : public RenderToSamplerNode
{
    public:

    Meta_Class(BarkRenderer)

    Uniform::Ptr u_octaves;
    Uniform::Ptr u_freq;
    Uniform::Ptr u_persist;
    Uniform::Ptr u_scaleX;
    Uniform::Ptr u_scaleY;

    BarkDescription desc;

    BarkRenderer();

    void updateDesc(const BarkDescription& ld);
};

#endif // BARKRENDERER_H
