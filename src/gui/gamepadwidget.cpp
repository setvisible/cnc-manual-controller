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

#include "gamepadwidget.h"
#include "ui_gamepadwidget.h"

#include <Joystick/JoystickDevice>
#include <Widgets/GamepadAxisWidget>

#include <QtCore/QtMath>
#include <QtCore/QTimer>

GamepadWidget::GamepadWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::GamepadWidget)
  , m_joystickDevice(new JoystickDevice(this))
  , m_timer(new QTimer())
  , m_deviceIndex(-1)
{
    ui->setupUi(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateState()));

    m_timer->setInterval(50);

    ui->gamepadXWidget->setTitle(QLatin1String("X"));
    ui->gamepadYWidget->setTitle(QLatin1String("Y"));
    ui->gamepadZWidget->setTitle(QLatin1String("Z"));

    setActive(false);
}

GamepadWidget::~GamepadWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
int GamepadWidget::deviceIndex() const
{
    return m_deviceIndex;
}

void GamepadWidget::setDeviceIndex(int deviceIndex)
{
    if (m_deviceIndex >= 0) {
        m_joystickDevice->close();
    }
    m_deviceIndex = -1;

    const int count = m_joystickDevice->availableJoystickCount();
    if (deviceIndex >= 0 && deviceIndex < count) {

        /* We assume the joystick has at least 4 axes. */
        m_joystickDevice->setJoystickIndex(deviceIndex);
        m_joystickDevice->open();
        if (m_joystickDevice->joystickNumAxes(deviceIndex) >= 4) {
            m_deviceIndex = deviceIndex;
            const QString name = m_joystickDevice->joystickName(m_deviceIndex);
            ui->deviceNameLabel->setText(tr("(device: %0)").arg(name));
        } else {
            ui->deviceNameLabel->setText(tr("(wrong device: need 4 axes)"));
        }
    }
}

/******************************************************************************
 ******************************************************************************/
bool GamepadWidget::isActive() const
{
    return true;
}

void GamepadWidget::setActive(bool active)
{
    if (active && m_deviceIndex >= 0) {
        m_timer->start();
        ui->infoDetailsLabel->setText(QLatin1String("Yes"));
    } else {
        m_timer->stop();
        ui->infoDetailsLabel->setText(QLatin1String("No"));
    }
}

/******************************************************************************
 ******************************************************************************/
void GamepadWidget::updateState()
{
    if (!m_joystickDevice || m_deviceIndex < 0) {
        return;
    }
    m_joystickDevice->storeCurrentState();

    Q_ASSERT(m_joystickDevice->axis().count() >= 4); // assume 4 axes

    // const double maxBound =  32767.0;
    // const double minBound = -32768.0;

    m_joystickDevice->storeCurrentState();
    const double x = (double) m_joystickDevice->axis().at(3);
    const double y = (double) m_joystickDevice->axis().at(2);
    const double z = (double) m_joystickDevice->axis().at(1);

    int xValue =  qCeil(5.0 * x / 32768.0);
    int yValue = -qCeil(5.0 * y / 32768.0);
    int zValue = -qCeil(5.0 * z / 32768.0);

    ui->gamepadXWidget->setValue(xValue);
    ui->gamepadYWidget->setValue(yValue);
    ui->gamepadZWidget->setValue(zValue);
}

/******************************************************************************
 ******************************************************************************/
int GamepadWidget::valueAxisX() const
{
    return ui->gamepadXWidget->value();
}

int GamepadWidget::valueAxisY() const
{
    return ui->gamepadYWidget->value();
}

int GamepadWidget::valueAxisZ() const
{
    return ui->gamepadZWidget->value();
}

