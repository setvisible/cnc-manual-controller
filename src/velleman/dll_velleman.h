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

#ifndef VELLEMAN_DLL_VELLEMAN_H
#define VELLEMAN_DLL_VELLEMAN_H

class DLL_Velleman
{
public:
    explicit DLL_Velleman();

    //
    // Assigns the functions of the VELLEMAN library K8055D.DLL
    //
    typedef int  (*t_func001)(int);
    typedef void (*t_func002)(void);
    typedef int  (*t_func003)(void);
    typedef int  (*t_func004)(int);
    typedef int  (*t_func005)(void);
    //
    typedef int  (*t_func006)(int);
    typedef void (*t_func007)(int *, int *);
    //
    typedef void (*t_func008)(int, int);
    typedef void (*t_func009)(int, int);
    typedef void (*t_func010)(int);
    typedef void (*t_func011)(void);
    typedef void (*t_func012)(int);
    typedef void (*t_func013)(void);
    //
    typedef void (*t_func014)(int);
    typedef void (*t_func015)(int);
    typedef void (*t_func016)(void);
    typedef void (*t_func017)(int);
    typedef void (*t_func018)(void);
    //
    typedef bool (*t_func019)(int);
    typedef int  (*t_func020)(void);
    //
    typedef void (*t_func021)(int);
    typedef int  (*t_func022)(int);
    typedef void (*t_func023)(int, int);
    //
    typedef int  (*t_func024)(int);
    typedef void (*t_func025)(int *);


    // General functions
    t_func001 OpenDevice;
    t_func002 CloseDevice;
    t_func003 SearchDevices;
    t_func004 SetCurrentDevice;
    t_func005 DLLVersion;

    // Analog to Digital converter functions
    t_func006 ReadAnalogChannel;
    t_func007 ReadAllAnalog;

    // Digital to Analog conversion functions
    t_func008 OutputAnalogChannel;
    t_func009 OutputAllAnalog;
    t_func010 ClearAnalogChannel;
    t_func011 ClearAllAnalog;
    t_func012 SetAnalogChannel;
    t_func013 SetAllAnalog;

    // Digital Output functions
    t_func014 WriteAllDigital;
    t_func015 ClearDigitalChannel;
    t_func016 ClearAllDigital;
    t_func017 SetDigitalChannel;
    t_func018 SetAllDigital;

    // Digital Input functions
    t_func019 ReadDigitalChannel;
    t_func020 ReadAllDigital;

    // Counter functions
    t_func021 ResetCounter;
    t_func022 ReadCounter;
    t_func023 SetCounterDebounceTime;

    // Readback procedures and functions
    t_func024 ReadBackDigitalOut;
    t_func025 ReadBackAnalogOut;

private:
    int loadLibrary();

};

#endif // VELLEMAN_DLL_VELLEMAN_H
