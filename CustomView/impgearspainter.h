#ifndef IMPGEARSPAINTER_H
#define IMPGEARSPAINTER_H

#include <QQuickWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QtConcurrent/QtConcurrent>
#include <QElapsedTimer>

#include "ProceduralScene/mainRenderer.h"

class ImpGearsPainter : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    ImpGearsPainter();
    ~ImpGearsPainter();

    void setTouch(qreal _x, qreal _y, qreal _z) {x=_x;y=_y;z=_z;}
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:
    void paint();

private:
    bool m_openglInitialized;

    qreal x, y, z;
    QSize m_viewportSize;

    QQuickWindow *m_window;
    float m_cumul_cam_rad;
    float m_cumul_cam_height;
    float m_cumul_cam_dst;

    QElapsedTimer timer;
    MainRenderer m_renderer;
};

#endif // IMPGEARSPAINTER_H
