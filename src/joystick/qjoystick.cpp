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

#include "qjoystick.h"

#include "SDL/SDL.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

#define POLL_INTERVAL 40

/*! \class QJoystick
 * \brief The class QJoystick is just a wrapper to the SDL_Joystick class.
 */

/*!
 * \brief Constructor
 */
QJoystick::QJoystick(QObject *parent) : QObject(parent)
    , m_joystick(Q_NULLPTR)
{
    /*
     * Sure, we're only using the Joystick,
     * but SDL doesn't work if video isn't initialised
     */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

QJoystick::~QJoystick()
{
    axis.clear();
    buttons.clear();
    if (m_joystick) {
        SDL_JoystickClose(m_joystick);
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    this->deleteLater();
}

/******************************************************************************
 ******************************************************************************/
int QJoystick::availableJoysticks() const
{
    return SDL_NumJoysticks();
}

int QJoystick::currentJoystick() const
{
    Q_ASSERT(m_joystick);
    return SDL_JoystickIndex(m_joystick);
}

QString QJoystick::joystickName(int js) const
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return QString(SDL_JoystickName(js));
}

int QJoystick::joystickNumAxes(int js) const
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return (SDL_JoystickNumAxes(m_joystick));
}

int QJoystick::joystickNumButtons(int js) const
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return (SDL_JoystickNumButtons(m_joystick));
}

void QJoystick::getData()
{
    axis.clear();
    buttons.clear();
    SDL_Event event;
    SDL_PollEvent(&event);
    for (int i = 0; i < SDL_JoystickNumAxes(m_joystick); ++i) {
        axis.append(SDL_JoystickGetAxis(m_joystick,i));
    }
    for (int i = 0; i < SDL_JoystickNumButtons(m_joystick); ++i) {
        buttons.append(SDL_JoystickGetButton(m_joystick,i));
    }
}

/******************************************************************************
 ******************************************************************************/
void QJoystick::setJoystick(int js)
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    // /* Otherwise, segfault */
    // if (m_joystick && SDL_WasInit(SDL_INIT_JOYSTICK)) {
    //     SDL_JoystickClose(m_joystick);
    // }
    m_joystick = SDL_JoystickOpen(js);
}
