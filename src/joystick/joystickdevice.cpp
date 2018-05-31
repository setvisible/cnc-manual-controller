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

#include "joystickdevice.h"

#include "SDL/SDL.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

#define POLL_INTERVAL 40

/*! \class JoystickDevice
 * \brief The class JoystickDevice is just a wrapper to the SDL_Joystick class.
 */

/*!
 * \brief Constructor
 */
JoystickDevice::JoystickDevice(QObject *parent) : QObject(parent)
  , m_joystick(Q_NULLPTR)
  , m_deviceIndex(-1)
{
    /*
     * Sure, we're only using the Joystick,
     * but SDL doesn't work if video isn't initialised
     */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

JoystickDevice::~JoystickDevice()
{
    m_axis.clear();
    m_buttons.clear();
    if (m_joystick) {
        SDL_JoystickClose(m_joystick);
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    this->deleteLater();
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Count the number of joysticks attached to the system.
 */
int JoystickDevice::availableJoystickCount() const
{
    return SDL_NumJoysticks();
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Get the device index of the current joystick.
 */
int JoystickDevice::joystickIndex() const
{
    return m_deviceIndex;
//    Q_ASSERT(m_joystick);
//    return SDL_JoystickIndex(m_joystick);
}

void JoystickDevice::setJoystickIndex(int deviceIndex)
{
    m_deviceIndex = deviceIndex;
}

/******************************************************************************
 ******************************************************************************/
void JoystickDevice::open()
{
    Q_ASSERT(m_deviceIndex < availableJoystickCount());
    Q_ASSERT(m_deviceIndex >= 0);
    // close(); /* Otherwise, segfault */
    m_joystick = SDL_JoystickOpen(m_deviceIndex);
}

void JoystickDevice::close()
{
    if (m_joystick && SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_JoystickClose(m_joystick);
    }
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Get the implementation dependent name of a joystick.
 */
QString JoystickDevice::joystickName(int deviceIndex) const
{
    Q_ASSERT(deviceIndex < availableJoystickCount());
    Q_ASSERT(deviceIndex >= 0);
    return QString(SDL_JoystickName(deviceIndex));
}

int JoystickDevice::joystickNumAxes(int deviceIndex) const
{
    Q_ASSERT(deviceIndex < availableJoystickCount());
    Q_ASSERT(deviceIndex >= 0);
    return (SDL_JoystickNumAxes(m_joystick));
}

int JoystickDevice::joystickNumButtons(int deviceIndex) const
{
    Q_ASSERT(deviceIndex < availableJoystickCount());
    Q_ASSERT(deviceIndex >= 0);
    return (SDL_JoystickNumButtons(m_joystick));
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Read and store the current state
 * of the axis controls and buttons on the current joystick.
 *
 * Use methods axis() and button() to get the stored state.
 */
void JoystickDevice::storeCurrentState()
{
    m_axis.clear();
    m_buttons.clear();
    SDL_Event event;
    SDL_PollEvent(&event);
    for (int i = 0; i < SDL_JoystickNumAxes(m_joystick); ++i) {
        m_axis.append(SDL_JoystickGetAxis(m_joystick,i));
    }
    for (int i = 0; i < SDL_JoystickNumButtons(m_joystick); ++i) {
        m_buttons.append(SDL_JoystickGetButton(m_joystick,i));
    }
}

/*!
 * \brief Get the current state of axis controls on the current joystick.
 */
QList<int> JoystickDevice::axis() const
{
    return m_axis;
}

/*!
 * \brief Get the current state of buttons on the current joystick.
 */
QList<bool> JoystickDevice::buttons() const
{
    return m_buttons;
}
