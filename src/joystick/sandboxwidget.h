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

#ifndef JOYSTICK_SAND_BOX_WIDGET_H
#define JOYSTICK_SAND_BOX_WIDGET_H

#include <QtWidgets/QWidget>

QT_BEGIN_INCLUDE_NAMESPACE
class QProgressBar;
class QCheckBox;
class QLabel;
QT_END_INCLUDE_NAMESPACE

class JoystickDevice;

namespace Ui {
class SandBoxWidget;
}

class SandBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SandBoxWidget(QWidget *parent = Q_NULLPTR);
    ~SandBoxWidget();

    int joystickId() const;
    void setJoystickId(int id);

public Q_SLOTS:
    void setSamplingActive(bool active);

private Q_SLOTS:
    void updateSelection(int index);
    void updateState();

private:
    Ui::SandBoxWidget *ui;
    JoystickDevice *m_joystickDevice;
    QTimer *m_timer;
    QList<QProgressBar*> m_progressBars;
    QList<QLabel*> m_labels;
    QList<QCheckBox*> m_checkboxes;

    /** Structure created for each joystick encountered. */
    struct JoystickData{
        int number_axes;
        int number_btn;
        QList<int> axis;
        QList<bool> button;
    };

    /** Available joystick count. Only set at initialization. */
    int m_joystickCount;

    /** List of joystick data */
    QList<JoystickData*> m_joysticks;

    /** current_joystick is dynamic, set by the user selection. */
    int m_currentJoystickIndex;

    void reset();
    void pollJoystick();

};

#endif // JOYSTICK_SAND_BOX_WIDGET_H
