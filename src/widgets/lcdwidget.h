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

#ifndef WIDGETS_LCD_WIDGET_H
#define WIDGETS_LCD_WIDGET_H

#include <QWidget>
#include <QVector3D>

namespace Ui {
    class LCDWidget;
}

class LCDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LCDWidget(QWidget* parent = Q_NULLPTR);
    ~LCDWidget();

public Q_SLOTS:
    void updateLCDDisplay(QVector3D vec);
    void updateSensorLED(bool isLedXOn, bool isLedYOn, bool isLedZOn);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LCDWidget *ui;
};

#endif // WIDGETS_LCD_WIDGET_H
