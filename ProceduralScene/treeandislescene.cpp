#include "ProceduralScene/treeandislescene.h"
#include "DataControl/texturegenerator.h"
#include <DataControl/loadingtaskqueue.h>

#include "GuiContext/loadinginfo.h"
#include "GuiContext/treemodel.h"



TreeOnIsleDemo::TreeOnIsleDemo()
{
    _firstLeafRendered = false;
    _firstBarkRendered = false;
    m_initialized = false;
    m_cam_height = 0.0;
}

TreeOnIsleDemo::~TreeOnIsleDemo()
{
}

void TreeOnIsleDemo::initScene()
{
    LoadingInfo::s_instance->signalProgress(5.0, "init scene");

    if(m_initialized) return;

    buildLeafEditGraph();
    buildBarkEditGraph();

    dsc = TreeDescriptor::create();
    dscLeaf = TreeDescriptor::create();
    grassDesc = TreeDescriptor::create();
    rdsc = RockDescriptor::create();
    ground = GroundDescriptor::create();
    waterDesc = GroundDescriptor::create();


    grassDesc->_divisionCount = 1;
    grassDesc->_leafCount = 0;
    grassDesc->_nodeCount = 3;
    grassDesc->_baseRadius = 0.1;
    grassDesc->_baseHeight = 1.0;
    grassDesc->_theta = 0.2;


    waterDesc->_scale = Vec3(50.0,1.0,50.0);
    waterDesc->_pos = Vec3(0.0,-4.0,0.0);
    waterDesc->_texCoordDensity = 64.0;
    waterDesc->_sphericalN = 0.0;
    waterDesc->_transparent = true;

    dsc->_ty = GeometryDescriptors::Ty_Tree;
    dscLeaf->_ty = GeometryDescriptors::Ty_Leaf;


    ImageSampler::Ptr bark_tex = m_barkRenderer->texture();
    bark_tex->setWrapping(ImageSampler::Wrapping_Repeat);

    ImageSampler::Ptr leaf_tex = m_leafRenderer->texture();

    ImageSampler::Ptr dft_transparent = ImageSampler::create(8, 8, 4, Vec4(1.0,1.0,1.0,0.2));
    ImageSampler::Ptr dft_white = ImageSampler::create(8, 8, 3, Vec4(1.0));
    ImageSampler::Ptr dft_grey = ImageSampler::create(8, 8, 3, Vec4(0.1));


    // impgears stuff
    Graph::Ptr graph = Graph::create();
    root = Node::create();
    treeRoot = Node::create();
    leafRoot = Node::create();

    Camera::Ptr camera = Camera::create(Vec3(0.0,1.0,0.0));
    camera->setPosition(Vec3(10.0f, 10.0f, 10.0f));
    camera->setTarget(Vec3(0.0f, 0.0f, 0.0f));

    reflection_scene = imp::ReflexionModel::create(
            imp::ReflexionModel::Lighting_Phong,
            imp::ReflexionModel::Texturing_Samplers_CNE,
            imp::ReflexionModel::MRT_2_Col_Emi, "scene reflection");


    QString instancedVert = "[no data]";
    QString url = ":/assets/glsl/instanced.vert";
    QFile vert(url);
    if ( vert.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        instancedVert= vert.readAll();
    }

    instanced_scene = imp::ReflexionModel::create(
                imp::ReflexionModel::Lighting_Phong,
                imp::ReflexionModel::Texturing_Samplers_CNE,
                imp::ReflexionModel::MRT_2_Col_Emi, "instanced_scene");
    instanced_scene->_vertCode = instancedVert.toStdString();

    QString waterVert = "[no data]";
    QString url2 = ":/assets/glsl/water.vert";
    QFile vert2(url2);
    if ( vert2.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        waterVert= vert2.readAll();
    }

    reflection_water = imp::ReflexionModel::create(
            imp::ReflexionModel::Lighting_Phong,
            imp::ReflexionModel::Texturing_Samplers_CNE,
            imp::ReflexionModel::MRT_2_Col_Emi, "water reflection");
    reflection_water->_vertCode = waterVert.toStdString();


    material_tree = Material::create(Vec3(1.0), 64.0);
    material_tree->_baseColor = bark_tex;

    material_leaf = Material::create(Vec3(1.0), 32.0);
    material_leaf->_baseColor = leaf_tex;

    material_rock = Material::create(Vec3(0.5,0.7,0.5), 32.0);
    material_rock->_reflectivity = dft_grey;

    material_ground = Material::create(Vec3(0.3,1.0,0.4), 32.0);
    material_water = Material::create(Vec3(0.1,0.4,1.0), 32.0);
    material_water->_reflectivity = dft_white;
    material_water->_baseColor = dft_transparent;

    Material::Ptr light_material = Material::create(Vec3(1.0), 16.0);
    light_material->_emissive = dft_white;


    LightNode::Ptr light = LightNode::create(Vec3(1.0),10.f);

    Geometry::Ptr pointGeo = Geometry::create();
    *pointGeo = Geometry::sphere(16, 0.4);
    pointGeo->setPrimitive(Geometry::Primitive_Triangles);
    Geometry::intoCCW(*pointGeo, true);
    pointGeo->generateColors(Vec3(1.0,1.0,1.0));
    pointGeo->generateNormals();
    pointGeo->generateTexCoords(Geometry::TexGenMode_Spheric);
    pointGeo->interpolateNormals();

    GeoNode::Ptr pointNode = GeoNode::create(pointGeo, false);
    pointNode->setPosition(Vec3(0.0,2,0.0));
    pointNode->setReflexion(reflection_scene);
    pointNode->setMaterial(light_material);

    GeoNode::Ptr test = GeoNode::create(pointGeo, false);
    test->setPosition(Vec3(0.0,0.0,0.0));
    test->setScale(Vec3(10.0));
    test->setReflexion(reflection_scene);
    test->setMaterial(material_rock);

    sky = ProceduralSkyBox::create();

    root->addNode(sky);
    root->addNode(light);
    root->addNode(camera);

    root->addNode(pointNode);
    root->addNode(treeRoot);
    root->addNode(leafRoot);

    graph->setRoot(root);

    LoadingInfo::s_instance->signalProgress(100.0, "graph ok");

    RenderPass::Ptr env = RenderPass::create();
    env->enablePass(RenderPass::Pass_EnvironmentMapping);

    sky->getState()->setRenderPass(env);
    pointNode->getState()->setRenderPass(env);

    graph->setClearColor(Vec4(0.0,0.0,1.0,1.0));
    Vec4 viewport(0.0,0.0,512.0,512.0);
    graph->getInitState()->setViewport( viewport );


    this->graph = graph;
    this->light =light;
    this->pointNode = pointNode;
    this->camera = camera;

    accessiblesSamplers.resize(4);
    accessiblesSamplers[2] = material_leaf->_baseColor;
    accessiblesSamplers[3] = material_tree->_baseColor;

    m_initialized = true;

    // this->graph->notifyChanges();

    LoadingInfo::s_instance->signalProgress(100.0, "loading finished");
}


void TreeOnIsleDemo::buildLeafEditGraph()
{
    Vec4 viewport(0.0,0.0,512.0,512.0);

    m_leafEditGraph = Graph::create();
    m_leafRenderer = LeafRenderer::create();
    m_leafEditGraph->setRoot(m_leafRenderer);
    m_leafEditGraph->getInitState()->setViewport( viewport );
    m_leafEditGraph->getInitState()->setOrthographicProjection(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
    m_leafEditGraph->setClearColor(Vec4(0.0,0.0,1.0,1.0));

}

void TreeOnIsleDemo::buildBarkEditGraph()
{
    Vec4 viewport(0.0,0.0,512.0,512.0);

    m_barkEditGraph = Graph::create();
    m_barkRenderer = BarkRenderer::create();
    m_barkEditGraph->setRoot(m_barkRenderer);
    m_barkEditGraph->getInitState()->setViewport( viewport );
    m_barkEditGraph->getInitState()->setOrthographicProjection(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
    m_barkEditGraph->setClearColor(Vec4(0.0,0.0,1.0,1.0));

}

void TreeOnIsleDemo::updateEventData(float cam_rad, float cam_height, float cam_dst)
{
    m_cam_rad = cam_rad;
    m_cam_height = cam_height;
    m_cam_dst = cam_dst;
}

void TreeOnIsleDemo::updateScene(float elapsedTime)
{
    if(m_initialized == false) return;

    // main scene
    float camera_rad = 0.0 + m_cam_rad;
    float camera_rad_y = -0.07;
    float camera_dst = 35.0 + m_cam_dst;
    float camera_height = dsc->_baseHeight; // + clamp(m_cam_height,0.f,100.f);


    const float PI = 3.141592;
    Vec3 lp(cos(elapsedTime)*20.0,8.0,sin(elapsedTime)*20.0);
    light->setPosition(lp);
    pointNode->setPosition(lp);
    Vec3 lp2(cos(camera_rad*PI ),sin(camera_rad_y*PI*0.5),sin(camera_rad*PI));
    camera->setPosition(Vec3(0.0,camera_height,0.0) + lp2*Vec3(camera_dst));
    camera->setTarget(Vec3(0.0, camera_height, 0.0));

    if(waterNode) waterNode->getState()->setUniform("u_time", (float)elapsedTime);
    else root->getState()->setUniform("u_time", (float)elapsedTime);
    LoadingInfo::s_instance->updateTime((float)elapsedTime);

    TreeModel* tree = TreeModel::s_instance;
    if(tree)
    {
        material_leaf->_color = tree->getDesc()->_leafColor;
        material_tree->_color = tree->getDesc()->_barkColor;
    }
}
