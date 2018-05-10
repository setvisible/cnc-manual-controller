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

private:
    DLL_Velleman *m_dll;

};

#endif // VELLEMAN_VM110N_MACHINE_H
