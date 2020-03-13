#include "impgearspainter.h"
#include "DataControl/loadingtaskqueue.h"

#include <QOpenGLExtraFunctions>
#include <GLES3/gl3.h>

#include "GuiContext/loadinginfo.h"
#include "GuiContext/barkmodel.h"
#include "GuiContext/treemodel.h"

#include "ProceduralScene/mainRenderer.h"


ImpGearsPainter::ImpGearsPainter()
{
    m_cumul_cam_rad = 0.0;
    m_cumul_cam_dst = 1.0;
    m_openglInitialized = false;

    LoadingTaskQueue::instance();
}

ImpGearsPainter::~ImpGearsPainter()
{
}


void ImpGearsPainter::paint()
{
    m_window->resetOpenGLState();

    if(!m_openglInitialized)
    {
        initializeOpenGLFunctions();
        LoadingInfo::s_instance->signalProgress(3.0, "OpenGL initialized");
        timer.start();

        m_openglInitialized = true;
    }

    float elapsed = timer.elapsed()*1e-3;
    m_cumul_cam_rad += x * 3.14/m_viewportSize.width();
    m_cumul_cam_height += y * 100.0/m_viewportSize.width();
    m_cumul_cam_dst -= z * 100.0/m_viewportSize.width();

    m_renderer.setViewport(m_viewportSize);
    m_renderer.init(elapsed);
    m_renderer.setCamera(m_cumul_cam_rad,m_cumul_cam_height,m_cumul_cam_dst);
    m_renderer.update(elapsed);
    m_renderer.draw(elapsed);

    m_window->resetOpenGLState();
}

