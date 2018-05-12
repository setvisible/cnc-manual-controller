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

#include "scheduler.h"

#include "engine.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QTime>
#include <QtCore/QTimerEvent>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif


/*! \class Scheduler
 * \brief The class Scheduler is a clock that schedules the ticks of the CNC machine.
 *
 * In order to perform real-time control, this clock must have a safe
 * and lock-free thread synchronisation with the other threads,
 * in particular the GUI threads and I/O threads.
 *
 * The Scheduler's thread must not block, so itruns on a dedicated thread,
 * so that the clock is as 'close to the metal' as possible.
 *
 */

Scheduler::Scheduler(QObject *parent) : QObject (parent)
  , m_interval(10)
  , m_timerId(-1)
  , m_monitoringTime(Q_NULLPTR)
  , m_monitoringPreciseTimer(Q_NULLPTR)
  , m_engine(Q_NULLPTR)
  , m_callback(Q_NULLPTR)
{
    /* -= IMPORTANT =-
     *
     * Do NEVER allocate heap objects (using new) in the constructor
     * of the QObject class as this allocation is then performed
     * on the main thread and not on the new QThread instance,
     * meaning that the newly created object is then owned by the
     * main thread and not the QThread instance.
     *
     * This will make the code fail to work.
     *
     * Instead, allocate such resources in the main function slot
     * such as process() in this case as when that is called
     * the object will be on the new thread instance
     * and thus it will own the resource.
     */
}

Scheduler::~Scheduler()
{
    /*
     * Free resources
     */
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Set the interval between two tick in milliseconds
 *
 * Remark: According to Qt documentation:
 * Windows 2000 has 15 millisecond accuracy;
 * other systems that we have tested can handle 1 millisecond intervals.
 */
void Scheduler::setInterval(int msec)
{
    /* On Qt and on Windows, when your timer timeout is "short enough"
     * (<= 10ms IIRC), Qt will force system's tick interval to ~1ms
     * (either 1000Hz or 1024Hz).
     */
    Q_ASSERT(msec >= 1);
    Q_ASSERT(msec <= 1000);
    m_interval = msec;
}

/******************************************************************************
 ******************************************************************************/
void Scheduler::setEngine(Engine *engine)
{
    m_engine = engine;
}

/*! \brief Set the function pointer.
 * The Scheduler calls it back at each tick.
 */
void Scheduler::setCallback(void (Engine::*fp)(void))
{
    m_callback = fp;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Start processing.
 */
void Scheduler::process()
{
    /*
     * -= Remarks =-
     * Allocate resources using new here
     */
    Q_ASSERT_X(m_engine, Q_FUNC_INFO, "Need an engine." );
    Q_ASSERT_X(m_callback, Q_FUNC_INFO, "Need a callback function." );

    m_monitoringTime = new QTime();
    m_monitoringTime->restart();

    m_monitoringPreciseTimer = new QElapsedTimer();
    m_monitoringPreciseTimer->restart();

    /* Some additional info. */
    if (m_monitoringPreciseTimer) {
        switch (m_monitoringPreciseTimer->clockType()) {
        case QElapsedTimer::SystemTime: qDebug() << "Clock Type: SystemTime"; break;
        case QElapsedTimer::MonotonicClock: qDebug() << "Clock Type: MonotonicClock"; break;
        case QElapsedTimer::TickCounter: qDebug() << "Clock Type: TickCounter"; break;
        case QElapsedTimer::MachAbsoluteTime: qDebug() << "Clock Type: MachAbsoluteTime"; break;
        case QElapsedTimer::PerformanceCounter: qDebug() << "Clock Type: PerformanceCounter"; break;
        default:
            break;
        }
    }

    /* Start */
    m_timerId = this->startTimer(m_interval, Qt::PreciseTimer);
}


void Scheduler::stop()
{
    this->killTimer(m_timerId);
    emit finished();
}

/******************************************************************************
 ******************************************************************************/
void Scheduler::timerEvent(QTimerEvent *event)
{
    if (m_timerId == event->timerId()) {

        /*
         * Measure the time elapsed since last tick.
         * We use different timers to assess the delay between two ticks,
         * so the scheduler thread's accurency.
         */
        const int elapsed = m_monitoringTime->elapsed();
        m_monitoringTime->restart();
        const qreal nsecsElapsed = m_monitoringPreciseTimer->nsecsElapsed() / 1000000.0;
        m_monitoringPreciseTimer->restart();
        const QString msg1 = tr("%0").arg(elapsed).rightJustified(4);
        const QString msg2 = tr("%0").arg(nsecsElapsed, 0, 'f', 3).rightJustified(8);
        const QString message = tr("Elapsed %0 msec (or %1 msec)").arg(msg1).arg(msg2);

        /// \todo probably not lock-free and congesting message stack if too many messages
        emit info(message);

        /*
         * Execute callback.
         *
         * Remark:
         * The callback function must be fast, in order to return
         * before the next interval.
         */
        (m_engine->*m_callback)();
    }
}

