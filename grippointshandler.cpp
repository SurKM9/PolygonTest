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

#include "grippointshandler.h"

GripPointsHandler::GripPointsHandler(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , AbstractInteractiveObject()
{
    setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemSendsGeometryChanges);
    hide();
}

GripPoint *GripPointsHandler::createGripPoint(GripPoint::Location location, int idx)
{
    auto grip = new GripPoint(location, this);
    m_usedPoints.insert(grip->location());
    m_gripPoints.insert(idx == -1 ? m_gripPoints.size() : idx, grip);
    return grip;
}

void GripPointsHandler::removeGripPoint(GripPoint *handle)
{
    m_gripPoints.removeOne(handle);
    delete handle;
}

GripPoint::Locations GripPointsHandler::usedPoints() const
{
    return m_usedPoints;
}

QList<GripPoint *> GripPointsHandler::gripPoints() const
{
    return m_gripPoints;
}

void GripPointsHandler::updateLayout()
{
    for (GripPoint *handle : m_gripPoints) {
        const bool used = m_usedPoints.contains(handle->location());
        handle->setIsUsed(used);
        if (used)
            handle->updateLayout();
    }
}

QRectF GripPointsHandler::boundingRect() const
{
    return parentItem() ? parentItem()->boundingRect() : QRectF();
}

void GripPointsHandler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void GripPointsHandler::handleGripPointPress(GripPoint *handle, const QPointF &at)
{
    Q_EMIT manualGeometryChangeStart(handle, at);
}

void GripPointsHandler::handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to)
{
    Q_EMIT manualGeometryChangeProgress(handle, from, to);
}

void GripPointsHandler::handleGripPointRelease(GripPoint *handle, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_EMIT manualGeometryChangeFinish(handle, pressedAt, releasedAt);
}

void GripPointsHandler::setGripPointPos(GripPoint *grip, const QPointF &pos)
{
    if (grip) {
        QRectF br = grip->boundingRect();
        br.moveCenter(mapFromScene(pos));
        grip->setPos(br.topLeft());
    }
}
