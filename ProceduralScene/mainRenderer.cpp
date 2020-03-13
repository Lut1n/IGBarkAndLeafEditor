#include "mainRenderer.h"

#include "GuiContext/loadinginfo.h"
#include "GuiContext/treemodel.h"
#include "GuiContext/displayoptions.h"
#include "GuiContext/impgearsconfig.h"
#include "GuiContext/leafmodel.h"
#include "GuiContext/barkmodel.h"

#include <PipelineES3/GlRenderer.h>
#include <PipelineES3/GLInterface.h>

#include <QDebug>

#include <array>

MainRenderer::MainRenderer()
    : scene(nullptr)
{
   m_isLoading = false;
   m_assetRequested = false;

   impgearsRendererPlugin = imp::GlPlugin::create();
   impgearsRenderer = impgearsRendererPlugin->getRenderer();

   scene = new TreeOnIsleDemo();
}


MainRenderer::~MainRenderer()
{
    delete scene;
}

void MainRenderer::init(qreal /*t*/)
{
    if(!m_isLoading && !scene->m_initialized)
    {
        m_isLoading = true;

        imp::Vec4 viewport(0.0,0.0,512.0, 512.0);
        impgearsRenderer->enableFeature(SceneRenderer::Feature_Phong, false);
        impgearsRenderer->enableFeature(SceneRenderer::Feature_Shadow, false);
        impgearsRenderer->enableFeature(SceneRenderer::Feature_Environment, false);
        impgearsRenderer->enableFeature(SceneRenderer::Feature_Bloom, false);
        impgearsRenderer->enableFeature(SceneRenderer::Feature_SSAO, false);
        impgearsRenderer->setOuputViewport( viewport );
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Default);

        // init now
        scene->initScene();
        scene->m_initialized = true;
        qDebug() << "handle scene loading finished";
    }
}

void MainRenderer::update(qreal /*t*/)
{
    ImpGearsConfig* config = ImpGearsConfig::s_instance;
    GlRenderer::Ptr gl = std::dynamic_pointer_cast<GlRenderer>(impgearsRenderer);

    static const std::array<int,4> sampleCount = {16,4,8,16};
    static const std::array<int,7> resolutions = {256,16,32,64,128,256,512};

    int ssaoSamples = sampleCount[config->ssaoSamples()];
    int shadowSamples = sampleCount[config->shadowSamples()];
    int shadowRes = resolutions[config->shadowRes()];
    int envRes = resolutions[config->environmentRes()];

    gl->setSsaoSampleCount(ssaoSamples);
    gl->setShadowSampleCount(shadowSamples);
    gl->setShadowResolution(shadowRes);
    gl->setEnvironmentResolution(envRes);
    gl->setLightPower( config->power() );
    gl->setAmbient( config->ambient() );

    DisplayOptions* dispopt = DisplayOptions::s_instance;
    impgearsRenderer->enableFeature(SceneRenderer::Feature_Phong, dispopt->phong());
    impgearsRenderer->enableFeature(SceneRenderer::Feature_Shadow, dispopt->shadow());
    impgearsRenderer->enableFeature(SceneRenderer::Feature_Environment, dispopt->environment());
    impgearsRenderer->enableFeature(SceneRenderer::Feature_Bloom, dispopt->bloom());
    impgearsRenderer->enableFeature(SceneRenderer::Feature_SSAO, dispopt->ssao());


    int item_current = dispopt->gBufferIndex();

    if(item_current == 0)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Default);
    else if(item_current == 1)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Lighting);
    else if(item_current == 2)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Emissive);
    else if(item_current == 3)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Normals);
    else if(item_current == 4)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_ShadowMap);
    else if(item_current == 5)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Environment);
    else if(item_current == 6)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Bloom);
    else if(item_current == 7)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Color);
    else if(item_current == 8)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Reflectivity);
    else if(item_current == 9)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_Depth);
    else if(item_current == 10)
        impgearsRenderer->setOutputFrame(SceneRenderer::RenderFrame_SSAO);

}

void MainRenderer::draw(qreal elapsed)
{
    if(scene->m_initialized)
    {
        imp::Vec4 viewport(0.0,0.0,m_viewport.width(), m_viewport.height());


        if( viewport != lastViewport )
        {
            lastViewport = viewport;
            impgearsRenderer->setOuputViewport( viewport );
        }


        bool stopRendering = false;
        // Leaf edit scene
        LeafModel* editleafmodel = LeafModel::s_instance;
        if( editleafmodel && (editleafmodel->editing()||!scene->_firstLeafRendered) )
        {
            if( editleafmodel->hasChanged() )
            {
                LeafDescription desc;
                scene->m_leafRenderer->updateDesc(editleafmodel->getDesc());
                editleafmodel->makeReady();
            }
            impgearsRenderer->render( scene->m_leafEditGraph );
            scene->_firstLeafRendered = true;
            stopRendering = true;
        }

        // Bark edit scene
        BarkModel* editbarkmodel = BarkModel::s_instance;
        if( !stopRendering && editbarkmodel && (editbarkmodel->editing()||!scene->_firstBarkRendered) )
        {
            if( editbarkmodel->hasChanged() )
            {
                BarkDescription desc;
                scene->m_barkRenderer->updateDesc(editbarkmodel->getDesc());
                editbarkmodel->makeReady();
            }
            impgearsRenderer->render( scene->m_barkEditGraph );
            scene->_firstBarkRendered = true;
            stopRendering = true;
        }


        scene->updateEventData(m_cameraHorizontalAngle,m_cameraCylindricHeight, m_cameraDistance);
        scene->updateScene(elapsed);

        if( !stopRendering )
            impgearsRenderer->render( scene->graph );

        if(!m_assetRequested)
        {
            m_assetRequested = true;
            LoadingTaskQueue::instance()->request(scene->graph,scene->root,scene->reflection_water, scene->material_water, scene->waterDesc);
            LoadingTaskQueue::instance()->request(scene->graph,scene->root,scene->reflection_scene, scene->material_ground, scene->ground);
            LoadingTaskQueue::instance()->request(scene->graph,scene->root,scene->reflection_scene, scene->material_rock, scene->rdsc);
            LoadingTaskQueue::instance()->request(scene->graph,scene->treeRoot,scene->instanced_scene, scene->material_tree, scene->dsc);
            LoadingTaskQueue::instance()->request(scene->graph,scene->leafRoot,scene->instanced_scene, scene->material_leaf, scene->dscLeaf);


            LoadingTaskQueue::instance()->request(scene->material_tree, MatComp_Normal, ":/assets/textures/normal.json");
            LoadingTaskQueue::instance()->request(scene->material_rock, MatComp_Color, ":/assets/textures/rock.json");
            LoadingTaskQueue::instance()->request(scene->material_ground, MatComp_Normal, ":/assets/textures/normal.json");
            LoadingTaskQueue::instance()->request(scene->material_ground, MatComp_Color, ":/assets/textures/grass.json");
            LoadingTaskQueue::instance()->request(scene->material_water, MatComp_Normal, ":/assets/textures/normal.json");

        }

        TreeModel* treemodel = TreeModel::s_instance;
        LoadingInfo* loadinginfo = LoadingInfo::s_instance;
        bool validChangesRequested = loadinginfo && loadinginfo->validMode()==1;
        if(treemodel && (validChangesRequested || !treemodel->editing()) && treemodel->hasChanged())
        {
            TreeDescriptor::Ptr d = treemodel->getDesc();
            TreeDescriptor::Ptr ld = treemodel->getLeafDesc();
            *ld = *d; ld->_ty = TreeDescriptor::Ty_Leaf; // clone params
            LoadingTaskQueue::instance()->request(scene->graph,scene->treeRoot,scene->instanced_scene, scene->material_tree, d);
            LoadingTaskQueue::instance()->request(scene->graph,scene->leafRoot,scene->instanced_scene, scene->material_leaf, ld);

            qDebug() << "detect changes in Tree Model";
            treemodel->makeReady();
            if(validChangesRequested) loadinginfo->setValidMode(0);
        }
    }
}
