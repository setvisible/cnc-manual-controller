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

#include "axiscontrolwidget.h"
#include "ui_axiscontrolwidget.h"

AxisControlWidget::AxisControlWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::AxisControlWidget)
  , m_commandMode(CommandMode::PULSE)
  , m_previousCommandValue(0)
{
    ui->setupUi(this);

    connect(ui->buttonPlus, SIGNAL(released()), this, SLOT(increment()));
    connect(ui->buttonMinus, SIGNAL(released()), this, SLOT(decrement()));
    connect(ui->buttonHome, SIGNAL(released()), this, SLOT(home()));
    connect(ui->buttonStop, SIGNAL(released()), this, SLOT(stop()));
    connect(ui->checkBoxContinuous, SIGNAL(toggled(bool)), this, SLOT(toggleContinuous(bool)));
    connect(ui->lineEditCounter, SIGNAL(editingFinished()), this, SLOT(filterCounter()));
}

AxisControlWidget::~AxisControlWidget()
{
    delete ui;
}

void AxisControlWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Return an integer storing the number and the direction of the step.
 * \return the steps number and direction
 *    0 -> Stop
 *   17 -> Increment by 17 steps
 *  -68 -> Decrement by 68 steps
 */
int AxisControlWidget::commandValue() const
{
    if (ui->checkBoxContinuous->isChecked()) {  /* Continuous incrementing. */
        return m_previousCommandValue;

    } else {                                    /* Manual incrementing. */
        /* Remark:
         * We increment and decrement one step by one
         * so the returned value is 1 or -1.
         */
        if (ui->buttonPlus->isDown()) {
            return 1;
        } else if (ui->buttonMinus->isDown()) {
            return -1;
        } else {
            return 0;
        }
    }
}

/******************************************************************************
 ******************************************************************************/
QString AxisControlWidget::title() const
{
    return ui->labelTitle->text();
}

void AxisControlWidget::setTitle(QString title)
{
    ui->labelTitle->setText(title);
}

/******************************************************************************
 ******************************************************************************/
CommandMode AxisControlWidget::commandMode() const
{
    return m_commandMode;
}

void AxisControlWidget::setCommandMode(CommandMode commandMode)
{
    m_commandMode = commandMode;

    switch(m_commandMode)
    {
    case CommandMode::PULSE:
        setCheckableButtons(false);
        ui->lineEditCounter->setEnabled(false);
        break;

    case CommandMode::TOGGLE:
        setCheckableButtons(true);
        ui->lineEditCounter->setEnabled(false);
        break;

    case CommandMode::COUNTER:
    case CommandMode::DIAGNOSTIC:
    default:
        setCheckableButtons(false);
        ui->lineEditCounter->setEnabled(true);
    }
    stop(); //stop all movements in case
}


/******************************************************************************
 ******************************************************************************/
void AxisControlWidget::increment()
{
    m_previousCommandValue = 1;
    if (m_commandMode == CommandMode::TOGGLE) {
        untoggleButtons();
        ui->buttonPlus->setChecked( true );
    }
    emit incremented();
}
void AxisControlWidget::decrement()
{
    m_previousCommandValue = -1;
    if (m_commandMode == CommandMode::TOGGLE) {
        untoggleButtons();
        ui->buttonMinus->setChecked( true );
    }
    emit decremented();
}

void AxisControlWidget::home()
{
    m_previousCommandValue = 0;
    if (m_commandMode == CommandMode::TOGGLE) {
        untoggleButtons();
        ui->buttonHome->setChecked(true);
    }
    emit homeAsked();
}

void AxisControlWidget::stop()
{
    m_previousCommandValue = 0;
    if (m_commandMode == CommandMode::TOGGLE) {
        untoggleButtons();
        ui->buttonStop->setChecked(true);
    }
    emit stopped();
}

void AxisControlWidget::toggleContinuous(bool)
{
    m_previousCommandValue = 0;
}

void AxisControlWidget::filterCounter()
{
    bool ok;
    double c = ui->lineEditCounter->text().toDouble(&ok);
    if (ok) {
        c = qAbs( c );
        c = qBound(0.00, c, 9999.99);
        ui->lineEditCounter->setText(QString("%0").arg( c ));
    } else {
        ui->lineEditCounter->setText(QLatin1String("0.00"));
    }
}

/******************************************************************************
 ******************************************************************************/
void AxisControlWidget::setCheckableButtons(bool checkable)
{
    ui->buttonPlus->setCheckable(checkable);
    ui->buttonStop->setCheckable(checkable);
    ui->buttonHome->setCheckable(checkable);
    ui->buttonMinus->setCheckable(checkable);

}
void AxisControlWidget::untoggleButtons()
{
    ui->buttonPlus->setChecked(false);
    ui->buttonStop->setChecked(false);
    ui->buttonHome->setChecked(false);
    ui->buttonMinus->setChecked(false);
    update();
}
