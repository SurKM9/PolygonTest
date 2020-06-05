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

#include "sbxperiment.h"

#include "ui_sbxperiment.h"

#include <QDebug>
#include <QTableWidget>

SBXperiment::SBXperiment(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SBXperiment)
{
    ui->setupUi(this);

    ui->spinBox->setValue(100);
    ui->spinBox2->setValue(2000);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setRowCount(10);
}

SBXperiment::~SBXperiment()
{
    delete ui;
}

void SBXperiment::on_spinBox_valueChanged(int val)
{
    ui->horizontalScrollBar->setMaximum(val);
}

void SBXperiment::on_spinBox2_valueChanged(int val)
{
    ui->horizontalScrollBar_2->setMaximum(val);
}

QScrollBar* SBXperiment::scroll1() const
{
    return ui->horizontalScrollBar;
}

QScrollBar* SBXperiment::scroll2() const
{
    return ui->horizontalScrollBar_2;
}

QScrollBar* SBXperiment::scroll3() const
{
    return ui->tableWidget->horizontalScrollBar();
}

ScrollSyncer::ScrollSyncer(QObject* parent)
    : QObject(parent)
{
}

void ScrollSyncer::addScrollBar(QScrollBar* sb)
{
    if (m_onePercent.contains(sb))
    {
        return;
    }

    connect(sb, &QScrollBar::rangeChanged, this, &ScrollSyncer::onRangeChanged);
    connect(sb, &QScrollBar::valueChanged, this, &ScrollSyncer::onScrolled);

    updateOnePercent(sb);
}

void ScrollSyncer::removeScrollBar(QScrollBar* sb)
{
    if (m_onePercent.remove(sb) > 0)
    {
        disconnect(sb, &QScrollBar::rangeChanged, this, &ScrollSyncer::onRangeChanged);
        disconnect(sb, &QScrollBar::valueChanged, this, &ScrollSyncer::onScrolled);
    }
}

void ScrollSyncer::updateOnePercent(QScrollBar* fromScrollBar)
{
    if (!fromScrollBar)
    {
        return;
    }

    m_onePercent[fromScrollBar] = qreal(fromScrollBar->maximum()) / 100.;
}

void ScrollSyncer::onRangeChanged(int /*min*/, int /*max*/)
{
    if (auto sb = qobject_cast<QScrollBar*>(sender()))
    {
        updateOnePercent(sb);
    }
}

void ScrollSyncer::onScrolled(int val)
{
    if (auto sb = qobject_cast<QScrollBar*>(sender()))
    {
        const qreal percent = qreal(val) / m_onePercent[sb];

        for (auto scroll : m_onePercent.keys())
            if (scroll != sb)
            {
                scrollToPercent(scroll, percent);
            }
    }
}

void ScrollSyncer::scrollToPercent(QScrollBar* scroll, qreal percent) const
{
    disconnect(scroll, &QScrollBar::valueChanged, this, &ScrollSyncer::onScrolled);
    const qreal onePercent = m_onePercent[scroll];
    scroll->setValue(percent * onePercent);
    connect(scroll, &QScrollBar::valueChanged, this, &ScrollSyncer::onScrolled);
}
