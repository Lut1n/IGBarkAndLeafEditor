#include "impgearsitem.h"

ImpGearsItem::ImpGearsItem()
    : m_renderer(nullptr)
{
    currGesture = false;
    twoPointGesture = false;
    connect(this, &QQuickItem::windowChanged, this, &ImpGearsItem::handleWindowChanged);
}

void ImpGearsItem::handleWindowChanged(QQuickWindow *win)
{
    if (win)
    {
        // this signal can be used for any preparation required before call to updatePaintNode.
        // The OpenGL context will be bound at this point.
        connect(win, &QQuickWindow::beforeSynchronizing, this, &ImpGearsItem::handleSync, Qt::DirectConnection);

        // use signal sceneGraphInvalidated : means all user resources tied to that context should be released.
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &ImpGearsItem::handleCleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void ImpGearsItem::handleSync()
{
    if (!m_renderer)
    {
        m_renderer = new ImpGearsPainter();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &ImpGearsPainter::paint, Qt::DirectConnection);
    }

    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setWindow(window());
}

void ImpGearsItem::handleCleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void ImpGearsItem::onOpenglLoaded()
{
}

float length(QPointF p)
{
    return std::sqrtf(QPointF::dotProduct(p,p));
}

void ImpGearsItem::touchEvent(QTouchEvent *event)
{
    if(event->type() == QEvent::TouchBegin) currGesture = true;
    else if(event->type() == QEvent::TouchEnd) currGesture = false;

    const QList<QTouchEvent::TouchPoint>& points = event->touchPoints();
    int count = points.size();

    QPointF delta;
    float scale = 0.0;

    if(count >= 2)
    {
        QPointF touchP1 = points[0].scenePos();
        QPointF touchP2 = points[1].scenePos();

        if(twoPointGesture && currGesture && event->type() != QEvent::TouchBegin)
        {
            QPointF lastDelta = m_last_touchPoint1 - m_last_touchPoint2;
            QPointF currDelta = touchP1 - touchP2;
            scale = length(currDelta) - length(lastDelta);
        }

        m_last_touchPoint2 = touchP2;
    }

    if(count >= 1)
    {
        QPointF touchP = points[0].scenePos();

        if(!twoPointGesture && currGesture && event->type() != QEvent::TouchBegin)
            delta = touchP - m_last_touchPoint1;

        m_last_touchPoint1 = touchP;
    }
    twoPointGesture = (count >= 2);

    m_renderer->setTouch(delta.x(),delta.y(),scale);
}
