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

#pragma once

#include "abstractinteractiveobject.h"
#include "grippoint.h"
#include "itemtypes.h"

#include <QGraphicsObject>
#include <QSet>

class GripPointsHandler : public QGraphicsObject, public AbstractInteractiveObject
{
    Q_OBJECT

public:
    enum
    {
        Type = UserType + static_cast<int>(myitems::Type::ItemGripPointsHandler)
    };
    int type() const override { return Type; }

    explicit GripPointsHandler(QGraphicsItem *parent = nullptr);

    GripPoint *createGripPoint(GripPoint::Location location, int idx = -1);
    void removeGripPoint(GripPoint *handle);

    void handleGripPointPress(GripPoint *handle, const QPointF &at) override;
    void handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to) override;
    void handleGripPointRelease(GripPoint *handle, const QPointF &pressedAt, const QPointF &releasedAt) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QList<GripPoint *> gripPoints() const;
    void updateLayout();

    GripPoint::Locations usedPoints() const;

    void setGripPointPos(GripPoint *grip, const QPointF &pos);

Q_SIGNALS:
    void manualGeometryChangeStart(GripPoint *gripPoint, const QPointF &at);
    void manualGeometryChangeProgress(GripPoint *gripPoint, const QPointF &from, const QPointF &to);
    void manualGeometryChangeFinish(GripPoint *gripPoint, const QPointF &startedAt, const QPointF &releasedAt);

protected:
    QList<GripPoint *> m_gripPoints;
    bool m_visible = false;

    GripPoint::Locations m_usedPoints;
};
