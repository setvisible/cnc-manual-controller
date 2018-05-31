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

#include "gamepadaxiswidget.h"
#include "ui_gamepadaxiswidget.h"

GamepadAxisWidget::GamepadAxisWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::GamepadAxisWidget)
{
    ui->setupUi(this);
    ui->slider->setEnabled(false);
}

GamepadAxisWidget::~GamepadAxisWidget()
{
    delete ui;
}

QString GamepadAxisWidget::title() const
{
    QString text = ui->titleTop->text();
    text = text.remove(0,1);
    return text;
}

void GamepadAxisWidget::setTitle(const QString &title)
{
    ui->titleTop->setText(QString("+%0").arg(title));
    ui->titleBottom->setText(QString("-%0").arg(title));
}

int GamepadAxisWidget::value() const
{
    return ui->slider->value();
}

void GamepadAxisWidget::setValue(int value)
{
    if (ui->slider->value() != value) {
        ui->slider->setValue(value);
        emit valueChanged(value);
    }
}
