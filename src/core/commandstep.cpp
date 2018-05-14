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

#include "commandstep.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif


/******************************************************************************
 ******************************************************************************/
/* #ifdef QT_TESTLIB_LIB
 * -= Remark =-
 * We commented out this QT_TESTLIB_LIB predirective because the following method
 * toString() doesn't call any of the QtTestLib methods.
 */
/// This function is used by QCOMPARE() to output verbose information in case of a test failure. */
char *toString(const CommandStep &step)
{
    switch (step) {
    case CommandStep::None:      return const_cast<char*>(QLatin1String("..").data()); break;
    case CommandStep::Increment: return const_cast<char*>(QLatin1String("++").data()); break;
    case CommandStep::Decrement: return const_cast<char*>(QLatin1String("--").data()); break;
    default:
        Q_UNREACHABLE();
        break;
    };
    return const_cast<char*>(QLatin1String("?").data());
}
/* #endif */


#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const CommandStep &step)
{
    switch (step) {
    case CommandStep::None:      dbg << QLatin1String("CommandStep::None"); break;
    case CommandStep::Increment: dbg << QLatin1String("CommandStep::Increment"); break;
    case CommandStep::Decrement: dbg << QLatin1String("CommandStep::Decrement"); break;
    default:
        Q_UNREACHABLE();
        break;
    }
    return dbg.maybeSpace();
}
#endif
