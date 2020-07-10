// qt
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>

// local
#include "keypointitem.h"
#include "grippoint.h"
#include "grippointshandler.h"



KeyPointItem::KeyPointItem(QGraphicsItem* parentItem) :
    InteractiveObject(parentItem)
    , m_item(new QGraphicsPathItem(this))
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents);
    m_item->setPen(QPen(Qt::green));

    m_points.reserve(18);

    // create initial points
    m_points.push_back(QPointF(300, 200)); // 0
    m_points.push_back(QPointF(300, 240)); // 1
    m_points.push_back(QPointF(270, 230)); // 2
    m_points.push_back(QPointF(270, 260)); // 3
    m_points.push_back(QPointF(270, 290)); // 4
    m_points.push_back(QPointF(330, 230)); // 5
    m_points.push_back(QPointF(330, 260)); // 6
    m_points.push_back(QPointF(330, 290)); // 7
    m_points.push_back(QPointF(280, 320)); // 8
    m_points.push_back(QPointF(280, 340)); // 9
    m_points.push_back(QPointF(280, 360)); // 10
    m_points.push_back(QPointF(320, 320)); // 11
    m_points.push_back(QPointF(320, 340)); // 12
    m_points.push_back(QPointF(320, 360)); // 13
    m_points.push_back(QPointF(280, 193)); // 14
    m_points.push_back(QPointF(320, 193)); // 15
    m_points.push_back(QPointF(260, 195)); // 16
    m_points.push_back(QPointF(340, 195)); // 17

    updateBoundingRect();
}



void KeyPointItem::onSelectionChanged(bool isSelected)
{
    if (isSelected)
    {
        QPen pen;
        pen.setWidthF(isSelected ? 1.5 : 1.);
        pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
        m_item->setPen(pen);
    }

    InteractiveObject::onSelectionChanged(isSelected);
}



void KeyPointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // to be able to draw here, remove the QGraphicsItem::ItemHasNoContents flag
}



QRectF KeyPointItem::boundingRect() const
{
    // similar to ::shape() above
    return m_item->boundingRect();
}



void KeyPointItem::updateGripPoints()
{
    // Automatically called by InteractiveObject when it decides
    // that this particular instance needs the grip points to be updated.
    // Can be called manually when the related data updated and we'd like to reflect it in the GUI

    if (!m_gripPointsHandler)
    {
        return;
    }

    const QVector<QPointF> points = this->localPoints();
    if (points.isEmpty())
    {
        return;
    }

    auto grips = m_gripPointsHandler->gripPoints();
    for (int idx = 0; idx < qMax(points.size(), grips.size()); ++idx)
    {
        if (idx >= points.size())
        {
            m_gripPointsHandler->removeGripPoint(grips.value(idx));
            continue;
        }
        if (idx >= grips.size())
        {
            m_gripPointsHandler->createGripPoint(GripPoint::Absolute);
        }
        m_gripPointsHandler->setGripPointPos(grips.value(idx), points.value(idx));
    }

    InteractiveObject::updateGripPoints();
}



QVector<QPointF> KeyPointItem::localPoints() const
{
    QVector<QPointF> pointsTable;
    QPainterPath p = m_item->path();

    qDebug() << p.elementCount();

    for (int i = 0; i < p.elementCount(); ++i)
    {
        pointsTable.push_back(p.elementAt(i));
    }

    return pointsTable;
    /*QPolygonF polygon = m_item->path().toFillPolygon();

    if (polygon.isClosed())
    {
        polygon.removeLast();
    }

    return mapToScene(polygon);*/
}



QPainterPath KeyPointItem::shape() const
{
    // QGraphicsItem's functionality.
    // It's used in scene's collision detection routines
    // and to get an item which is currently under mouse.

    return m_item->shape();

    // Note that the shape is in local coordinates, so for complex items
    // it's necessary to translate children shapes to parent coordinates:

    //    QPainterPath thisShape = BaseClass::shape();
    //    QPainterPath titleShape = m_titleItem->shape();
    //    titleShape.translate(m_titleItem->pos());
    //    QPainterPath contentShape = m_contentItem->shape();
    //    contentShape.translate(m_contentItem->pos());
    //    thisShape.addPath(titleShape);
    //    thisShape.addPath(contentShape);
}



void KeyPointItem::onManualMoveStart(GripPoint* gp, const QPointF& at)
{
}



void KeyPointItem::onManualMoveProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(gp);

    // It's a place to check if the current item is movable,
    // validate the desired movement, check for collisions or whatever,
    // and apply it to GUI, if the movement is acceptable

    QPointF shift = to - from;
    if (!shift.isNull())
    {
        if (auto scene = this->scene())
        {
            const QRectF& sceneRect = scene->sceneRect();
            for (const QPointF& currentPoint : localPoints())
            {
                const QPointF& upcomingPoint = currentPoint + shift;
                if (!sceneRect.contains(upcomingPoint))
                {
                    return;
                }
            }

            moveBy(shift.x(), shift.y());
            updateBoundingRect();
        }
    }
}



void KeyPointItem::onManualMoveFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
}



void KeyPointItem::onManualResizeStart(GripPoint* gp, const QPointF& at)
{
}



void KeyPointItem::onManualResizeProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(from)

    if (!m_gripPointsHandler || !gp)
    {
        return;
    }

    if (scene() && scene()->sceneRect().contains(to))
    {
        const auto grips = m_gripPointsHandler->gripPoints();
        updatePoint(grips.indexOf(gp), to);
    }
}



void KeyPointItem::onManualResizeFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
}



void KeyPointItem::initGripPoints()
{
    // Automatically called by InteractiveObject when it decides
    // that this particular instance needs the grip points.
    // There is no point to call it directly.
    InteractiveObject::initGripPoints();

    for (int idx = 0; idx < m_points.size(); ++idx)
    {
        m_gripPointsHandler->createGripPoint(GripPoint::Absolute, idx);
    }
}



void KeyPointItem::updatePoint(int at, const QPointF& scenePoint)
{
    if (at >= 0 && at < m_points.size())
    {
        m_points[at] = mapFromScene(scenePoint);
        updateBoundingRect();
    }
}



void KeyPointItem::updateBoundingRect()
{
    prepareGeometryChange();

    QPainterPath pp;

    pp.moveTo(m_points[0]);
    pp.lineTo(m_points[1]);
    pp.lineTo(m_points[2]);
    pp.lineTo(m_points[3]);
    pp.lineTo(m_points[4]);

    pp.moveTo(m_points[1]);
    pp.lineTo(m_points[5]);
    pp.lineTo(m_points[6]);
    pp.lineTo(m_points[7]);

    pp.moveTo(m_points[1]);
    pp.lineTo(m_points[8]);
    pp.lineTo(m_points[9]);
    pp.lineTo(m_points[10]);

    pp.moveTo(m_points[1]);
    pp.lineTo(m_points[11]);
    pp.lineTo(m_points[12]);
    pp.lineTo(m_points[13]);

    pp.moveTo(m_points[0]);
    pp.lineTo(m_points[14]);
    pp.lineTo(m_points[16]);

    pp.moveTo(m_points[0]);
    pp.lineTo(m_points[15]);
    pp.lineTo(m_points[17]);

    //pp.closeSubpath();
    m_item->setPath(pp);

    // while this particular implementation we using m_item->boundingRect() instead,
    // in complex items it may be comfy to handle the bounding rect manually.
    // Then it's the right place to update InteractiveObject::m_boundingRect:
    //    m_boundingRect = pp.boundingRect();
    updateGripPoints();
}

