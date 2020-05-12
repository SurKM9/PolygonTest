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

class QPointF;

class GripPoint;
class AbstractInteractiveObject
{
public:
    virtual ~AbstractInteractiveObject() {}
    virtual void handleGripPointPress(GripPoint *handle, const QPointF &at) = 0;
    virtual void handleGripPointMove(GripPoint *handle, const QPointF &from, const QPointF &to) = 0;
    virtual void handleGripPointRelease(GripPoint *handle, const QPointF &pressedAt, const QPointF &releasedAt) = 0;
};
