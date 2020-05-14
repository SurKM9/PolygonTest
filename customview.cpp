// qt
#include <QtMath>

// local
#include "customview.h"



CustomView::CustomView(QWidget* parent) :
    QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
}



void CustomView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        // Do a wheel-based zoom about the cursor position
        double angle = event->angleDelta().y();
        double factor = qPow(1.0015, angle);

        auto targetViewportPos = event->pos();
        auto targetScenePos = mapToScene(event->pos());

        scale(factor, factor);
        centerOn(targetScenePos);
        QPointF deltaViewportPos = targetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
        QPointF viewportCenter = mapFromScene(targetScenePos) - deltaViewportPos;
        centerOn(mapToScene(viewportCenter.toPoint()));

        return;
    }

    QGraphicsView::wheelEvent(event);
}
