// qt
#include <QBrush>
#include <QPen>
#include <QPainter>

// local
#include "polygonresizehandle.h"
#include "polygonitem.h"



PolygonResizeHandle::PolygonResizeHandle(QGraphicsItem* parent) :
    QGraphicsEllipseItem(parent)
{
    m_boundingRect = QRectF(0, 0, 5, 5);

    m_parent = qgraphicsitem_cast<PolygonItem*>(parent);
}



QRectF PolygonResizeHandle::boundingRect() const
{
    return m_boundingRect;
}



void PolygonResizeHandle::setIndex(uint32_t index)
{
    m_index = index;
}



uint32_t PolygonResizeHandle::index() const
{
    return m_index;
}



void PolygonResizeHandle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter->drawRect(m_boundingRect);
}



void PolygonResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //    if(event->button() == Qt::LeftButton)
    //    {

    //    }

    QGraphicsEllipseItem::mousePressEvent(event);
}



void PolygonResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        const QPointF& point = event->scenePos();

        QPainterPath tempPath = m_parent->path();
        tempPath.setElementPositionAt(m_index, point.x(), point.y());
        m_parent->updatePath(tempPath);
    }

    QGraphicsEllipseItem::mouseMoveEvent(event);
}



void PolygonResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
