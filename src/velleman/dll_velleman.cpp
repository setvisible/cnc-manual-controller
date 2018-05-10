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

#include "dll_velleman.h"

#include <QtCore/QLibrary>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

/*!
 * \class DLL_Velleman
 * \brief The class DLL_Velleman provides methods to use the Velleman library.
 */

/*!
 * \brief Constructor
 */
DLL_Velleman::DLL_Velleman()
{
    loadLibrary();
}

/******************************************************************************
 ******************************************************************************/
/*!
 * \brief Load and resolve the function of the dll.
 *
 * \returns  0 if all functions loaded and resolved
 * \returns -1 if DLL file not found and loaded
 * \returns -2 if one function not resolved
 */
int DLL_Velleman::loadLibrary()
{
    QLibrary library("k8055d.dll");

    if (!library.load()) {
        printf("The program can't start because k8055d.dll is missing from your computer.\n");
        qDebug() << library.errorString();
        return -1;  // error load DLL
    }

    qDebug() << "library loaded";

    // General functions
    OpenDevice              = (t_func001) library.resolve("OpenDevice");
    CloseDevice             = (t_func002) library.resolve("CloseDevice");
    SearchDevices           = (t_func003) library.resolve("SearchDevices");
    SetCurrentDevice        = (t_func004) library.resolve("SetCurrentDevice");
    DLLVersion              = (t_func005) library.resolve("Version");

    // Analog to Digital converter functions
    ReadAnalogChannel       = (t_func006) library.resolve("ReadAnalogChannel");
    ReadAllAnalog           = (t_func007) library.resolve("ReadAllAnalog");

    // Digital to Analog conversion functions
    OutputAnalogChannel     = (t_func008) library.resolve("OutputAnalogChannel");
    OutputAllAnalog         = (t_func009) library.resolve("OutputAllAnalog");
    ClearAnalogChannel      = (t_func010) library.resolve("ClearAnalogChannel");
    ClearAllAnalog          = (t_func011) library.resolve("ClearAllAnalog");
    SetAnalogChannel        = (t_func012) library.resolve("SetAnalogChannel");
    SetAllAnalog            = (t_func013) library.resolve("SetAllAnalog");

    // Digital Output functions
    WriteAllDigital         = (t_func014) library.resolve("WriteAllDigital");
    ClearDigitalChannel     = (t_func015) library.resolve("ClearDigitalChannel");
    ClearAllDigital         = (t_func016) library.resolve("ClearAllDigital");
    SetDigitalChannel       = (t_func017) library.resolve("SetDigitalChannel");
    SetAllDigital           = (t_func018) library.resolve("SetAllDigital");

    // Digital Input functions
    ReadDigitalChannel      = (t_func019) library.resolve("ReadDigitalChannel");
    ReadAllDigital          = (t_func020) library.resolve("ReadAllDigital");

    // Counter functions
    ResetCounter            = (t_func021) library.resolve("ResetCounter");
    ReadCounter             = (t_func022) library.resolve("ReadCounter");
    SetCounterDebounceTime  = (t_func023) library.resolve("SetCounterDebounceTime");

    // Readback procedures and functions
    ReadBackDigitalOut      = (t_func024) library.resolve("ReadBackDigitalOut");
    ReadBackAnalogOut       = (t_func025) library.resolve("ReadBackAnalogOut");

    // -----------------

    // General functions
    if (!OpenDevice)             { return -2; }
    if (!CloseDevice)            { return -2; }
    if (!SearchDevices)          { return -2; }
    if (!SetCurrentDevice)       { return -2; }
    if (!DLLVersion)             { return -2; }

    // Analog to Digital converter functions
    if (!ReadAnalogChannel)      { return -2; }
    if (!ReadAllAnalog)          { return -2; }

    // Digital Output functions
    if (!OutputAnalogChannel)    { return -2; }
    if (!OutputAllAnalog)        { return -2; }
    if (!ClearAnalogChannel)     { return -2; }
    if (!ClearAllAnalog)         { return -2; }
    if (!SetAnalogChannel)       { return -2; }
    if (!SetAllAnalog)           { return -2; }

    // Digital Output functions
    if (!WriteAllDigital)        { return -2; }
    if (!ClearDigitalChannel)    { return -2; }
    if (!ClearAllDigital)        { return -2; }
    if (!SetDigitalChannel)      { return -2; }
    if (!SetAllDigital)          { return -2; }

    // Digital Input functions
    if (!ReadDigitalChannel)     { return -2; }
    if (!ReadAllDigital)         { return -2; }

    // Counter functions
    if (!ResetCounter)           { return -2; }
    if (!ReadCounter)            { return -2; }
    if (!SetCounterDebounceTime) { return -2; }

    // Readback procedures and functions
    if (!ReadBackDigitalOut)     { return -2; }
    if (!ReadBackAnalogOut)      { return -2; }


    return 0; // ok
}


