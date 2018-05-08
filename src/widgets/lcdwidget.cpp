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

#include "lcdwidget.h"
#include "ui_lcdwidget.h"


LCDWidget::LCDWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::LCDWidget)
{
    ui->setupUi(this);

    this->updateLCDDisplay(QVector3D(0, 0, 0));
    this->updateSensorLED(false, false, false);
}

LCDWidget::~LCDWidget()
{
    delete ui;
}

void LCDWidget::changeEvent(QEvent *e)
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

void LCDWidget::updateLCDDisplay(QVector3D vec )
{
    ui->LCD_1->setText(QString("%0  %1").arg("+").arg(vec.x()));
    ui->LCD_2->setText(QString("%0  %1").arg("+").arg(vec.y()));
    ui->LCD_3->setText(QString("%0  %1").arg("+").arg(vec.z()));
}

void LCDWidget::updateSensorLED(bool isLedXOn, bool isLedYOn, bool isLedZOn)
{
    QString style_OFF = QLatin1String(" background-color: rgb(55, 0, 0);");
    QString style_ON  = QLatin1String(" background-color: rgb(0,255, 0);");
    ui->endLED_1->setStyleSheet(isLedXOn ? style_ON : style_OFF);
    ui->endLED_2->setStyleSheet(isLedYOn ? style_ON : style_OFF);
    ui->endLED_3->setStyleSheet(isLedZOn ? style_ON : style_OFF);
}
