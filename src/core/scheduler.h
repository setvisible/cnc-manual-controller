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

#ifndef CORE_PROCESS_H
#define CORE_PROCESS_H

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QElapsedTimer;
class QTime;
QT_END_NAMESPACE

class Engine;

class Scheduler : public QObject
{
    Q_OBJECT
public:
    explicit Scheduler(QObject *parent = Q_NULLPTR);
    ~Scheduler();

    void setInterval(int msec);

    void setEngine(Engine *engine);
    void setCallback(void (Engine::*fp)(void));

public Q_SLOTS:
    void process();
    void stop();

Q_SIGNALS:
    void finished();
    void info(QString message);

protected:
      void timerEvent(QTimerEvent *event);

private:
    int m_interval;
    int m_timerId;
    QTime *m_monitoringTime;
    QElapsedTimer *m_monitoringPreciseTimer;

    Engine *m_engine;
    void (Engine::*m_callback)(void);
};


#endif // CORE_PROCESS_H
