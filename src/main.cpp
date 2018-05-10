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

#include "globals.h"
#include "mainwindow.h"

#include <QtCore/QLibrary>
#include <QtWidgets/QApplication>

bool checkLibrary()
{
    /* Load the library in advance to verify it exists. */
    QLibrary library("k8055d.dll");
    if (!library.load()) {
        printf("The program can't start because k8055d.dll is missing from your computer.\n");
        return false; // Library not found.
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (!checkLibrary()) {
        return -1;


    QApplication app(argc, argv);

    QCoreApplication::setApplicationName(STR_APPLICATION_NAME);
    QCoreApplication::setOrganizationName(STR_APPLICATION_ORGANIZATION);
    QCoreApplication::setApplicationVersion(STR_APPLICATION_VERSION);

    MainWindow w;
    w.show();
    return app.exec();
}

