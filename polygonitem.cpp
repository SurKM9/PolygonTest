#include "polygonitem.h"

#include "grippoint.h"
#include "grippointshandler.h"

PolygonItem::PolygonItem(QGraphicsItem* parentItem)
    : InteractiveObject(parentItem)
    , m_item(new QGraphicsPathItem(this))
    , m_points()
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents);
    m_item->setBrush(Qt::gray);
}

QPainterPath PolygonItem::shape() const
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

QRectF PolygonItem::boundingRect() const
{
    // similar to ::shape() above
    return m_item->boundingRect();
}

void PolygonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // to be able to draw here, remove the QGraphicsItem::ItemHasNoContents flag
}

void PolygonItem::initGripPoints()
{
    // Automatically called by InteractiveObject when it decides
    // that this particular instance needs the grip points.
    // There is no point to call it directly.

    InteractiveObject::initGripPoints();
    for (int idx = 0; idx < m_points.size(); ++idx)
    {
        m_gripPointsHandler->createGripPoint(GripPoint::Absolute);
    }
}

void PolygonItem::updateGripPoints()
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

QVector<QPointF> PolygonItem::localPoints() const
{
    QPolygonF polygon = m_item->path().toFillPolygon();
    if (polygon.isClosed())
    {
        polygon.removeLast();
    }
    return mapToScene(polygon);
}

void PolygonItem::appendPoint(const QPointF& scenePoint)
{
    m_points.append(mapFromScene(scenePoint));
    updateBoundingRect();
}

void PolygonItem::updatePoint(int at, const QPointF& scenePoint)
{
    if (at >= 0 && at < m_points.size())
    {
        m_points[at] = mapFromScene(scenePoint);
        updateBoundingRect();
    }
}

void PolygonItem::onManualMoveStart(GripPoint* gp, const QPointF& at)
{
    Q_UNUSED(gp);
    Q_UNUSED(at);

    // This is a handler for "mouse press event";
    // It is invoked as a result of either:
    // - user interaction with GripPoint::Center, if any, or, as in this particular case,
    // - direct drag of item body

    // It's the place to validate current item state, is it capable to be moved at all, etc.

    // If you'd like to disable movement for this particular instance of PolygonItem,
    // introduce something like m_isMovable = false; to be checked in
    // PolygonItem::onManualMoveProgress/Finish

    // If you'd like to disable movement for all instances of PolygonItem,
    // just keep its implementation of onManualMoveStart/Progress/Finish empty
}

void PolygonItem::onManualMoveProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(gp);

    // It's a place to check if the current item is movable,
    // validate the desired movement, check for collisions or whatever,
    // and apply it to GUI, if the movement is acceptable

    const QPointF shift = to - from;
    if (!shift.isNull())
    {
        moveBy(shift.x(), shift.y());
        updateBoundingRect();
    }
}

void PolygonItem::onManualMoveFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
    Q_UNUSED(gp);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);

    // It's a place to "commit" the whole movement - update the data represented by this item,
    // populate the UndoCommands stack, etc
}

void PolygonItem::onManualResizeStart(GripPoint* gp, const QPointF& at)
{
    Q_UNUSED(gp);
    Q_UNUSED(at);
}

void PolygonItem::onManualResizeProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(from)

    if (!m_gripPointsHandler || !gp)
    {
        return;
    }

    const auto grips = m_gripPointsHandler->gripPoints();
    updatePoint(grips.indexOf(gp), to);
}

void PolygonItem::onManualResizeFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
    Q_UNUSED(gp);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);
}

void PolygonItem::onSelectionChanged(bool isSelected)
{
    QPen pen;
    pen.setWidthF(isSelected ? 1.5 : 1.);
    pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
    m_item->setPen(pen);

    InteractiveObject::onSelectionChanged(isSelected);
}

void PolygonItem::updateBoundingRect()
{
    // The helper method used to "rebuild" the layout
    // note the prepareGeometryChange() call -
    // it's necessary for QGraphicsItem to be updated correctyl

    prepareGeometryChange();

    QPainterPath pp;
    pp.addPolygon(QPolygonF(m_points));
    pp.closeSubpath();

    m_item->setPath(pp);

    // while this particular implementation we using m_item->boundingRect() instead,
    // in complex items it may be comfy to handle the bounding rect manually.
    // Then it's the right place to update InteractiveObject::m_boundingRect:
    //    m_boundingRect = pp.boundingRect();
    updateGripPoints();
}
