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

#include "preferences.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

/*! \class Preferences
 * \brief The class Preferences stores the user's settings
 * and the parameters of the CNC machine.
 *
 */

Preferences::Preferences(QObject *parent) : QObject (parent)
  , m_currentJoystickId(0)
{
}

Preferences::~Preferences()
{
}

/******************************************************************************
 ******************************************************************************/
int Preferences::currentJoystickId() const
{
    return m_currentJoystickId;
}

void Preferences::setCurrentJoystickId(int id)
{
    m_currentJoystickId = id;
}

