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

#include "circularbuffer.h"

#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif

/*! \class CircularBuffer
 * \brief The class CircularBuffer provides a fixed capacity storage.
 *
 * When its capacity is exhausted, newly inserted elements will cause
 * elements to be overwritten, either at the beginning or end of the
 * buffer (depending on what insert operation is used).
 */

/*!
 * \brief Constructor
 */
CircularBuffer::CircularBuffer()
{
    this->setCapacity(32);
}

CircularBuffer::~CircularBuffer()
{
}

/******************************************************************************
 ******************************************************************************/
int CircularBuffer::capacity() const
{
    return m_capacity;
}

void CircularBuffer::setCapacity(int capacity)
{
    if (capacity > 0) {
        m_capacity = capacity;
        m_queue.clear();
        m_queue.reserve(capacity);
    }
}

int CircularBuffer::size() const
{
    return m_queue.size();
}

bool CircularBuffer::isEmpty() const
{
    return m_queue.isEmpty();
}

/******************************************************************************
 ******************************************************************************/
void CircularBuffer::push(CommandBuffer buffer)
{
    if (size() >= m_capacity) {
        m_queue.dequeue();
    }
    m_queue.enqueue(buffer);
}

CommandBuffer CircularBuffer::pop()
{
    if (size() > 0) {
        return m_queue.dequeue();
    }
    return CommandBuffer();
}

