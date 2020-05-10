// qt
#include <QPainter>

// local
#include "polygonitem.h"



PolygonItem::PolygonItem(const QPainterPath& path, QGraphicsItem* parent) :
    QGraphicsPathItem(path, parent),
    m_path(path)
{
    m_boundingRect = QRectF(0, 0, m_path.boundingRect().width(), m_path.boundingRect().height());

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemSendsGeometryChanges);

    // path needs to set otherwise moving or selecting doesn't work
    setPath(m_path);
}



void PolygonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QBrush(QColor(128, 128, 255, 75), Qt::SolidPattern));
    painter->drawPath(m_path);
}



void PolygonItem::updatePath(const QPainterPath& path)
{
    m_path = path;

    m_boundingRect = m_path.boundingRect();

    // path needs to set otherwise moving or selecting doesn't work
    setPath(m_path);
    update();
}
