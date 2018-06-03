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

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <Core/Engine>
#include <Core/Preferences>
#include <Joystick/SandBoxWidget>

PreferencesDialog::PreferencesDialog(Engine *engine, QWidget *parent) : QDialog(parent)
  , ui(new Ui::PreferencesDialog)
  , m_preferences(Q_NULLPTR)
{
    ui->setupUi(this);

    Q_ASSERT(engine);
    m_preferences = engine->preferences();

    Q_ASSERT(m_preferences);
    ui->sandBoxWidget->setJoystickId(m_preferences->currentJoystickId());
    ui->sandBoxWidget->setSamplingActive(true);

}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::accept()
{
    Q_ASSERT(m_preferences);
    m_preferences->setCurrentJoystickId(ui->sandBoxWidget->joystickId());

    QDialog::accept();
}
