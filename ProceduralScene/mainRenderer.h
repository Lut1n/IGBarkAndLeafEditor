#ifndef MAINSCENE_H
#define MAINSCENE_H


#include "ProceduralScene/treeandislescene.h"
#include <Plugins/RenderPlugin.h>
#include <QOpenGLFunctions>
#include <QSize>

class MainRenderer : protected QOpenGLFunctions
{
public:
    MainRenderer();
    ~MainRenderer();

    virtual void init(qreal t);
    virtual void update(qreal t);
    virtual void draw(qreal t);

    TreeOnIsleDemo* getScene() {return scene;}

    void setViewport(QSize vp){m_viewport=vp;}
    void setCamera(qreal horizontalAngle, qreal cylheight, qreal distance)
    {
        m_cameraHorizontalAngle=horizontalAngle;
        m_cameraCylindricHeight = cylheight;
        m_cameraDistance=distance;
    }

protected:

    LoadingTaskQueue loadingQueue;
    TextureCache texcache;

    qreal m_cameraHorizontalAngle;
    qreal m_cameraCylindricHeight;
    qreal m_cameraDistance;

    imp::Vec4 lastViewport;
    QSize m_viewport;

    imp::RenderPlugin::Ptr impgearsRendererPlugin;
    imp::SceneRenderer::Ptr impgearsRenderer;

    TreeOnIsleDemo* scene;

    bool m_isLoading;
    bool m_assetRequested;
};

#endif // MAINSCENE_H
