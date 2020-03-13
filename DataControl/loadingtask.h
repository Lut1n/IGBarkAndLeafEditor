#ifndef LOADINGTASK_H
#define LOADINGTASK_H


#include <SceneGraph/Graph.h>

#include "DataControl/texturecache.h"
#include "DataControl/treegenerator.h"

// base super class for loading task
struct LoadingTaskInfoBase : public Object
{
    Meta_Class(LoadingTaskInfoBase)

    enum Type
    {
        Ty_Geometry,
        Ty_Texture,
        Ty_CubeMap,
        Ty_Opaque
    };

    LoadingTaskInfoBase(Type t) : _t(t){}
    virtual ~LoadingTaskInfoBase(){}



    Type _t;
};

enum MatCompTy
{
    MatComp_Color,
    MatComp_Normal,
    MatComp_Reflective,
    MatComp_Emissive
};

struct LoadingTaskTextureInfo
        : public LoadingTaskInfoBase
{
    Meta_Class(LoadingTaskTextureInfo)

    LoadingTaskTextureInfo() :LoadingTaskInfoBase(Ty_Texture) {}
    virtual ~LoadingTaskTextureInfo() {}

    imp::Material::Ptr _dstMat;
    MatCompTy _dstMatSmpl;
    std::string _url;
    imp::Image::Ptr result;
};


struct CubeMapHandler
{
    virtual void handleCubemap(const std::vector<imp::Image::Ptr>& images) = 0;
};

struct LoadingTaskCubeMapInfo
        : public LoadingTaskInfoBase
{
    Meta_Class(LoadingTaskCubeMapInfo)

    LoadingTaskCubeMapInfo() :LoadingTaskInfoBase(Ty_CubeMap) {}
    virtual ~LoadingTaskCubeMapInfo() {}

    CubeMapHandler* _dstHandler;
    std::vector<std::string> _url;
    std::vector<imp::Image::Ptr> result;
};

struct LoadingTaskGeometryInfo
        : public LoadingTaskInfoBase
{
    Meta_Class(LoadingTaskGeometryInfo)

    LoadingTaskGeometryInfo() :LoadingTaskInfoBase(Ty_Geometry), _unique(false) {}
    virtual ~LoadingTaskGeometryInfo() {}

    imp::Node::Ptr _dstNode;
    // std::string _url;
    GeometryDescriptors::Ptr _desc;
    imp::ReflexionModel::Ptr _reflexion;
    imp::Material::Ptr _mat;

    imp::Node::Ptr _result;
    bool _unique;
    imp::Graph::Ptr _graph;
};



#endif // LOADINGTASK_H
