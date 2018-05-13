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

#define C_SENSOR_EMERGENCY_CHANNEL  0
#define C_SENSOR_X_CHANNEL          1
#define C_SENSOR_Y_CHANNEL          2
#define C_SENSOR_Z_CHANNEL          3
#define C_SENSOR_DIR_CHANNEL        4

#define C_MOTOR_X_DIR_CHANNEL       2
#define C_MOTOR_X_STEP_CHANNEL      3
#define C_MOTOR_Y_DIR_CHANNEL       4
#define C_MOTOR_Y_STEP_CHANNEL      5
#define C_MOTOR_Z_DIR_CHANNEL       6
#define C_MOTOR_Z_STEP_CHANNEL      7


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
  , m_cardAdress(0)
{
    setCardAddress(true, true);
}

VM110NMachine::~VM110NMachine()
{
    delete m_dll;
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Set the card address of the VM110N (K8055N) device.
 *
 * The card address settings must be done before the USB cable is connected
 * to the card.
 *
 * \code
 * this->setCardAddress(true, false) // Card jumpers: SK5 is ON, SK6 is OFF
 * \endcode
 */
inline void VM110NMachine::setCardAddress(bool sk5, bool sk6)
{
    m_cardAdress = 3 - (2 * int(sk5) + int(sk6)); //0, 1, 2 or 3
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Opens the communication link to the VM110N (K8055N) device.
 */
bool VM110NMachine::openDevice()
{
    Q_ASSERT(m_dll);
    int h = m_dll->OpenDevice(m_cardAdress);
    switch (h) {
    case 0:
    case 1:
    case 2:
    case 3:
        m_dll->ClearAllDigital(); // Set all output to zero.
        qDebug() << "Card " << h << " connected";
        break;
    case -1:
    default:
        qDebug() << "Card " << m_cardAdress << " not found";
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

/******************************************************************************
 ******************************************************************************/
/*! \brief Read the digital input channels
 */
void VM110NMachine::receive()
{
    Q_ASSERT(m_dll);
    const int i = m_dll->ReadAllDigital();
    m_sensorChannel[0] = (i & 1) > 0;
    m_sensorChannel[1] = (i & 2) > 0;
    m_sensorChannel[2] = (i & 4) > 0;
    m_sensorChannel[3] = (i & 8) > 0;
    m_sensorChannel[4] = (i & 16) > 0;
}

/******************************************************************************
 ******************************************************************************/
bool VM110NMachine::isEmergencyActive()
{
    return m_sensorChannel[C_SENSOR_EMERGENCY_CHANNEL];
}

bool VM110NMachine::hasSensorActive()
{
    return  m_sensorChannel[C_SENSOR_X_CHANNEL] ||
            m_sensorChannel[C_SENSOR_Y_CHANNEL] ||
            m_sensorChannel[C_SENSOR_Z_CHANNEL];
}

bool VM110NMachine::isSensorMXActive()
{
    return m_sensorChannel[C_SENSOR_X_CHANNEL] && !direction();
}

bool VM110NMachine::isSensorMYActive()
{
    return m_sensorChannel[C_SENSOR_Y_CHANNEL] && !direction();
}

bool VM110NMachine::isSensorMZActive()
{
    return m_sensorChannel[C_SENSOR_Z_CHANNEL] && !direction();
}

bool VM110NMachine::isSensorPXActive()
{
    return m_sensorChannel[C_SENSOR_X_CHANNEL] && direction();
}

bool VM110NMachine::isSensorPYActive()
{
    return m_sensorChannel[C_SENSOR_Y_CHANNEL] && direction();
}

bool VM110NMachine::isSensorPZActive()
{
    return m_sensorChannel[C_SENSOR_Z_CHANNEL] && direction();
}

inline bool VM110NMachine::direction()
{
    return m_sensorChannel[C_SENSOR_DIR_CHANNEL];
}

/******************************************************************************
 ******************************************************************************/
void VM110NMachine::setMotorXStep(int step)
{
    if (step > 0) {
        m_actuatorChannel[C_MOTOR_X_DIR_CHANNEL] = false;
        m_actuatorChannel[C_MOTOR_X_STEP_CHANNEL] = true;
    } else if (step < 0) {
        m_actuatorChannel[C_MOTOR_X_DIR_CHANNEL] = true;
        m_actuatorChannel[C_MOTOR_X_STEP_CHANNEL] = true;
    } else {
        m_actuatorChannel[C_MOTOR_X_STEP_CHANNEL] = false;
    }
}

void VM110NMachine::setMotorYStep(int step)
{
    if (step > 0) {
        m_actuatorChannel[C_MOTOR_Y_DIR_CHANNEL] = false;
        m_actuatorChannel[C_MOTOR_Y_STEP_CHANNEL] = true;
    } else if (step < 0) {
        m_actuatorChannel[C_MOTOR_Y_DIR_CHANNEL] = true;
        m_actuatorChannel[C_MOTOR_Y_STEP_CHANNEL] = true;
    } else {
        m_actuatorChannel[C_MOTOR_Y_STEP_CHANNEL] = false;
    }
}

void VM110NMachine::setMotorZStep(int step)
{
    if (step > 0) {
        m_actuatorChannel[C_MOTOR_Z_DIR_CHANNEL] = false;
        m_actuatorChannel[C_MOTOR_Z_STEP_CHANNEL] = true;
    } else if (step < 0) {
        m_actuatorChannel[C_MOTOR_Z_DIR_CHANNEL] = true;
        m_actuatorChannel[C_MOTOR_Z_STEP_CHANNEL] = true;
    } else {
        m_actuatorChannel[C_MOTOR_Z_STEP_CHANNEL] = false;
    }
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Write the digital output channels
 */
void VM110NMachine::send()
{
    Q_ASSERT(m_dll);
    int i = 0;
    i = i | (m_actuatorChannel[0] << 0);
    i = i | (m_actuatorChannel[1] << 1);
    i = i | (m_actuatorChannel[2] << 2);
    i = i | (m_actuatorChannel[3] << 3);
    i = i | (m_actuatorChannel[4] << 4);
    i = i | (m_actuatorChannel[5] << 5);
    i = i | (m_actuatorChannel[6] << 6);
    i = i | (m_actuatorChannel[7] << 7);
    m_dll->WriteAllDigital(i);
}
