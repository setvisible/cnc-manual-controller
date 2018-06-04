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

#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QtCore/QObject>
#include <QtCore/QtContainerFwd> /* Forward Declarations of the Qt's Containers */


class CircularBuffer;
class IMachine;
class Scheduler;
class ControlWidget;
class GamepadWidget;
class Preferences;

class Engine : public QObject
{
    friend class Scheduler;
    Q_OBJECT
public:
    explicit Engine(QObject *parent = Q_NULLPTR);

    void reset();

    int ticksPerSecond() const;
    bool isConnected() const;

    void setControlInput(ControlWidget *controlWidget);
    void setGamepadInput(GamepadWidget *gamepadWidget);

    Preferences *preferences() const;


Q_SIGNALS:
    void connectedChanged(bool status);
    void ticksPerSecondChanged(int ticksPerSecond);

    void started();
    void stopped();

public Q_SLOTS:
    void setConnected(bool checked);
    void setTicksPerSecond(int ticksPerSecond);

    void start();
    void stop();

    void infoString(QString message);
    void errorString(QString err);

protected:
    void commandCallback() noexcept;

public Q_SLOTS:
    void readInput();

private:
    CircularBuffer *m_circularBuffer;
    IMachine *m_machine;
    bool m_isConnected;
    int m_interval;

    ControlWidget *m_controlWidget;
    GamepadWidget *m_gamepadWidget;
    Preferences *m_preferences;

    void readControlInput();
    void readGamepadInput();
};


#endif // CORE_ENGINE_H
