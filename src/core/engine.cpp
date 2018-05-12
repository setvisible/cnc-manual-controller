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
#include "scheduler.h"

#include <Velleman/VM110NMachine>

#include <QtCore/QThread>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

/*! \class Engine
 * \brief The class Engine sends instructions to the CNC machine.
 *
 * It controls the real-time thread where the scheduler runs.
 */

/*!
 * \brief Constructor
 */
Engine::Engine(QObject *parent) : QObject(parent)
  , m_process(new Process())
  , m_machine(new VM110NMachine(this))
  , m_isConnected(false)
  , m_interval(10)
{
    reset();
}


void Engine::reset()
{
    stop();
    setConnected(false);
    setTicksPerSecond(100);
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
        stop();
        m_machine->closeDevice();
    }
    m_isConnected = checked;
    if (m_isConnected) {
        // try to connect
        bool isOpen = m_machine->openDevice();
        start();

    }
    emit connectedChanged(m_isConnected);
}

/******************************************************************************
 ******************************************************************************/
int Engine::ticksPerSecond() const
{
    return m_interval;
}

void Engine::setTicksPerSecond(int ticksPerSecond)
{
    m_interval = 1000 / ticksPerSecond; // in msec
    emit ticksPerSecondChanged(ticksPerSecond);
}

/******************************************************************************
 ******************************************************************************/
void Engine::start()
{
    /* Create a real-time thread. */
    QThread *thread = new QThread;

    /* Create and move the scheduler to its dedicated thread. */
    Scheduler *worker = new Scheduler();
    worker->moveToThread(thread);
    worker->setInterval(m_interval);
    worker->setEngine(this);
    worker->setCallback(&Engine::commandCallback);

    // Hooks up the error message signal from the worker
    // to an error processing function in the main thread.
    connect(worker, SIGNAL(info(QString)), this, SLOT(infoString(QString)));

    // Connects the thread’s started() signal to the processing() slot
    // in the scheduler, causing it to start.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    /* Clean-up */
    /*
     * Then the clean-up: when the worker instance emits finished(),
     * as we did in the example, it will signal the thread to quit,
     * i.e. shut down. We then mark the worker instance using the same finished()
     *  signal for deletion. Finally, to prevent nasty crashes
     * because the thread hasn’t fully shut down yet when it is deleted,
     * we connect the finished() of the thread (not the worker!)
     *  to its own deleteLater() slot.
     *
     * This will cause the thread to be deleted only
     * after it has fully shut down.
     */
    connect(this, SIGNAL(stopped()), worker, SLOT(stop()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start(QThread::TimeCriticalPriority);
    // Rem: Qt says use thread->exec(); for non-GUI threads?

    emit started();
}

void Engine::stop()
{
    emit stopped();
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Command Thread
 */
void Engine::commandCallback() noexcept
{
}

