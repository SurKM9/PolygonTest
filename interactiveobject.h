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

#include "grippoint.h"

#include <QGraphicsObject>
#include <QPen>
#include <QPointer>

class InteractiveObject : public QGraphicsObject
{
    Q_OBJECT
public:
    int type() const override = 0;

    InteractiveObject(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    virtual void updateGripPoints();

Q_SIGNALS:
    void relocated(const QPointF &from, const QPointF &to) const;
    void clicked();
    void doubleClicked();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void onManualMoveStart(GripPoint *gp, const QPointF &at);
    virtual void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);

    virtual void onManualResizeStart(GripPoint *gp, const QPointF &at);
    virtual void onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void onManualResizeFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt);

    virtual void hideGripPoints();
    virtual void showGripPoints();
    virtual void initGripPoints();

    virtual void onSelectionChanged(bool isSelected);

private Q_SLOTS:
    virtual void gripPointPressed(GripPoint *pos, const QPointF &at);
    virtual void gripPointMoved(GripPoint *gp, const QPointF &from, const QPointF &to);
    virtual void gripPointReleased(GripPoint *pos, const QPointF &pressedAt, const QPointF &releasedAt);

protected:
    QPointer<GripPointsHandler> m_gripPointsHandler;
    QRectF m_boundingRect;
    QPointF m_prevPos;
};
