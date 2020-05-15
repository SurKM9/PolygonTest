// qt
#include <QDebug>
#include <QGraphicsView>

// local
#include "customscene.h"
#include "polygonitem.h"
#include "rectangleitem.h"

CustomScene::CustomScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_polygon(nullptr)
    , m_rectangle(nullptr)
{
}



void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_origPoint = event->scenePos();

    views().at(0)->setDragMode(QGraphicsView::NoDrag);

    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)
    {
        if (m_polygon == nullptr)
        {
            m_polygon = new PolygonItem({});
            addItem(m_polygon);
            storePoint(m_origPoint);
        }

        storePoint(m_origPoint);
        return;
    }

    if (event->button() == Qt::RightButton && m_polygon != nullptr)
    {
        m_polygon = nullptr;
    }

    update();

    QGraphicsScene::mousePressEvent(event);
}



void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier)
    {
        if(m_rectangle == nullptr)
        {
            // create a rectangle
            m_rectangle = new RectangleItem({});

            // draw rectangle on the scene
            addItem(m_rectangle);

            // set the pos
            m_rectangle->setPos(m_origPoint);
        }

        m_rectangle->updateRect(QRectF(0, 0, event->scenePos().x() - m_origPoint.x(),
                                       event->scenePos().y() - m_origPoint.y()));
    }

    if (m_polygon != nullptr)
    {
        previewPoint(event->scenePos());
    }

    update();

    QGraphicsScene::mouseMoveEvent(event);
}



void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    m_rectangle = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}



void CustomScene::storePoint(const QPointF& point)
{
    if (m_polygon)
    {
        m_polygon->appendPoint(point);
    }
}



void CustomScene::previewPoint(const QPointF& point)
{
    if (m_polygon)
    {
        m_polygon->updatePoint(m_polygon->localPoints().size() - 1, point);
    }
}
