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

#include "grippoint.h"

#include "grippointshandler.h"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GripPoint::GripPoint(Location pos, GripPointsHandler *parent)
    : QGraphicsItem(parent)
    , m_listener(parent)
    , m_location(pos)
    , m_boundRect(0., 0., 16., 16.)
{
    setFlags(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setVisible(true);
};

void GripPoint::updateLayout()
{
    if (QGraphicsItem *targetObj = parentItem()) {
        QCursor c;

        const QRectF targetRect = targetObj->boundingRect();
        QPointF destination;
        switch (m_location) {
        case TopLeft:
            destination = targetRect.topLeft();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeFDiagCursor;
            break;
        case Top:
            destination = { targetRect.center().x(), targetRect.top() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeVerCursor;
            break;
        case TopRight:
            destination = targetRect.topRight();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeBDiagCursor;
            break;
        case Right:
            destination = { targetRect.right(), targetRect.center().y() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeHorCursor;
            break;
        case BottomRight:
            destination = targetRect.bottomRight();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeFDiagCursor;
            break;
        case Bottom:
            destination = { targetRect.center().x(), targetRect.bottom() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeVerCursor;
            break;
        case BottomLeft:
            destination = targetRect.bottomLeft();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeBDiagCursor;
            break;
        case Left:
            destination = { targetRect.left(), targetRect.center().y() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeHorCursor;
            break;
        case Center: {
            destination = targetRect.center();
            c = Qt::SizeAllCursor;
            break;
        }
        case Absolute:
            destination = pos();
            c = Qt::SizeAllCursor;
            break;
        }

        if (Center != m_location) {
            QRectF r(m_boundRect.translated(pos()));
            r.moveCenter(destination);
            setPos(r.topLeft());
        }

        setCursor(c);
    }

    m_path = QPainterPath();
    m_path.addRect(m_boundRect);
}

QRectF GripPoint::boundingRect() const
{
    return m_boundRect;
}

bool GripPoint::isMover() const
{
    return gripType() == GripPoint::GripType::Mover;
}

GripPoint::GripType GripPoint::gripType() const
{
    return m_location == Center ? GripType::Mover : GripType::Resizer;
}

bool GripPoint::isUsed() const
{
    return m_used;
}
void GripPoint::setIsUsed(bool used)
{
    if (m_used == used)
        return;

    m_used = used;
    if (!m_used)
        setVisible(false);
}

void GripPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    static const QBrush fillBrush(Qt::white);
    static const QPen strokePen(Qt::darkBlue);

    painter->fillPath(m_path, fillBrush);
    painter->strokePath(m_path, strokePen);
}

void GripPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener)
        m_listener->handleGripPointMove(this, event->lastScenePos(), event->scenePos());
}

void GripPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_listener)
            m_listener->handleGripPointPress(this, event->scenePos());
        event->accept();
    } else {
        event->ignore();
    }
}

void GripPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener)
        m_listener->handleGripPointRelease(this, event->buttonDownScenePos(event->button()), event->scenePos());
}
