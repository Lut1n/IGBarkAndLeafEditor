#include "proceduralskybox.h"


SkyGen::SkyGen()
{
    skyGeo = GeoNode::create(Geometry::create(),false);

    *skyGeo->_geo = Geometry::cube();
    Geometry::intoCCW(*skyGeo->_geo, false);
    skyGeo->_geo->scale(Vec3(50.0));
    skyGeo->_geo->generateNormals(Geometry::NormalGenMode_PerFace);
    skyGeo->_geo->generateColors(Vec3(1.0));
    skyGeo->_geo->generateTexCoords();

    QString skyVert = "[no data]";
    QString url = ":/assets/glsl/sky.vert";
    QFile frag(url);
    if ( frag.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        skyVert= frag.readAll();
    }
    QString skyFrag = "[no data]";
    QString url2 = ":/assets/glsl/sky_gen.frag";
    QFile frag2(url2);
    if ( frag2.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        skyFrag= frag2.readAll();
    }

    _shaderGen = ReflexionModel::create(
                ReflexionModel::Lighting_Customized,
                ReflexionModel::Texturing_Samplers_CNE,
                ReflexionModel::MRT_2_Col_Emi);
    _shaderGen->_fragCode_lighting = skyFrag.toStdString();
    _shaderGen->_vertCode = skyVert.toStdString();

    skyGeo->setReflexion(_shaderGen);

    Graph::Ptr graph = Graph::create();
    graph->setRoot(skyGeo);
    graph->getInitState()->setViewport( Vec4(0.0,0.0,512.0,512.0) );
    graph->setClearColor(Vec4(1.0,1.0,1.0,1.0));

    setScene(graph);
}



void ProceduralSkyBox::handleCubemap(const std::vector<imp::Image::Ptr>& images)
{
    _cubemap = CubeMapSampler::create(images);
    getState()->setUniform("u_input_cube", _cubemap, 3);
    qDebug() << "handle cubemap";
}

ProceduralSkyBox::ProceduralSkyBox() : GeoNode(Geometry::create(), false)
{
    *_geo = Geometry::sphere(8,50.0);
    Geometry::intoCCW(*_geo, false);
    _geo->generateNormals(Geometry::NormalGenMode_PerFace);
    _geo->generateColors(Vec3(1.0));
    _geo->generateTexCoords();

    _cubemap = CubeMapSampler::create(512, 512, 4, Vec4(1.0) );
    _subrender = SkyGen::create();
    _subrender->setCubeMap(_cubemap);

    addNode(_subrender);

    QString skyVert = "[no data]";
    QString url = ":/assets/glsl/sky.vert";
    QFile frag(url);
    if ( frag.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        skyVert= frag.readAll();
    }
    QString skyFrag = "[no data]";
    QString url2 = ":/assets/glsl/sky_disp.frag";
    QFile frag2(url2);
    if ( frag2.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        skyFrag= frag2.readAll();
    }

    _shader = ReflexionModel::create(
                ReflexionModel::Lighting_Customized,
                ReflexionModel::Texturing_Samplers_CNE,
                ReflexionModel::MRT_2_Col_Emi);
    _shader->_fragCode_lighting = skyFrag.toStdString();
    _shader->_vertCode = skyVert.toStdString();

    setReflexion(_shader);
    float reversed = 0.0;
    getState()->setUniform("u_reversed", reversed);
    getState()->setUniform("u_input_cube", _cubemap, 3);
}
