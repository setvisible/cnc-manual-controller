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

#include "process.h"

#include "circularbuffer.h"


#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

#define C_MIN_INTERVAL_MSEC  10


/*! \class Process
 * \brief The class Process runs a circular buffer in a dedicated
 * real-time thread.
 *
 */

/*!
 * \brief Constructor
 */
Process::Process()
    : m_circularBuffer(new CircularBuffer())
    , m_continue(false)
    , m_interval(C_MIN_INTERVAL_MSEC)
{

}

Process::~Process()
{
    stop();
    delete m_circularBuffer;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief This property holds the timeout interval in milliseconds.
 */
int Process::interval() const
{
    return m_interval;
}

void Process::setInterval(int msec)
{
    if (msec >= C_MIN_INTERVAL_MSEC) {
        m_interval = msec;
    } else {
        m_interval = C_MIN_INTERVAL_MSEC;
    }
}

/******************************************************************************
 ******************************************************************************/
void Process::start()
{
//    Q_ASSERT(m_circularBuffer);


}

/******************************************************************************
 ******************************************************************************/
void Process::stop()
{
    m_continue = false;
}


