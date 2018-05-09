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

#include "engine.h"

#include "process.h"

/*! \class Engine
 * \brief The class Engine sends instructions to the CNC machine.
 */

/*!
 * \brief Constructor
 */
Engine::Engine(QObject *parent) : QObject(parent)
  , m_process(new Process())
  , m_isConnected(false)
  , m_speed(100)
{
}


void Engine::reset()
{
    setConnected(false);
    setSpeed(100);
}

/******************************************************************************
 ******************************************************************************/
bool Engine::isConnected() const
{
    return m_isConnected;
}

void Engine::setConnected(bool checked)
{
    if (m_isConnected) {
        // disconnect first
    }
    m_isConnected = checked;
    if (m_isConnected) {
        // try to connect
    }
    emit connectedChanged(m_isConnected);
}

/******************************************************************************
 ******************************************************************************/
int Engine::speed() const
{
    return m_speed;
}

void Engine::setSpeed(int val)
{
    m_speed = val;
    emit speedChanged(m_speed);
}

