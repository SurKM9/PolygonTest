/*
   Copyright (C) 2020 Denis Gofman - <sendevent@gmail.com>

   This application is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#pragma once

#include <QMap>
#include <QScrollBar>
#include <QWidget>
namespace Ui {
class SBXperiment;
}

class SBXperiment : public QWidget
{
    Q_OBJECT

public:
    explicit SBXperiment(QWidget *parent = nullptr);
    ~SBXperiment();

    QScrollBar *scroll1() const;
    QScrollBar *scroll2() const;

private:
    Ui::SBXperiment *ui;

private slots:
    void on_spinBox_valueChanged(int val);
    void on_spinBox2_valueChanged(int val);
};

class ScrollSyncer : public QObject
{
    Q_OBJECT
public:
    ScrollSyncer(QObject *parent = nullptr);

    void addScrollBar(QScrollBar *sb);
    void removeScrollBar(QScrollBar *sb);

private slots:
    void onScrolled(int val);
    void onRangeChanged(int min, int max);

private:
    QMap<QScrollBar *, qreal> m_onePercent;
    void updateOnePercent(QScrollBar *fromScrollBar);
    void scrollToPercent(QScrollBar *scroll, qreal percent) const;
};
