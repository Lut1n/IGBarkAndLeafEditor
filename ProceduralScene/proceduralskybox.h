#ifndef PROCEDURALSKYBOX_H
#define PROCEDURALSKYBOX_H

#include <SceneGraph/RenderToSamplerNode.h>
#include <SceneGraph/GeoNode.h>

#include "DataControl/loadingtaskqueue.h"

#include <QDebug>
#include <QFile>

using namespace imp;

struct SkyGen : public RenderToSamplerNode
{
public:

    Meta_Class(SkyGen)

    ReflexionModel::Ptr _shaderGen;
    GeoNode::Ptr skyGeo;

    SkyGen();
};

struct ProceduralSkyBox : public GeoNode, public CubeMapHandler
{
public:

    Meta_Class(ProceduralSkyBox)

    CubeMapSampler::Ptr _cubemap;
    ReflexionModel::Ptr _shader;
    SkyGen::Ptr _subrender;

    void handleCubemap(const std::vector<imp::Image::Ptr>& images);

    ProceduralSkyBox();

    virtual ~ProceduralSkyBox(){}
};

#endif // PROCEDURALSKYBOX_H
