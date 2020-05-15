// qt
#include <QtMath>

// local
#include "customview.h"

CustomView::CustomView(QWidget *parent)
    : QGraphicsView(parent)
    , m_zoomPercent(100)
{
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
}

void CustomView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        QPointF oldPos = mapToScene(event->pos());

        setZoom(m_zoomPercent + (event->delta() > 0 ? zoomStepPercent() : -zoomStepPercent()));

        QPointF newPos = mapToScene(event->pos());
        QPointF delta = newPos - oldPos;

        translate(delta.x(), delta.y());

        Q_EMIT zoomChanged(m_zoomPercent);

        return;
    }

    QGraphicsView::wheelEvent(event);
}

qreal CustomView::minZoomLevel() const
{
    return 0.1;
}

qreal CustomView::maxZoomLevel() const
{
    return 1000;
}

qreal CustomView::zoomStepPercent() const
{
    return 5;
}

void CustomView::setZoom(double percent)
{
    percent = qBound(minZoomLevel(), percent, maxZoomLevel());
    if (qFuzzyCompare(m_zoomPercent, percent))
        return;

    m_zoomPercent = percent;

    resetTransform();
    scale(m_zoomPercent / 100.0, m_zoomPercent / 100.0);

    Q_EMIT zoomChanged(m_zoomPercent);
}
