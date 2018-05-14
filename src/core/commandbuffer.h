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

#ifndef CORE_COMMAND_BUFFER_H
#define CORE_COMMAND_BUFFER_H

#include <Core/CommandFrame>

#include <QtCore/QMetaType>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE
class QDebug;
QT_END_NAMESPACE

class CommandBuffer
{
public:
    bool operator==(const CommandBuffer &other) const;
    bool operator!=(const CommandBuffer &other) const;

    CommandFrame frames[4]; // 4 * 2 ticks * 10 msec = 80 msec per action    
};

#ifdef QT_TESTLIB_LIB
char *toString(const CommandBuffer &buffer);
#endif

Q_DECLARE_METATYPE(CommandBuffer)

#ifdef QT_DEBUG
QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug dbg, const CommandBuffer &buffer);
QT_END_NAMESPACE
#endif



#endif // CORE_COMMAND_BUFFER_H
