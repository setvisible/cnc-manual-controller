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

#include "sandboxwidget.h"
#include "ui_sandboxwidget.h"

#include "qjoystick.h"

#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QList>

/*! \class SandBoxWidget
 * \brief The class SandBoxWidget provides a GUI to setup the Joystick.
 */
SandBoxWidget::SandBoxWidget(QWidget *parent) : QWidget(parent)
  , ui(new Ui::SandBoxWidget)
  , m_joystick(new QJoystick(this))
  , m_timer(new QTimer())
{
    ui->setupUi(this);

    m_timer->setInterval(50);

    m_progressBars.clear();
    m_labels.clear();
    m_checkboxes.clear();

    /* Initialize the joysticks. */
    init_joysticks();

    /*
     * If there's a joystick present, initialize axes and buttons displays
     * and start the update timer.
     */
    if (joysavail > 0) {
        this->updateSelection(0);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(updateData()));
        connect(ui->avail_box, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSelection(int)));
        connect(ui->sample_box, SIGNAL(toggled(bool)), this, SLOT(toggleSampling(bool)));
        if (ui->sample_box->isChecked()) {
            m_timer->start();
        }
    }
}

SandBoxWidget::~SandBoxWidget()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
int SandBoxWidget::joystickId() const
{
    return 0;
}

void SandBoxWidget::setJoystickId(int id)
{
    Q_UNUSED(id);
}

/******************************************************************************
 ******************************************************************************/
// Initialize Joystick information
// Get # of joysticks available
// Populate # of axes and buttons
void SandBoxWidget::init_joysticks()
{
    ui->avail_box->clear();

    // Find number of joysticks present
    joysavail=m_joystick->availableJoysticks();

    // Create joysticks list
    for(int i=0;i<joysavail;i++){
        JoystickData *tempjoy;
        tempjoy = new JoystickData;
        joystick.append(tempjoy);
    }

    // Populate labels depending on the number of joysticks found
    switch (joysavail) {
    case 0:
        ui->avail_label->setText(QString("No joysticks found"));
        ui->avail_box->setDisabled(true);
        break;
    default:
        if(joysavail==1)
            ui->avail_label->setText(QString("%1 joystick found").arg(joysavail));
        else
            ui->avail_label->setText(QString("%1 joysticks found").arg(joysavail));

        // Populate data structure for all joysticks
        for(int i=0; i<joysavail;i++) {
            m_joystick->setJoystick(i);

            // Populate ComboBox
            ui->avail_box->addItem(m_joystick->joystickName(i));

            // Axes
            joystick.at(i)->number_axes = m_joystick->joystickNumAxes(i);

            for (int j = 0; j < joystick.at(i)->number_axes; ++j) {
                joystick.at(i)->axis.append(0);
            }

            // Buttons
            joystick.at(i)->number_btn  = m_joystick->joystickNumButtons(i);

            for (int j = 0; j < joystick.at(i)->number_btn; ++j) {
                joystick.at(i)->button.append(false);
            }
        }

        current_joystick = 0;
        ui->num_axbtnlabel->setText(QString("%1 axes - %2 buttons")
                                    .arg(joystick.at(0)->number_axes)
                                    .arg(joystick.at(0)->number_btn));

        m_joystick->setJoystick(ui->avail_box->currentIndex());

        break;
    }

}

/******************************************************************************
 ******************************************************************************/
/*! \brief Lay out progress bars for axes and buttons.
 */
void SandBoxWidget::updateSelection(int index)
{
    QLayoutItem *child = Q_NULLPTR;
    while ((child = ui->axes_Layout->takeAt(0)) != Q_NULLPTR) {
        delete child->widget();
        delete child;
    }

    for (int j = 0; j < m_progressBars.size(); ++j){
        m_progressBars.at(j)->deleteLater();
        m_labels.at(j)->deleteLater();

    }
    m_progressBars.clear();
    m_labels.clear();

    while (!ui->buttons_Layout->isEmpty()) {
        ui->buttons_Layout->removeWidget(ui->buttons_Layout->takeAt(0)->widget());
    }

    for (int j = 0; j < m_checkboxes.size(); ++j) {
        m_checkboxes.at(j)->deleteLater();
    }
    m_checkboxes.clear();

    current_joystick = index;

    ui->num_axbtnlabel->setText(QString("%1 axes - %2 buttons")
                                .arg(joystick.at(index)->number_axes)
                                .arg(joystick.at(index)->number_btn));
    m_joystick->setJoystick(ui->avail_box->currentIndex());

    for (int i = 0; i < joystick.at(index)->number_axes; ++i){

        QLabel *label = new QLabel(QString("0"), this);
        QFont font;
        font.setPointSize(9);
        label->setAlignment(Qt::AlignLeft);
        label->setFont(font);
        label->setFixedWidth(50);

        QProgressBar *progressbar = new QProgressBar(this);
        progressbar->setOrientation(Qt::Vertical);
        progressbar->setTextVisible(false);
        progressbar->setMinimum(-32768);
        progressbar->setMaximum(32768);
        progressbar->setValue(0);
        progressbar->setAlignment(Qt::AlignRight);

        m_progressBars.append(progressbar);
        m_labels.append(label);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(m_progressBars.at(i));
        layout->addWidget(m_labels.at(i));

        ui->axes_Layout->addLayout(layout);
    }

    for (int j = 0; j < joystick.at(current_joystick)->number_btn; ++j) {
        QCheckBox *checkbox = new QCheckBox(this);
        checkbox->setChecked(false);
        m_checkboxes.append(checkbox);
        ui->buttons_Layout->addWidget(m_checkboxes[j]);
    }
}

void SandBoxWidget::updateData()
{
    m_joystick->getData();
    pollJoystick();
    for (int i = 0; i < joystick.at(current_joystick)->number_axes; ++i) {
        m_progressBars[i]->setValue(joystick.at(current_joystick)->axis[i]);
        m_labels[i]->setText(QString("%1").arg(joystick.at(current_joystick)->axis[i]));
    }

    for (int i = 0; i < joystick.at(current_joystick)->number_btn; ++i) {
        m_checkboxes[i]->setChecked(joystick.at(current_joystick)->button[i]);
    }
    //this->update();
}

/******************************************************************************
 ******************************************************************************/
// Extracts data from QJoystick class
void SandBoxWidget::pollJoystick()
{
    m_joystick->getData();
    for (int i = 0; i < joystick.at(current_joystick)->number_axes; ++i){
        joystick.at(current_joystick)->axis[i] = m_joystick->axis[i];
    }

    for (int i = 0; i < joystick.at(current_joystick)->number_btn; ++i){
        joystick.at(current_joystick)->button[i] = m_joystick->buttons[i];
    }
}

/******************************************************************************
 ******************************************************************************/
// toggleSampling is offered as a function a parent widget/class can call
void SandBoxWidget::toggleSampling(bool down)
{
    if (down) {
        m_timer->start();
    } else {
        m_timer->stop();
    }
}

void SandBoxWidget::exttoggle(bool on)
{
    if (on) {
        // m_timer->start();
        ui->sample_box->setChecked(true);
    } else{
        // m_timer->stop();
        ui->sample_box->setChecked(false);
    }
}
