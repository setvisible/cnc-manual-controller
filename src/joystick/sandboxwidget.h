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

#ifndef JOYSTICK_SETTING_WIDGET_H
#define JOYSTICK_SETTING_WIDGET_H

#include <QtWidgets/QWidget>

QT_BEGIN_INCLUDE_NAMESPACE
class QProgressBar;
class QCheckBox;
class QLabel;
QT_END_INCLUDE_NAMESPACE

class QJoystick;

namespace Ui {
class JoystickSettingWidget;
}

class JoystickSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JoystickSettingWidget(QWidget *parent = Q_NULLPTR);
    ~JoystickSettingWidget();

    int joystickId() const;
    void setJoystickId(int id);

    /// Structure created for each joystick encountered.
    struct JoystickData{
        int number_axes;
        int number_btn;
        QList<int> axis;
        QList<bool> button;
    };

    /// Available joystick count. Only set at initialization.
    int joysavail;

    /// List of joystick data
    QList<JoystickData*> joystick;

    /// current_joystick is dynamic, set by the user selection.
    int current_joystick;

    void pollJoystick();
    void exttoggle(bool);

private Q_SLOTS:
    void updateSelection(int index);
    void updateData();
    void toggleSampling(bool down);

private:
    Ui::JoystickSettingWidget *ui;
    QJoystick *m_joystick;
    QTimer *m_timer;
    QList<QProgressBar*> m_progressBars;
    QList<QLabel*> m_labels;
    QList<QCheckBox*> m_checkboxes;

    void init_joysticks();

};

#endif // JOYSTICK_SETTING_WIDGET_H
