#include "customscene.h"
#include "polygonitem.h"



CustomScene::CustomScene(QObject* parent) :
    QGraphicsScene(parent),
    m_polygon(nullptr)
{
}



void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& m_origPoint = event->scenePos();

    if(event->button() == Qt::LeftButton)
    {
        if(m_polygon == nullptr)
        {
            m_points.clear();
            m_polygon = new PolygonItem({});
            addItem(m_polygon);
            storePoint(m_origPoint);
        }

        storePoint(m_origPoint);
    }

    if(event->button() == Qt::RightButton && m_polygon != nullptr)
    {
        m_polygon = nullptr;
    }

    update();

    QGraphicsScene::mousePressEvent(event);
}



void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_polygon != nullptr)
    {
        previewPoint(event->scenePos());
    }

    update();

    QGraphicsScene::mouseMoveEvent(event);
}



void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void CustomScene::storePoint(const QPointF& point)
{
    m_points.append(point);
    updatePolygon();
}

void CustomScene::previewPoint(const QPointF& point)
{
    if(m_points.size()>1)
    {
        m_points.replace(m_points.size()-1, point);
        updatePolygon();
    }
}

void CustomScene::updatePolygon()
{
    QPainterPath path;
    path.moveTo(m_points.first());
    for(const auto& point : m_points)
        path.lineTo(point);
    path.closeSubpath();

    if(m_polygon)
        m_polygon->updatePath(path);
}

