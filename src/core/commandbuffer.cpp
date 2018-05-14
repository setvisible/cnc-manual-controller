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

#include "commandbuffer.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif
#ifdef QT_TESTLIB_LIB
#  include <QtTest/QTest>
#endif

/******************************************************************************
 ******************************************************************************/
bool CommandBuffer::operator==(const CommandBuffer &other) const
{
    for (int i = 0; i < 4; ++i) {
        if ((*this).frames[i] != other.frames[i]) {
            return false;
        }
    }
    return true;
}

bool CommandBuffer::operator!=(const CommandBuffer &other) const
{
    return ((*this) == other) ? false : true;
}

/******************************************************************************
 ******************************************************************************/
#ifdef QT_TESTLIB_LIB
/// This function is used by QCOMPARE() to output verbose information in case of a test failure.
char *toString(const CommandBuffer &buffer)
{
    QString content;
    for (int i = 0; i < 4; ++i) {
        content += toString(buffer.frames[i]);
    }
    QString str = QString("CommandBuffer[4] {%0}").arg(content);

    // bring QTest::toString overloads into scope:
    using QTest::toString;

    // delegate char* handling to QTest::toString(QByteArray):
    return toString(str);
}
#endif

#ifdef QT_DEBUG
/// Custom Types to a Stream
QDebug operator<<(QDebug dbg, const CommandBuffer &buffer)
{
    dbg.noquote() << QLatin1String("CommandBuffer[4] {");
    for (int i = 0; i < 4; ++i) {        
        dbg.noquote() << "(";
        dbg.noquote() << toString(buffer.frames[i]);
        dbg.noquote() << ")";
    }
    dbg.noquote() << QLatin1String(")");
    return dbg.maybeSpace();
}
#endif
