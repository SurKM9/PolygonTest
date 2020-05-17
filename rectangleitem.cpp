// qt
#include <QGraphicsScene>

// local
#include "rectangleitem.h"
#include "grippoint.h"
#include "grippointshandler.h"



RectangleItem::RectangleItem(QGraphicsItem* parent) :
    InteractiveObject(parent),
    m_item(new QGraphicsRectItem(this))
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents);
    m_item->setPen(QPen(Qt::green, 2, Qt::SolidLine));
    m_item->setBrush(QBrush(QColor(128, 128, 255, 128), Qt::SolidPattern));
}



QRectF RectangleItem::boundingRect() const
{
    return m_item->boundingRect();
}



QPainterPath RectangleItem::shape() const
{
    return m_item->shape();
}



void RectangleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}



int RectangleItem::type() const
{
    return Type;
}



void RectangleItem::updateGripPoints()
{
    if (!m_gripPointsHandler)
    {
        return;
    }

    const QVector<QPointF> points = this->localPoints();
    if (points.isEmpty())
    {
        return;
    }

    for(int idx = 0; idx < 8; ++idx)
    {
        auto grips = m_gripPointsHandler->gripPoints();
        m_gripPointsHandler->setGripPointPos(grips.value(idx), points.value(idx));
    }

    InteractiveObject::updateGripPoints();
}



QVector<QPointF> RectangleItem::localPoints() const
{
    QVector<QPointF> pointsTable;
    const QRectF rect = m_item->rect();

    pointsTable.append(mapToScene(rect.topLeft()));
    pointsTable.append(mapToScene(QPointF(rect.left() + rect.width() / 2 - 1, rect.top())));

    pointsTable.append(mapToScene(rect.topRight()));
    pointsTable.append(mapToScene(QPointF(rect.right(), rect.top() + rect.height() / 2 - 1)));

    pointsTable.append(mapToScene(rect.bottomRight()));
    pointsTable.append(mapToScene(QPointF(rect.left() + rect.width() / 2 - 1, rect.bottom())));

    pointsTable.append(mapToScene(rect.bottomLeft()));
    pointsTable.append(mapToScene(QPointF(rect.left(), rect.top() + rect.height() / 2 - 1)));

    return pointsTable;
}



void RectangleItem::updateRect(const QRectF& rect)
{
    m_rect = rect;
    updateBoundingRect();
}



void RectangleItem::onManualMoveStart(GripPoint* gp, const QPointF& at)
{
    Q_UNUSED(gp);
    Q_UNUSED(at);

}



void RectangleItem::onManualMoveProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(gp);

    // It's a place to check if the current item is movable,
    // validate the desired movement, check for collisions or whatever,
    // and apply it to GUI, if the movement is acceptable

    //    QPointF shift = to - from;
    //    if (!shift.isNull())
    //    {
    //        moveBy(shift.x(), shift.y());
    //        updateBoundingRect();
    //    }

    const QPointF shift = to - from;
    if (!shift.isNull())
    {
        moveBy(shift.x(), shift.y());

        // get the new moved position
        QPointF newPos = pos();

        // restricts the item from moving outside the
        // image
        if (newPos.x() < 0)
        {
            newPos.setX(0);
        }
        else if (newPos.x() + boundingRect().right() > scene()->width())
        {
            newPos.setX(scene()->width() - boundingRect().width());
        }

        if (newPos.y() < 0)
        {
            newPos.setY(0);
        }
        else if (newPos.y() + boundingRect().bottom() > scene()->height())
        {
            newPos.setY(scene()->height() - boundingRect().height());
        }

        setPos(newPos);
        updateBoundingRect();
    }
}



void RectangleItem::onManualMoveFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
    Q_UNUSED(gp);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);

    // It's a place to "commit" the whole movement - update the data represented by this item,
    // populate the UndoCommands stack, etc
}



void RectangleItem::onManualResizeStart(GripPoint* gp, const QPointF& at)
{
    Q_UNUSED(gp);
    Q_UNUSED(at);
}



void RectangleItem::onManualResizeProgress(GripPoint* gp, const QPointF& from, const QPointF& to)
{
    Q_UNUSED(from)

    if (!m_gripPointsHandler || !gp)
    {
        return;
    }

    switch(gp->location())
    {
        case GripPoint::Top:
            m_rect.setTop(mapFromScene(to).y());
            break;

        case GripPoint::Right:
            m_rect.setRight(mapFromScene(to).x());
            break;

        case GripPoint::Bottom:
            m_rect.setBottom(mapFromScene(to).y());
            break;

        case GripPoint::Left:
            m_rect.setLeft(mapFromScene(to).x());
            break;

        case GripPoint::TopLeft:
            m_rect.setTopLeft(mapFromScene(to));
            break;

        case GripPoint::TopRight:
            m_rect.setTopRight(mapFromScene(to));
            break;

        case GripPoint::BottomRight:
            m_rect.setBottomRight(mapFromScene(to));
            break;

        case GripPoint::BottomLeft:
            m_rect.setBottomLeft(mapFromScene(to));
            break;

        default:
            break;
    }

    updateRect(m_rect);
}



void RectangleItem::onManualResizeFinish(GripPoint* gp, const QPointF& pressedAt, const QPointF& releasedAt)
{
    Q_UNUSED(gp);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);
}



void RectangleItem::initGripPoints()
{
    InteractiveObject::initGripPoints();

    // topLeft
    m_gripPointsHandler->createGripPoint(GripPoint::TopLeft, 0);
    m_gripPointsHandler->createGripPoint(GripPoint::Top, 1);

    // topRight
    m_gripPointsHandler->createGripPoint(GripPoint::TopRight, 2);
    m_gripPointsHandler->createGripPoint(GripPoint::Right, 3);

    // bottomRight
    m_gripPointsHandler->createGripPoint(GripPoint::BottomRight, 4);
    m_gripPointsHandler->createGripPoint(GripPoint::Bottom, 5);

    // bottomLeft
    m_gripPointsHandler->createGripPoint(GripPoint::BottomLeft, 6);
    m_gripPointsHandler->createGripPoint(GripPoint::Left, 7);
}



void RectangleItem::onSelectionChanged(bool isSelected)
{
    if(!isSelected)
    {
        m_item->setPen(QPen(Qt::green, 2, Qt::SolidLine));
        m_item->setBrush(QBrush(QColor(128, 128, 255, 128), Qt::SolidPattern));
    }
    else
    {
        QPen pen;
        pen.setWidthF(isSelected ? 1.5 : 1.);
        pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
        m_item->setPen(pen);
    }

    InteractiveObject::onSelectionChanged(isSelected);
}



void RectangleItem::updateBoundingRect()
{
    // The helper method used to "rebuild" the layout
    // note the prepareGeometryChange() call -
    // it's necessary for QGraphicsItem to be updated correctyl

    prepareGeometryChange();
    m_boundingRect = m_rect;

    m_item->setRect(m_rect);

    // while this particular implementation we using m_item->boundingRect() instead,
    // in complex items it may be comfy to handle the bounding rect manually.
    // Then it's the right place to update InteractiveObject::m_boundingRect:
    //    m_boundingRect = pp.boundingRect();
    updateGripPoints();
}
