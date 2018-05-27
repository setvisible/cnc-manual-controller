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

#ifndef JOYSTICK_QJOYSTICK_H
#define JOYSTICK_QJOYSTICK_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

class QJoystick : public QObject
{
    Q_OBJECT
public:
    explicit QJoystick(QObject *parent = Q_NULLPTR);
    ~QJoystick();

    int availableJoystickCount() const;

    int joystickIndex() const;
    void setJoystickIndex(int deviceIndex);

    QString joystickName(int deviceIndex) const;
    int joystickNumAxes(int deviceIndex) const;
    int joystickNumButtons(int deviceIndex) const;

    void storeCurrentState();

    QList<int> axis() const;
    QList<bool> buttons() const;

private:
    SDL_Joystick *m_joystick;
    QList<int> m_axis;
    QList<bool> m_buttons;

};

#endif // JOYSTICK_QJOYSTICK_H
