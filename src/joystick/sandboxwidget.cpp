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
  , m_joystickDevice(new QJoystick(this))
  , m_timer(new QTimer())
{
    ui->setupUi(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateData()));
    connect(ui->deviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSelection(int)));
    connect(ui->samplingGroupBox, SIGNAL(toggled(bool)), this, SLOT(setSamplingActive(bool)));

    m_timer->setInterval(50);

    reset();
    setSamplingActive(false);
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
/*!
 * \brief Reset the joysticks information
 *
 * The method gets the number of joysticks available
 * and populate axes and buttons.
 */
void SandBoxWidget::reset()
{
    m_progressBars.clear();
    m_labels.clear();
    m_checkboxes.clear();

    ui->deviceComboBox->clear();

    /* Find number of joysticks present */
    m_joystickCount = m_joystickDevice->availableJoystickCount();

    /* Create joysticks list */
    for (int i = 0; i < m_joystickCount; ++i){
        JoystickData *joystick = new JoystickData;
        m_joysticks.append(joystick );
    }

    /* Populate labels depending on the number of joysticks found */
    if (m_joystickCount == 0) {
        ui->availableDevicesLabel->setText(QString("No joysticks found"));
        ui->deviceComboBox->setDisabled(true);

    } else {
        QString text = QString("%0 joystick%1 found:")
                .arg(m_joystickCount)
                .arg(m_joystickCount == 1 ? QLatin1String("") : QLatin1String("s"));
        ui->availableDevicesLabel->setText(text);

        /* Populate data structure for all joysticks */
        for (int i = 0; i < m_joystickCount; ++i) {
            m_joystickDevice->setJoystickIndex(i);

            /* Populate ComboBox */
            ui->deviceComboBox->addItem(m_joystickDevice->joystickName(i));

            /* Axes */
            m_joysticks.at(i)->number_axes = m_joystickDevice->joystickNumAxes(i);
            for (int j = 0; j < m_joysticks.at(i)->number_axes; ++j) {
                m_joysticks.at(i)->axis.append(0);
            }

            /* Buttons */
            m_joysticks.at(i)->number_btn  = m_joystickDevice->joystickNumButtons(i);
            for (int j = 0; j < m_joysticks.at(i)->number_btn; ++j) {
                m_joysticks.at(i)->button.append(false);
            }
        }

        m_currentJoystickIndex = 0;
        ui->infoLabel->setText(QString("%1 axes - %2 buttons")
                               .arg(m_joysticks.at(0)->number_axes)
                               .arg(m_joysticks.at(0)->number_btn));

        m_joystickDevice->setJoystickIndex(ui->deviceComboBox->currentIndex());
    }

    /*
     * If there's a joystick present, initialize axes and buttons displays
     * and start the update timer.
     */
    if (m_joystickCount > 0) {
        updateSelection(0);
    }
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Lay out progress bars for axes and buttons.
 */
void SandBoxWidget::updateSelection(int index)
{
    QLayoutItem *child = Q_NULLPTR;
    while ((child = ui->axesLayout->takeAt(0)) != Q_NULLPTR) {
        delete child->widget();
        delete child;
    }

    for (int j = 0; j < m_progressBars.size(); ++j){
        m_progressBars.at(j)->deleteLater();
        m_labels.at(j)->deleteLater();

    }
    m_progressBars.clear();
    m_labels.clear();

    while (!ui->buttonsLayout->isEmpty()) {
        ui->buttonsLayout->removeWidget(ui->buttonsLayout->takeAt(0)->widget());
    }

    for (int j = 0; j < m_checkboxes.size(); ++j) {
        m_checkboxes.at(j)->deleteLater();
    }
    m_checkboxes.clear();

    m_currentJoystickIndex = index;

    ui->infoLabel->setText(QString("%1 axes - %2 buttons")
                           .arg(m_joysticks.at(index)->number_axes)
                           .arg(m_joysticks.at(index)->number_btn));
    m_joystickDevice->setJoystickIndex(ui->deviceComboBox->currentIndex());

    for (int i = 0; i < m_joysticks.at(index)->number_axes; ++i){

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

        ui->axesLayout->addLayout(layout);
    }

    for (int j = 0; j < m_joysticks.at(m_currentJoystickIndex)->number_btn; ++j) {
        QCheckBox *checkbox = new QCheckBox(this);
        checkbox->setChecked(false);
        m_checkboxes.append(checkbox);
        ui->buttonsLayout->addWidget(m_checkboxes[j]);
    }
}

void SandBoxWidget::updateData()
{
    m_joystickDevice->storeCurrentState();
    pollJoystick();
    for (int i = 0; i < m_joysticks.at(m_currentJoystickIndex)->number_axes; ++i) {
        m_progressBars[i]->setValue(m_joysticks.at(m_currentJoystickIndex)->axis[i]);
        m_labels[i]->setText(QString("%1").arg(m_joysticks.at(m_currentJoystickIndex)->axis[i]));
    }

    for (int i = 0; i < m_joysticks.at(m_currentJoystickIndex)->number_btn; ++i) {
        m_checkboxes[i]->setChecked(m_joysticks.at(m_currentJoystickIndex)->button[i]);
    }
    //this->update();
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Extracts data from QJoystick class
 */
void SandBoxWidget::pollJoystick()
{
    m_joystickDevice->storeCurrentState();
    for (int i = 0; i < m_joysticks.at(m_currentJoystickIndex)->number_axes; ++i){
        m_joysticks.at(m_currentJoystickIndex)->axis[i] = m_joystickDevice->axis().at(i);
    }

    for (int i = 0; i < m_joysticks.at(m_currentJoystickIndex)->number_btn; ++i){
        m_joysticks.at(m_currentJoystickIndex)->button[i] = m_joystickDevice->buttons().at(i);
    }
}

/******************************************************************************
 ******************************************************************************/
/*! \brief Set the Sampling active if \a active is \a true.
 */
void SandBoxWidget::setSamplingActive(bool active)
{
    if (active) {
        m_timer->start();
    } else {
        m_timer->stop();
    }
    ui->infoLabel->setEnabled(active);
    ui->axesLayout->setEnabled(active);
    ui->buttonsLayout->setEnabled(active);
}

