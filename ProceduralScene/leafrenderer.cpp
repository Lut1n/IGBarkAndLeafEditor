#include "leafrenderer.h"


LeafRenderer::LeafRenderer()
{
    Graph::Ptr graph = Graph::create();
    graph->getInitState()->setViewport( Vec4(0.0,0.0,512.0,512.0) );
    graph->getInitState()->setOrthographicProjection(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
    graph->setClearColor(Vec4(0.0,0.0,1.0,1.0));

    QuadNode::Ptr quad = QuadNode::create();
    u_branchDepth = Uniform::create("iDepth",Uniform::Type_1i);
    u_branchCount = Uniform::create("iBranchs",Uniform::Type_1i);
    u_texOctave = Uniform::create("iTexOctave",Uniform::Type_1i);
    u_texPersist = Uniform::create("iTexPersist",Uniform::Type_1f);
    u_texFreq = Uniform::create("iTexFreq",Uniform::Type_1f);
    u_ShapeFq1 = Uniform::create("iShapeFq1",Uniform::Type_1f);
    u_ShapeFq2 = Uniform::create("iShapeFq2",Uniform::Type_1f);
    u_ShapeMin = Uniform::create("iShapeMin",Uniform::Type_1f);
    u_ShapeMax = Uniform::create("iShapeMax",Uniform::Type_1f);
    u_Tilt = Uniform::create("iTilt",Uniform::Type_1f);
    u_Len = Uniform::create("iLen",Uniform::Type_1f);
    u_LenFct = Uniform::create("iLenFct",Uniform::Type_1f);
    u_Thickness = Uniform::create("iThickness",Uniform::Type_1f);

    quad->getState()->setUniform(u_branchDepth);
    quad->getState()->setUniform(u_branchCount);
    quad->getState()->setUniform(u_texOctave);
    quad->getState()->setUniform(u_texPersist);
    quad->getState()->setUniform(u_texFreq);
    quad->getState()->setUniform(u_ShapeFq1);
    quad->getState()->setUniform(u_ShapeFq2);
    quad->getState()->setUniform(u_ShapeMin);
    quad->getState()->setUniform(u_ShapeMax);
    quad->getState()->setUniform(u_Tilt);
    quad->getState()->setUniform(u_Len);
    quad->getState()->setUniform(u_LenFct);
    quad->getState()->setUniform(u_Thickness);

    QString data = "[no data]";
    QString url = ":/assets/glsl/leaf.frag";
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
    ImageSampler::Ptr genResult = ImageSampler::create(512, 512, 4, Vec4(0.0,1.0,0.0,1.0));
    genResult->setWrapping(imp::ImageSampler::Wrapping_Clamp);
    genResult->setFiltering(imp::ImageSampler::Filtering_Linear);
    genResult->_mipmap = true;
    setTexture(genResult);

    Material::Ptr material = Material::create(Vec3(1.0), 32.0);
    material->_baseColor = genResult;
    QuadNode::Ptr displayquad = QuadNode::create();
    displayquad->setMaterial(material);
    ReflexionModel::Ptr displayshader = ReflexionModel::create(ReflexionModel::Lighting_None, ReflexionModel::Texturing_Samplers_CN, ReflexionModel::MRT_1_Col, "glsl texture display");
    displayquad->setReflexion(displayshader);

    displayquad->setScale(Vec3(0.5,0.5,1.0));
    displayquad->setPosition(Vec3(0.0,0.4,0.0));

    addNode(displayquad);
}

void LeafRenderer::updateDesc(const LeafDescription& ld)
{
    desc = ld;
    u_branchDepth->set(desc.branchDepth);
    u_branchCount->set(desc.branchCount);
    u_texOctave->set(desc.texOctave);
    u_texPersist->set(desc.texPersist);
    u_texFreq->set(desc.texFreq);
    u_ShapeFq1->set(desc.shapeFq1);
    u_ShapeFq2->set(desc.shapeFq2);
    u_ShapeMin->set(desc.shapeMin);
    u_ShapeMax->set(desc.shapeMax);
    u_Tilt->set(desc.tilt);
    u_Len->set(desc.length);
    u_LenFct->set(desc.lengthFct);
    u_Thickness->set(desc.thickness);
    makeDirty();
    // if(texture())texture()->_needUpdate = true;
}
