// qt
#include <QDebug>
#include <QGraphicsView>

// local
#include "customscene.h"
#include "polygonitem.h"

CustomScene::CustomScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_polygon(nullptr)
{
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const QPointF &m_origPoint = event->scenePos();

    views().at(0)->setDragMode(QGraphicsView::NoDrag);

    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
        if (m_polygon == nullptr) {
            m_polygon = new PolygonItem({});
            addItem(m_polygon);
            storePoint(m_origPoint);
        }

        storePoint(m_origPoint);
        return;
    }

    if (event->button() == Qt::RightButton && m_polygon != nullptr) {
        m_polygon = nullptr;
    }

    update();

    QGraphicsScene::mousePressEvent(event);
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_polygon != nullptr) {
        previewPoint(event->scenePos());
    }

    update();

    QGraphicsScene::mouseMoveEvent(event);
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    QGraphicsScene::mouseReleaseEvent(event);
}

void CustomScene::storePoint(const QPointF &point)
{
    if (m_polygon)
        m_polygon->appendPoint(point);
}

void CustomScene::previewPoint(const QPointF &point)
{
    if (m_polygon)
        m_polygon->updatePoint(m_polygon->localPoints().size() - 1, point);
}
