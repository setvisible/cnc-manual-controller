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

#ifndef WIDGETS_AXIS_CONTROL_WIDGET_H
#define WIDGETS_AXIS_CONTROL_WIDGET_H

#include <QWidget>

namespace Ui {
class AxisControlWidget;
}

enum class CommandMode {
    PULSE,
    TOGGLE,
    COUNTER,
    DIAGNOSTIC
};

class AxisControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AxisControlWidget(QWidget* parent = Q_NULLPTR);
    ~AxisControlWidget();

    QString axisTitle() const;
    void setAxisTitle(QString title);

    CommandMode commandMode() const;
    void setCommandMode(CommandMode mode);

Q_SIGNALS:
    void stopped();
    void homeAsked();
    void incremented();
    void decremented();

protected:
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void stop();
    void home();
    void increment();
    void decrement();
    void filterCounter();

private:
    Ui::AxisControlWidget *ui;
    CommandMode m_commandMode;

    void setCheckableButtons(bool value);
    void untoggleButtons();

};

#endif // WIDGETS_AXIS_CONTROL_WIDGET_H
