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

#include <Core/CircularBuffer>

#include <QtTest/QtTest>

class tst_CircularBuffer : public QObject
{
    Q_OBJECT
private slots:
    void test_empty();
    void test_size();
    void test_capacity();

    void test_push();
    void test_pushWhenExceedingCapacity();

    void test_pop();
    void test_popWhenEmpty();

    void test_popAndPushConcurrently();
};

/*************************************************************************
 *************************************************************************/
void tst_CircularBuffer::test_empty()
{
    // Given, When
    CircularBuffer target;

    // Then
    QCOMPARE(target.size(), 0);
    QCOMPARE(target.capacity(), 32);
}

void tst_CircularBuffer::test_size()
{
    // Given
    CommandBuffer buffer;
    CircularBuffer target;

    // When, Then
    QCOMPARE(target.size(), 0);

    target.push(buffer);
    QCOMPARE(target.size(), 1);

    target.push(buffer);
    QCOMPARE(target.size(), 2);

    target.push(buffer);
    QCOMPARE(target.size(), 3);

    target.pop();
    QCOMPARE(target.size(), 2);

    target.pop();
    QCOMPARE(target.size(), 1);

    target.pop();
    QCOMPARE(target.size(), 0);
}

void tst_CircularBuffer::test_capacity()
{
    CircularBuffer target;

    QCOMPARE(target.capacity(), 32);

    target.setCapacity(16);
    QCOMPARE(target.capacity(), 16);

    target.setCapacity(-1);
    QCOMPARE(target.capacity(), 16);

    target.setCapacity(0);
    QCOMPARE(target.capacity(), 16);

    target.setCapacity(1);
    QCOMPARE(target.capacity(), 1);
}

void tst_CircularBuffer::test_push()
{
    // Given
    CircularBuffer target;
    CommandBuffer input;

    // When
    QCOMPARE(target.size(), 0);
    target.push(input);

    // Then
    QCOMPARE(target.size(), 1);
}

void tst_CircularBuffer::test_pushWhenExceedingCapacity()
{
    // Given
    CircularBuffer target;
    CommandBuffer input0;
    input0.frames[2].actuatorX = CommandStep::Increment; // creates a signature
    input0.frames[0].actuatorY = CommandStep::Decrement;
    input0.frames[1].actuatorZ = CommandStep::Decrement;
    target.push(input0);
    CommandBuffer expected;
    expected.frames[2].actuatorX = CommandStep::Increment; // creates another signature
    expected.frames[0].actuatorY = CommandStep::Decrement;
    expected.frames[1].actuatorZ = CommandStep::Increment;
    expected.frames[3].actuatorX = CommandStep::Increment;
    target.push(expected);

    const int capacity = target.capacity();
    for (int i = 0; i < capacity - 1; ++i) {
        target.push(CommandBuffer());  // Fill the CircularBuffer with (n-1) items
    }

    // When
    CommandBuffer actual = target.pop();

    // Then
    QCOMPARE(actual, expected); // must be equal to input1, not input0 nor empty
}

void tst_CircularBuffer::test_pop()
{
    // Given
    CircularBuffer target;
    CommandBuffer expected;
    expected.frames[0].actuatorX = CommandStep::Increment; // creates a signature
    expected.frames[2].actuatorY = CommandStep::Decrement;
    expected.frames[1].actuatorZ = CommandStep::Decrement;
    expected.frames[3].actuatorX = CommandStep::Increment;
    target.push(expected);

    // When
    CommandBuffer actual = target.pop();

    // Then
    QCOMPARE(actual, expected); // must be equal
}

void tst_CircularBuffer::test_popWhenEmpty()
{
    // Given
    CircularBuffer target;
    CommandBuffer input;
    input.frames[0].actuatorX = CommandStep::Increment; // creates a signature
    input.frames[2].actuatorY = CommandStep::Decrement;
    input.frames[1].actuatorZ = CommandStep::Decrement;
    input.frames[3].actuatorX = CommandStep::Increment;
    target.push(input);
    CommandBuffer expected = input;

    // When
    CommandBuffer actual = target.pop();
    CommandBuffer emptyOne = target.pop();

    // Then
    QCOMPARE(emptyOne, CommandBuffer());
    QCOMPARE(actual, expected);
}

void tst_CircularBuffer::test_popAndPushConcurrently()
{
    // TODO
}

QTEST_APPLESS_MAIN(tst_CircularBuffer)

#include "tst_circularbuffer.moc"
