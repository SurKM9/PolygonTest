#include "customscene.h"
#include "polygonitem.h"



CustomScene::CustomScene(QObject* parent) :
    QGraphicsScene(parent),
    m_polygon(nullptr)
{
}



void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_origPoint = event->scenePos();

    if(event->button() == Qt::LeftButton)
    {
        if(m_polygon == nullptr)
        {
            m_path.moveTo(m_origPoint);
            m_polygon = new PolygonItem(m_path);
            addItem(m_polygon);
        }
        else
        {
            m_path.lineTo(m_origPoint);
            m_polygon->updatePath(m_path);
        }
    }

    if(event->button() == Qt::RightButton && m_polygon != nullptr)
    {
        m_path.closeSubpath();
        m_polygon = nullptr;
    }

    update();

    QGraphicsScene::mousePressEvent(event);
}



void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_polygon != nullptr)
    {
        m_path.lineTo(event->scenePos());
        m_polygon->updatePath(m_path);
    }

    update();

    QGraphicsScene::mouseMoveEvent(event);
}



void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}
