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

class Process;

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = Q_NULLPTR);

    void reset();

    int speed() const;
    bool isConnected() const;

Q_SIGNALS:
    void connectedChanged(bool status);
    void speedChanged(int val);

public Q_SLOTS:
    void setConnected(bool checked);
    void setSpeed(int val);

private:
    Process *m_process;
    bool m_isConnected;
    int m_speed;


};


#endif // CORE_ENGINE_H
