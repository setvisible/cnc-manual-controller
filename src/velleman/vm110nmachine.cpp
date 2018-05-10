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

#include "vm110nmachine.h"

#include "dll_velleman.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

/**
 * \class VM110NMachine
 * \brief The class VM110NMachine provides methods to use the Machine
 * controlled by a Velleman VM110N USB Card.
 *
 * It converts the CNC commands to the Velleman USB code.
 *
 */

VM110NMachine::VM110NMachine(QObject *parent) : IMachine(parent)
  , m_dll(new DLL_Velleman())
{
}

VM110NMachine::~VM110NMachine()
{
    delete m_dll;
}

/******************************************************************************
 ******************************************************************************/
bool VM110NMachine::openDevice()
{
    Q_ASSERT(m_dll);
    int cardAddress = 0; //0, 1, 2 or 3

    int h = m_dll->OpenDevice(cardAddress);
    switch (h) {
    case 0:
    case 1:
    case 2:
    case 3:
        qDebug() << "Card " << h << " connected";
        break;
    case -1:
    default:
        qDebug() << "Card " << cardAddress << " not found";
        return false;
        break;
    }
    return true;
}

/******************************************************************************
 ******************************************************************************/
void VM110NMachine::closeDevice()
{
    Q_ASSERT(m_dll);
    m_dll->CloseDevice();
}
