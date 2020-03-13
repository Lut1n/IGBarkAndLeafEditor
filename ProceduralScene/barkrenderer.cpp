#include "barkrenderer.h"


BarkRenderer::BarkRenderer()
{
    Graph::Ptr graph = Graph::create();
    graph->getInitState()->setViewport( Vec4(0.0,0.0,512.0,512.0) );
    graph->getInitState()->setOrthographicProjection(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
    graph->setClearColor(Vec4(0.0,0.0,1.0,1.0));

    QuadNode::Ptr quad = QuadNode::create();
    u_octaves = Uniform::create("iOctaves",Uniform::Type_1i);
    u_freq = Uniform::create("iFreq",Uniform::Type_1f);
    u_persist = Uniform::create("iPersist",Uniform::Type_1f);
    u_scaleX = Uniform::create("iScaleX",Uniform::Type_1f);
    u_scaleY = Uniform::create("iScaleY",Uniform::Type_1f);

    quad->getState()->setUniform(u_octaves);
    quad->getState()->setUniform(u_freq);
    quad->getState()->setUniform(u_persist);
    quad->getState()->setUniform(u_scaleX);
    quad->getState()->setUniform(u_scaleY);

    QString data = "[no data]";
    QString url = ":/assets/glsl/bark.frag";
    QFile frag(url);
    if ( frag.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        data= frag.readAll();
    }

    ReflexionModel::Ptr shader = ReflexionModel::create(ReflexionModel::Lighting_Phong,
                                                        ReflexionModel::Texturing_Customized,
                                                        ReflexionModel::MRT_2_Col_Emi,
                                                        "glsl gen texture");
    shader->_fragCode_texturing = data.toStdString();

    quad->setReflexion(shader);
    graph->setRoot(quad);

    setScene(graph);
    updateDesc(desc);






    // display result (or not)
    ImageSampler::Ptr genResult = ImageSampler::create(512, 512, 4, Vec4(0.5,0.5,0.0,1.0));
    genResult->setWrapping(imp::ImageSampler::Wrapping_Repeat);
    genResult->setFiltering(imp::ImageSampler::Filtering_Linear);
    genResult->_mipmap = true;
    setTexture(genResult);

    Material::Ptr material = Material::create(Vec3(1.0), 32.0);
    material->_baseColor = genResult;
    QuadNode::Ptr displayquad = QuadNode::create();
    displayquad->setMaterial(material);
    ReflexionModel::Ptr displayshader = ReflexionModel::create(ReflexionModel::Lighting_None, ReflexionModel::Texturing_Samplers_CN, ReflexionModel::MRT_1_Col, "glsl texture display");
    displayquad->setReflexion(displayshader);
    addNode(displayquad);
}

void BarkRenderer::updateDesc(const BarkDescription& ld)
{
    desc = ld;
    u_octaves->set(desc.m_octaves);
    u_freq->set(desc.m_freq);
    u_persist->set(desc.m_persist);
    u_scaleX->set(desc.m_scaleX);
    u_scaleY->set(desc.m_scaleY);
    makeDirty();
    // if(texture())texture()->_needUpdate = true;
}
