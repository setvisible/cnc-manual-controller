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

#ifndef GUI_GAMEPAD_WIDGET_H
#define GUI_GAMEPAD_WIDGET_H

#include <QtWidgets/QWidget>

QT_BEGIN_INCLUDE_NAMESPACE
class QTimer;
QT_END_INCLUDE_NAMESPACE

class JoystickDevice;

namespace Ui {
class GamepadWidget;
}

class GamepadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GamepadWidget(QWidget* parent = Q_NULLPTR);
    ~GamepadWidget();

    int valueAxisX() const;
    int valueAxisY() const;
    int valueAxisZ() const;

public Q_SLOTS:
    int deviceIndex() const;
    void setDeviceIndex(int deviceIndex);

    bool isActive() const;
    void setActive(bool active);

private:
    Ui::GamepadWidget *ui;
    JoystickDevice *m_joystickDevice;
    QTimer *m_timer;
    int m_deviceIndex;

private Q_SLOTS:
    void updateState();
};

#endif // GUI_GAMEPAD_WIDGET_H
