#ifndef IMPGEARSITEM_H
#define IMPGEARSITEM_H

#include <QQuickItem>
#include <QGestureEvent>
#include "CustomView/impgearspainter.h"
#include "ProceduralScene/mainRenderer.h"

class ImpGearsItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    ImpGearsItem();

    qreal t() const { return m_t; }
    void setT(qreal t) {m_t=t;tChanged();forceRepaint();}

    void forceRepaint() { if(window()) window()->update(); }

protected:
    // Event handler reimplemented from super class for handling touch point events
    void touchEvent(QTouchEvent *event) override;

signals:
    void tChanged();

private slots:
    void handleSync();
    void handleCleanup();
    void handleWindowChanged(QQuickWindow *win);
    void onOpenglLoaded();

private:
    qreal m_t;
    ImpGearsPainter *m_renderer;

    std::vector<QPointF> gui_touchPoints;

    int lastTouchCount;
    QPointF m_last_touchPoint1;
    QPointF m_last_touchPoint2;
    QPointF m_start_touchPoint1;
    QPointF m_start_touchPoint2;
    bool currGesture;
    bool twoPointGesture;
};

#endif // IMPGEARSITEM_H
