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

#ifndef CORE_COMMAND_STEP_H
#define CORE_COMMAND_STEP_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE
class QDebug;
QT_END_NAMESPACE

enum class CommandStep {
    None, Increment, Decrement
};

/* #ifdef QT_TESTLIB_LIB */
char *toString(const CommandStep &step);
/* #endif */

Q_DECLARE_METATYPE(CommandStep)

#ifdef QT_DEBUG
QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const CommandStep &step);
QT_END_NAMESPACE
#endif

#endif // CORE_COMMAND_STEP_H
