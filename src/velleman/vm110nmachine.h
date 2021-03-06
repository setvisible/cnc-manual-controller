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

#ifndef VELLEMAN_VM110N_MACHINE_H
#define VELLEMAN_VM110N_MACHINE_H

#include <Core/IMachine>

class DLL_Velleman;

class VM110NMachine : public IMachine
{
public:
    explicit VM110NMachine(QObject *parent = Q_NULLPTR);
    ~VM110NMachine();

    virtual bool openDevice() Q_DECL_OVERRIDE;
    virtual void closeDevice() Q_DECL_OVERRIDE;

    virtual void receive() Q_DECL_OVERRIDE;

    virtual bool isEmergencyActive() Q_DECL_OVERRIDE;
    virtual bool hasSensorActive() Q_DECL_OVERRIDE;

    virtual bool isSensorMXActive() Q_DECL_OVERRIDE;
    virtual bool isSensorMYActive() Q_DECL_OVERRIDE;
    virtual bool isSensorMZActive() Q_DECL_OVERRIDE;
    virtual bool isSensorPXActive() Q_DECL_OVERRIDE;
    virtual bool isSensorPYActive() Q_DECL_OVERRIDE;
    virtual bool isSensorPZActive() Q_DECL_OVERRIDE;

    virtual void setMotorXStep(CommandStep step) Q_DECL_OVERRIDE;
    virtual void setMotorYStep(CommandStep step) Q_DECL_OVERRIDE;
    virtual void setMotorZStep(CommandStep step) Q_DECL_OVERRIDE;

    virtual void send() Q_DECL_OVERRIDE;

private:
    DLL_Velleman *m_dll;
    int m_cardAdress;
    bool m_sensorChannel[5] = { false };
    bool m_actuatorChannel[8] = { false };

    inline void setCardAddress(bool sk5, bool sk6);
    inline bool direction();
};

#endif // VELLEMAN_VM110N_MACHINE_H
