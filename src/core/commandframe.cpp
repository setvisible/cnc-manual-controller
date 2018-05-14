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

#include "commandframe.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif

/******************************************************************************
 ******************************************************************************/
bool CommandFrame::operator==(const CommandFrame &other) const
{
    return  (*this).actuatorX == other.actuatorX &&
            (*this).actuatorY == other.actuatorY &&
            (*this).actuatorZ == other.actuatorZ;
}

bool CommandFrame::operator!=(const CommandFrame &other) const
{
    return ((*this) == other) ? false : true;
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const CommandFrame &frame)
{
    QString str = QString("(%0 %1 %2)")
            .arg(toString(frame.actuatorX))
            .arg(toString(frame.actuatorY))
            .arg(toString(frame.actuatorZ));

    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString(str);
}
#else
QString toString(const CommandFrame &frame)
{
    QString str = QString("(%0 %1 %2)")
            .arg(toString(frame.actuatorX))
            .arg(toString(frame.actuatorY))
            .arg(toString(frame.actuatorZ));
    return str;
}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const CommandFrame &frame)
{
    dbg.noquote() << QLatin1String("CommandFrame(");
    dbg.quote() << frame.actuatorX << frame.actuatorY << frame.actuatorZ;
    dbg.noquote() << QLatin1String(")");
    return dbg.maybeQuote();
}
#endif
