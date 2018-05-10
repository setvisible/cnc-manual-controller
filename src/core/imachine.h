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

};

#endif // CORE_IMACHINE_H
