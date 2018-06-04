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

#include "circularbuffer.h"
#include "scheduler.h"

#include <Core/Preferences>
#include <GUI/ControlWidget>
#include <GUI/GamepadWidget>
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
  , m_circularBuffer(new CircularBuffer())
  , m_machine(new VM110NMachine(this))
  , m_isConnected(false)
  , m_interval(10)
  , m_controlWidget(Q_NULLPTR)
  , m_gamepadWidget(Q_NULLPTR)
  , m_preferences(new Preferences(this))
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
Preferences* Engine::preferences() const
{
    return m_preferences;
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
        stop();
        m_machine->closeDevice();
    }
    m_isConnected = checked;
    if (m_isConnected) {
        m_machine->openDevice();
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
    if (ticksPerSecond < 1) {
        ticksPerSecond = 1;
    }
    if (ticksPerSecond > 1000) {
        ticksPerSecond = 1000;
    }
    Q_ASSERT(ticksPerSecond >= 1);
    Q_ASSERT(ticksPerSecond <= 1000);
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
/*!
 * \remark This method is run in the command thread,
 * so we have to **guarantee** that:
 *
 * \li this function will return in time < buffer length
 * \li will finish processing the buffer
 * \li the output buffer will contain __valid command data__ afterwards
 * \li there will be no error / exception / ...
 *
 * Otherwise, we will cause a glitch.
 */
void Engine::commandCallback() noexcept
{
    /* Read the VM110N input and write the value in Circular Buffer. */

    static CommandBuffer m_commandBuffer;
    static int m_frameIndex = -1;
    static bool m_descendingEdge = false;

    m_descendingEdge = !m_descendingEdge;

    m_machine->receive();

    /* if a limit is reached or Emergency Stop pushed, stops the program */
    if (m_machine->isEmergencyActive() ) {
        errorString(tr("Emergency Stop"));
        stop();
        return;
    }

    if ( m_machine->hasSensorActive()) {
        // recovery mode?
        errorString(tr("Reach limit. Stop"));
        stop();
        return;
    }

    if (m_descendingEdge) {
        m_frameIndex++;
        if (m_frameIndex > 3) {
            m_frameIndex = 0;

            if (m_circularBuffer->isEmpty()) {

                readInput();
                // not good: thread can block

                /* Pop the Circular Buffer */
                m_commandBuffer = m_circularBuffer->pop();

            } else {
                /* if empty, just do nothing */
                m_commandBuffer = CommandBuffer();
            }
        }
        CommandFrame frame = m_commandBuffer.frames[m_frameIndex];
        m_machine->setMotorXStep(frame.actuatorX);
        m_machine->setMotorYStep(frame.actuatorY);
        m_machine->setMotorZStep(frame.actuatorZ);
    } else {
        m_machine->setMotorXStep(CommandStep::None);
        m_machine->setMotorYStep(CommandStep::None);
        m_machine->setMotorZStep(CommandStep::None);
    }

    /* Send the command to the machine. */
    m_machine->send();
}

/******************************************************************************
 ******************************************************************************/

void Engine::infoString(QString message)
{
    //qDebug() << Q_FUNC_INFO << message;
}

void Engine::errorString(QString err)
{
    qWarning() << Q_FUNC_INFO << err;
}

/******************************************************************************
 ******************************************************************************/
void Engine::setControlInput(ControlWidget *controlWidget)
{
    m_controlWidget = controlWidget;
}

void Engine::setGamepadInput(GamepadWidget *gamepadWidget)
{
    m_gamepadWidget = gamepadWidget;
}

/******************************************************************************
 ******************************************************************************/
void Engine::readInput()
{
    const bool hasGamepad = m_gamepadWidget->deviceIndex() >= 0;
    if (hasGamepad) {
        readGamepadInput();
    } else {
        readControlInput();
    }
}

/******************************************************************************
 ******************************************************************************/
static inline CommandStep toStep(const int value)
{
    if (value > 0) {
        return CommandStep::Increment;
    } else if (value < 0) {
        return CommandStep::Decrement;
    } else {
        return CommandStep::None;
    }
}

void Engine::readControlInput()
{
    CommandStep commandX = CommandStep::None;
    CommandStep commandY = CommandStep::None;
    CommandStep commandZ = CommandStep::None;
    if (m_controlWidget) {
        commandX = toStep(m_controlWidget->valueAxisX());
        commandY = toStep(m_controlWidget->valueAxisY());
        commandZ = toStep(m_controlWidget->valueAxisZ());
    }
    CommandBuffer buffer;
    for (int i = 0; i < 4; ++i) {
        buffer.frames[i].actuatorX = commandX;
        buffer.frames[i].actuatorY = commandY;
        buffer.frames[i].actuatorZ = commandZ;
    }
    m_circularBuffer->push(buffer);
    qDebug() << Q_FUNC_INFO << buffer;
}

/******************************************************************************
 ******************************************************************************/
static inline CommandStep toStep(const int step, const int iterator, CommandStep command)
{
    const int absStep = qAbs(step);
    Q_ASSERT(absStep <= 5);
    Q_ASSERT(iterator >= 0);
    Q_ASSERT(iterator <= 3);
    if ((iterator * 5) <= (step * 3)) {
        return command;
    } else {
        return CommandStep::None;
    }
}

void Engine::readGamepadInput()
{
    Q_ASSERT(m_gamepadWidget);
    const int stepX = m_gamepadWidget->valueAxisX();
    const int stepY = m_gamepadWidget->valueAxisY();
    const int stepZ = m_gamepadWidget->valueAxisZ();
    const CommandStep commandX = toStep(stepX);
    const CommandStep commandY = toStep(stepY);
    const CommandStep commandZ = toStep(stepZ);
    CommandBuffer buffer;
    for (int i = 0; i < 4; ++i) {
        buffer.frames[i].actuatorX = toStep(stepX, i, commandX);
        buffer.frames[i].actuatorY = toStep(stepY, i, commandY);
        buffer.frames[i].actuatorZ = toStep(stepZ, i, commandZ);
    }
    m_circularBuffer->push(buffer);
    qDebug() << Q_FUNC_INFO << buffer;
}
