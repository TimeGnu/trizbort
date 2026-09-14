/*
 * Copyright (c) 2026  Jason Self <j@jxself.org>
 *
 *  This file is free software: you may copy, redistribute and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This file is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see https://www.gnu.org/licenses/.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright (c) 2010-2022 by Genstein and Jason Lautzenheiser
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */

#include "ExportModel.h"

#include <algorithm>

namespace trizbort {

const QList<MappableDirection> &allDirections()
{
    static const QList<MappableDirection> dirs = {
        MappableDirection::North, MappableDirection::South, MappableDirection::East,
        MappableDirection::West, MappableDirection::NorthEast, MappableDirection::SouthEast,
        MappableDirection::SouthWest, MappableDirection::NorthWest, MappableDirection::Up,
        MappableDirection::Down, MappableDirection::In, MappableDirection::Out,
        MappableDirection::None};
    return dirs;
}

bool compassPointFromPort(const QString &port, CompassPoint &out)
{
    // Index order matches CompassPointHelper.Names and the CompassPoint enum.
    static const char *const names[16] = {
        "n", "nne", "ne", "ene", "e", "ese", "se", "sse",
        "s", "ssw", "sw", "wsw", "w", "wnw", "nw", "nnw"};
    const QString p = port.toLower();
    for (int i = 0; i < 16; ++i) {
        if (p == QLatin1String(names[i])) {
            out = static_cast<CompassPoint>(i);
            return true;
        }
    }
    return false;
}

MappableDirection oppositeOf(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return MappableDirection::South;
    case MappableDirection::South:     return MappableDirection::North;
    case MappableDirection::East:      return MappableDirection::West;
    case MappableDirection::West:      return MappableDirection::East;
    case MappableDirection::NorthEast: return MappableDirection::SouthWest;
    case MappableDirection::NorthWest: return MappableDirection::SouthEast;
    case MappableDirection::SouthEast: return MappableDirection::NorthWest;
    case MappableDirection::SouthWest: return MappableDirection::NorthEast;
    case MappableDirection::Up:        return MappableDirection::Down;
    case MappableDirection::Down:      return MappableDirection::Up;
    case MappableDirection::In:        return MappableDirection::Out;
    case MappableDirection::Out:       return MappableDirection::In;
    default:                           return MappableDirection::North;
    }
}

// --- Exit ---

Exit::Exit(Location *source, Location *target, CompassPoint visualCompassPoint,
           const QString &connectionText, const Connection &connection)
    : source(source), target(target), visualCompassPoint(visualCompassPoint)
{
    conditional = (connection.style == ConnectionStyle::Dashed);

    // Order matters and is preserved from the C#: priority is computed while
    // secondaryDirection is still unset, which makes the negative branches dead.
    assignPrimaryPriority();
    assignSecondaryDirection(connectionText);
    if (secondaryDirection.has_value())
        primaryDirection = secondaryDirection.value();
    else
        assignPrimaryDirection();
}

void Exit::assignPrimaryPriority()
{
    m_primaryPriority = 0;
    switch (visualCompassPoint) {
    case CompassPoint::North:
    case CompassPoint::South:
    case CompassPoint::East:
    case CompassPoint::West:
    case CompassPoint::NorthEast:
    case CompassPoint::SouthEast:
    case CompassPoint::SouthWest:
    case CompassPoint::NorthWest:
        if (!secondaryDirection.has_value())
            m_primaryPriority += 4;
        else
            m_primaryPriority -= 2;
        break;
    default:
        if (!secondaryDirection.has_value())
            m_primaryPriority += 3;
        else
            m_primaryPriority -= 1;
        break;
    }
}

void Exit::assignSecondaryDirection(const QString &connectionText)
{
    if (connectionText == QLatin1String("up"))
        secondaryDirection = MappableDirection::Up;
    else if (connectionText == QLatin1String("down"))
        secondaryDirection = MappableDirection::Down;
    else if (connectionText == QLatin1String("in"))
        secondaryDirection = MappableDirection::In;
    else if (connectionText == QLatin1String("out"))
        secondaryDirection = MappableDirection::Out;
    else
        secondaryDirection.reset();
}

void Exit::assignPrimaryDirection()
{
    switch (visualCompassPoint) {
    case CompassPoint::NorthNorthWest:
    case CompassPoint::North:
    case CompassPoint::NorthNorthEast:
        primaryDirection = MappableDirection::North;
        break;
    case CompassPoint::NorthEast:
        primaryDirection = MappableDirection::NorthEast;
        break;
    case CompassPoint::EastNorthEast:
    case CompassPoint::East:
    case CompassPoint::EastSouthEast:
        primaryDirection = MappableDirection::East;
        break;
    case CompassPoint::SouthEast:
        primaryDirection = MappableDirection::SouthEast;
        break;
    case CompassPoint::SouthSouthEast:
    case CompassPoint::South:
    case CompassPoint::SouthSouthWest:
        primaryDirection = MappableDirection::South;
        break;
    case CompassPoint::SouthWest:
        primaryDirection = MappableDirection::SouthWest;
        break;
    case CompassPoint::WestSouthWest:
    case CompassPoint::West:
    case CompassPoint::WestNorthWest:
        primaryDirection = MappableDirection::West;
        break;
    case CompassPoint::NorthWest:
        primaryDirection = MappableDirection::NorthWest;
        break;
    }
}

int Exit::priority(MappableDirection direction) const
{
    if (direction == primaryDirection)
        return m_primaryPriority;
    if (secondaryDirection.has_value() && direction == secondaryDirection.value())
        return 1;
    return -1;
}

bool Exit::isReciprocated(Location *source, MappableDirection direction, Location *target)
{
    if (target != nullptr) {
        const MappableDirection opposite = oppositeOf(direction);
        Exit *reciprocal = target->bestExit(opposite);
        if (reciprocal != nullptr)
            return reciprocal->target == source;
    }
    return false;
}

// --- Location ---

namespace {

// Reproduce .NET's List<T>.Sort (introspective sort) exactly, because the
// exporters depend on how it orders equal-priority exits. In particular it
// uses fixed swap networks for 2- and 3-element partitions, which reorder ties
// differently from a stable sort. Comparison mirrors the C#: b - a (descending
// by priority for the given direction).
int cmpExit(Exit *a, Exit *b, MappableDirection dir)
{
    return b->priority(dir) - a->priority(dir);
}

void swapItems(QList<Exit *> &a, int i, int j)
{
    Exit *t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void swapIfGreater(QList<Exit *> &a, MappableDirection dir, int i, int j)
{
    if (i != j && cmpExit(a[i], a[j], dir) > 0)
        swapItems(a, i, j);
}

void insertionSort(QList<Exit *> &a, int lo, int hi, MappableDirection dir)
{
    for (int i = lo; i < hi; ++i) {
        Exit *t = a[i + 1];
        int j = i;
        while (j >= lo && cmpExit(t, a[j], dir) < 0) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = t;
    }
}

void downHeap(QList<Exit *> &a, int i, int n, int lo, MappableDirection dir)
{
    Exit *d = a[lo + i - 1];
    while (i <= n / 2) {
        int child = 2 * i;
        if (child < n && cmpExit(a[lo + child - 1], a[lo + child], dir) < 0)
            ++child;
        if (!(cmpExit(d, a[lo + child - 1], dir) < 0))
            break;
        a[lo + i - 1] = a[lo + child - 1];
        i = child;
    }
    a[lo + i - 1] = d;
}

void heapSort(QList<Exit *> &a, int lo, int hi, MappableDirection dir)
{
    const int n = hi - lo + 1;
    for (int i = n / 2; i >= 1; --i)
        downHeap(a, i, n, lo, dir);
    for (int i = n; i > 1; --i) {
        swapItems(a, lo, lo + i - 1);
        downHeap(a, 1, i - 1, lo, dir);
    }
}

int pickPivotAndPartition(QList<Exit *> &a, int lo, int hi, MappableDirection dir)
{
    const int mid = lo + (hi - lo) / 2;
    swapIfGreater(a, dir, lo, mid);
    swapIfGreater(a, dir, lo, hi);
    swapIfGreater(a, dir, mid, hi);
    Exit *pivot = a[mid];
    swapItems(a, mid, hi - 1);
    int left = lo, right = hi - 1;
    while (left < right) {
        while (cmpExit(a[++left], pivot, dir) < 0) { }
        while (cmpExit(pivot, a[--right], dir) < 0) { }
        if (left >= right)
            break;
        swapItems(a, left, right);
    }
    if (left != hi - 1)
        swapItems(a, left, hi - 1);
    return left;
}

void introSort(QList<Exit *> &a, int lo, int hi, int depthLimit, MappableDirection dir)
{
    while (hi > lo) {
        const int partitionSize = hi - lo + 1;
        if (partitionSize <= 16) {
            if (partitionSize == 1)
                return;
            if (partitionSize == 2) {
                swapIfGreater(a, dir, lo, hi);
                return;
            }
            if (partitionSize == 3) {
                swapIfGreater(a, dir, lo, hi - 1);
                swapIfGreater(a, dir, lo, hi);
                swapIfGreater(a, dir, hi - 1, hi);
                return;
            }
            insertionSort(a, lo, hi, dir);
            return;
        }
        if (depthLimit == 0) {
            heapSort(a, lo, hi, dir);
            return;
        }
        --depthLimit;
        const int p = pickPivotAndPartition(a, lo, hi, dir);
        introSort(a, p + 1, hi, depthLimit, dir);
        hi = p - 1;
    }
}

int floorLog2(int n)
{
    int r = 0;
    while ((n >>= 1) != 0)
        ++r;
    return r;
}

void dotnetListSort(QList<Exit *> &a, MappableDirection dir)
{
    if (a.size() > 1)
        introSort(a, 0, a.size() - 1, 2 * (floorLog2(a.size()) + 1), dir);
}

} // namespace

Exit *Location::bestExit(MappableDirection direction) const
{
    return m_bestByDirection.value(static_cast<int>(direction), nullptr);
}

void Location::pickBestExits()
{
    m_bestByDirection.clear();
    for (MappableDirection direction : allDirections()) {
        Exit *exit = pickBestExit(direction);
        if (exit != nullptr)
            m_bestByDirection.insert(static_cast<int>(direction), exit);
    }
}

Exit *Location::pickBestExit(MappableDirection direction)
{
    // Sort in place exactly as the C# does (mExits.Sort with a b-a comparer),
    // reproducing .NET's introspective sort so equal-priority ties break the
    // same way byte-for-byte.
    dotnetListSort(m_exits, direction);

    if (!m_exits.isEmpty()) {
        Exit *exit = m_exits.first();
        if (exit->primaryDirection == direction
            || (exit->secondaryDirection.has_value()
                && exit->secondaryDirection.value() == direction))
            return exit;
    }
    return nullptr;
}

} // namespace trizbort
