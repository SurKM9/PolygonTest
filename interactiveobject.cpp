/*
   Copyright (C) 2020 Denis Gofman - <sendevent@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#include "interactiveobject.h"

#include "grippointshandler.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

InteractiveObject::InteractiveObject(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
             | QGraphicsItem::ItemIsSelectable);

    setCursor(Qt::ArrowCursor);
}

void InteractiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF InteractiveObject::boundingRect() const
{
    return m_boundingRect;
}

void InteractiveObject::gripPointPressed(GripPoint *gp, const QPointF &at)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveStart(gp, at);
    else
        onManualResizeStart(gp, at);
}

void InteractiveObject::gripPointMoved(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveProgress(gp, from, to);
    else
        onManualResizeProgress(gp, from, to);
}

void InteractiveObject::gripPointReleased(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (!gp)
        return;

    if (gp->isMover())
        onManualMoveFinish(gp, pressedAt, releasedAt);
    else
        onManualResizeFinish(gp, pressedAt, releasedAt);
}

void InteractiveObject::onSelectionChanged(bool isSelected)
{
    setZValue(isSelected ? 1 : 0);
    if (isSelected) {
        showGripPoints();
        updateGripPoints();
    } else {
        hideGripPoints();
    }
}

void InteractiveObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
    onManualMoveProgress(nullptr, event->lastScenePos(), event->scenePos());
}

void InteractiveObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    onManualMoveStart(nullptr, event->scenePos());
}

void InteractiveObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    onManualMoveFinish(nullptr, event->buttonDownScenePos(event->button()), event->scenePos());
    emit clicked();
    QGraphicsObject::mouseReleaseEvent(event);
}

void InteractiveObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit doubleClicked();
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void InteractiveObject::onManualMoveStart(GripPoint *gp, const QPointF &at)
{
    Q_UNUSED(gp)
    Q_UNUSED(at)
}

void InteractiveObject::onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp)
    Q_UNUSED(from)
    Q_UNUSED(to)
}

void InteractiveObject::onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(gp)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)
}

void InteractiveObject::onManualResizeStart(GripPoint *gp, const QPointF &at)
{
    Q_UNUSED(gp)
    Q_UNUSED(at)
}

void InteractiveObject::onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(gp)
    Q_UNUSED(pressedAt)
    Q_UNUSED(releasedAt)
}

void InteractiveObject::onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp)
    Q_UNUSED(from)
    Q_UNUSED(to)
}

void InteractiveObject::hideGripPoints()
{
    m_gripPointsHandler->setVisible(false);
}

void InteractiveObject::showGripPoints()
{
    initGripPoints();
    m_gripPointsHandler->setVisible(true);
}

void InteractiveObject::initGripPoints()
{
    if (m_gripPointsHandler)
        return;

    m_gripPointsHandler = new GripPointsHandler(this);

    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeStart, this,
            &InteractiveObject::gripPointPressed);
    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeProgress, this,
            &InteractiveObject::gripPointMoved);
    connect(m_gripPointsHandler, &GripPointsHandler::manualGeometryChangeFinish, this,
            &InteractiveObject::gripPointReleased);

    connect(m_gripPointsHandler, &GripPointsHandler::visibleChanged, this, [this]() {
        if (m_gripPointsHandler && !m_gripPointsHandler->isVisible())
            delete m_gripPointsHandler; // it's not a thing directly added to the scene, so just delete is enough
    });
}

void InteractiveObject::updateGripPoints()
{
    if (m_gripPointsHandler)
        m_gripPointsHandler->updateLayout();
}

QVariant InteractiveObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemSelectedChange:
        onSelectionChanged(value.toBool());
        break;
    case QGraphicsItem::ItemPositionChange:
        m_prevPos = pos();
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        Q_EMIT relocated(m_prevPos, pos());
        break;
    default:
        break;
    }

    return QGraphicsObject::itemChange(change, value);
}
