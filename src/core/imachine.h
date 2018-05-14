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

#ifndef CORE_IMACHINE_H
#define CORE_IMACHINE_H

#include <QtCore/QObject>
#include <QtCore/QtContainerFwd> /* Forward Declarations of the Qt's Containers */

#include <Core/CommandStep>

/*! \class IMachine
 *  \brief The class IMachine is an interface to the CNC machine.
 *
 */
class IMachine : public QObject
{
public:
    explicit IMachine(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    virtual ~IMachine() Q_DECL_NOEXCEPT {}

    virtual bool openDevice() = 0;
    virtual void closeDevice() = 0;

    virtual void receive() = 0;

    virtual bool isEmergencyActive() = 0;
    virtual bool hasSensorActive() = 0;

    virtual bool isSensorMXActive() = 0;
    virtual bool isSensorMYActive() = 0;
    virtual bool isSensorMZActive() = 0;
    virtual bool isSensorPXActive() = 0;
    virtual bool isSensorPYActive() = 0;
    virtual bool isSensorPZActive() = 0;

    virtual void setMotorXStep(CommandStep step) = 0;
    virtual void setMotorYStep(CommandStep step) = 0;
    virtual void setMotorZStep(CommandStep step) = 0;

    virtual void send() = 0;

};

#endif // CORE_IMACHINE_H
