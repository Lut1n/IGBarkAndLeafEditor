#ifndef TREEONISLEDEMO_H
#define TREEONISLEDEMO_H

#include <Graphics/ImageOperation.h>
#include <SceneGraph/Graph.h>
#include <SceneGraph/Camera.h>
#include <SceneGraph/GeoNode.h>
#include <SceneGraph/QuadNode.h>
#include <SceneGraph/RenderPass.h>
#include <Descriptors/ImageIO.h>
#include <Descriptors/FileInfo.h>
#include <Descriptors/JsonImageOp.h>
#include <SceneGraph/Graph.h>
#include <SceneGraph/Camera.h>
#include <SceneGraph/LightNode.h>
#include <SceneGraph/GeoNode.h>
#include <SceneGraph/QuadNode.h>
#include <SceneGraph/RenderPass.h>
#include <SceneGraph/RenderToSamplerNode.h>
#include <Descriptors/ImageIO.h>
#include <Descriptors/FileInfo.h>
#include <Descriptors/JsonImageOp.h>


using namespace imp;

#include "DataModel/rockdescriptor.h"
#include "DataModel/grounddescriptor.h"

#include "DataControl/loadingtaskqueue.h"

#include "ProceduralScene/leafrenderer.h"
#include "ProceduralScene/barkrenderer.h"
#include "ProceduralScene/proceduralskybox.h"





class TreeOnIsleDemo
{
public:
    TreeOnIsleDemo();

    virtual ~TreeOnIsleDemo();

    void initScene();
    void updateEventData(float cam_rad, float cam_height, float cam_dst);
    void updateScene(float elapsedTime);

    void buildLeafEditGraph();
    void buildBarkEditGraph();

    imp::Graph::Ptr graph;

    imp::LightNode::Ptr light;
    imp::GeoNode::Ptr pointNode;
    imp::Camera::Ptr camera;

    imp::ReflexionModel::Ptr instanced_scene;
    imp::ReflexionModel::Ptr reflection_scene;
    imp::ReflexionModel::Ptr reflection_water;
    ProceduralSkyBox::Ptr sky;
    TreeDescriptor::Ptr dsc,dscLeaf, grassDesc;
    TreeDescriptor dscEdited;
    RockDescriptor::Ptr rdsc;
    GroundDescriptor::Ptr ground;
    GroundDescriptor::Ptr waterDesc;

    Node::Ptr root;
    Node::Ptr treeRoot;
    Node::Ptr leafRoot;
    GeoNode::Ptr treeNode;
    GeoNode::Ptr leafNode;
    GeoNode::Ptr rockNode;
    GeoNode::Ptr waterNode;

    Material::Ptr material_tree;
    Material::Ptr material_leaf;
    Material::Ptr material_rock;
    Material::Ptr material_ground;
    Material::Ptr material_water;

    std::vector<ImageSampler::Ptr> accessiblesSamplers;

    imp::Graph::Ptr m_leafEditGraph;
    LeafRenderer::Ptr m_leafRenderer;

    imp::Graph::Ptr m_barkEditGraph;
    BarkRenderer::Ptr m_barkRenderer;

    bool _firstLeafRendered;
    bool _firstBarkRendered;

    bool m_initialized;

    float m_cam_rad;
    float m_cam_height;
    float m_cam_dst;
};

#endif // TREEONISLEDEMO_H
