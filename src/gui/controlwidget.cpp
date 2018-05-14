/* - CNC Manual Controller - Copyright (C) 2018 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <Widgets/AxisControlWidget>

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);

    ui->axis0Control->setTitle(tr("Axis X"));
    ui->axis1Control->setTitle(tr("Axis Y"));
    ui->axis2Control->setTitle(tr("Axis Z"));
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
