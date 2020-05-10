// qt
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QDebug>

// local
#include "polygonresizehandle.h"
#include "polygonitem.h"



PolygonResizeHandle::HandleItem::HandleItem(int index, const QPointF& pos, PolygonResizeHandle* parent) :
    QGraphicsEllipseItem(-4, -4, 10, 10, parent),
    m_parent(parent),
    m_index(index)
{
    setPos(pos);
    setBrush(QBrush(Qt::red));
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
}



int PolygonResizeHandle::HandleItem::index() const
{
    return m_index;
}



QVariant PolygonResizeHandle::HandleItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    QVariant retVal = value;

    if(change == ItemPositionHasChanged)
    {
        m_parent->resize(m_index, value.toPointF());
    }

    return retVal;
}



PolygonResizeHandle::PolygonResizeHandle(QGraphicsItem* parent) :
    QGraphicsItem(parent)
{
    m_boundingRect = parent->boundingRect();

    m_parent = qgraphicsitem_cast<PolygonItem*>(parent);

    // get the path from the polygon
    QPainterPath polyPath = m_parent->path();

    // get the vertices from the path
    int vertices = polyPath.elementCount();

    // create as many handles as vertices
    for(int i = 0; i < vertices; i++)
    {
        // get the pos of the vertex in
        const QPointF& pos = QPointF(polyPath.elementAt(i).x, polyPath.elementAt(i).y);

        // save the handles in a container
        m_handles.push_back(new HandleItem(i, pos, this));
    }
}



QRectF PolygonResizeHandle::boundingRect() const
{
    return m_boundingRect;
}



void PolygonResizeHandle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(widget);
    Q_UNUSED(option);
}



void PolygonResizeHandle::resize(int index, const QPointF& newPos)
{
    QPainterPath tempPath = m_parent->path();
    tempPath.setElementPositionAt(index, newPos.x(), newPos.y());
    m_parent->updatePath(tempPath);

    qDebug() << "Item position changed";
}
