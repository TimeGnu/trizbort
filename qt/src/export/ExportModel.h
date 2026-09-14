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

#ifndef TRIZBORT_EXPORT_MODEL_H
#define TRIZBORT_EXPORT_MODEL_H

#include <optional>

#include <QHash>
#include <QList>
#include <QString>

#include "../MapDocument.h"

// The intermediate export model, ported from Export/Domain/{Exit,Location,Thing,
// ExportRegion}.cs and the direction/compass helpers in Domain/Misc/CompassPoint.cs.
namespace trizbort {

enum class MappableDirection {
    North, South, East, West, NorthEast, SouthEast, SouthWest, NorthWest,
    Up, Down, In, Out, None
};

// 16-point compass, same order as the C# CompassPoint enum.
enum class CompassPoint {
    North, NorthNorthEast, NorthEast, EastNorthEast, East, EastSouthEast,
    SouthEast, SouthSouthEast, South, SouthSouthWest, SouthWest, WestSouthWest,
    West, WestNorthWest, NorthWest, NorthNorthWest
};

// Directions in MappableDirection enum order (== C# Directions.AllDirections).
const QList<MappableDirection> &allDirections();

// Dock port string ("n","ne","nnw",...) -> CompassPoint. false if unrecognized.
bool compassPointFromPort(const QString &port, CompassPoint &out);

MappableDirection oppositeOf(MappableDirection direction);

class Location;

// One direction of a connection (a two-way connection yields two Exits).
class Exit {
public:
    Exit(Location *source, Location *target, CompassPoint visualCompassPoint,
         const QString &connectionText, const Connection &connection);

    Location *source = nullptr;
    Location *target = nullptr;
    CompassPoint visualCompassPoint = CompassPoint::North;
    bool conditional = false; // dashed connection
    bool exported = false;
    MappableDirection primaryDirection = MappableDirection::North;
    std::optional<MappableDirection> secondaryDirection;

    int priority(MappableDirection direction) const;
    static bool isReciprocated(Location *source, MappableDirection direction, Location *target);

private:
    int m_primaryPriority = 0;
    void assignPrimaryPriority();
    void assignSecondaryDirection(const QString &connectionText);
    void assignPrimaryDirection();
};

// One object parsed from a room's Objects text.
class Thing {
public:
    QString displayName;
    QString exportName;
    Location *location = nullptr;
    Thing *container = nullptr;
    int indent = 0;
    QString propString;
    QList<Thing *> contents;
};

class ExportRegion {
public:
    Region region;
    QString exportName;
};

class Location {
public:
    Location(const Room *room, const QString &exportName)
        : room(room), exportName(exportName) {}

    const Room *room = nullptr;
    QString exportName;
    QList<Thing *> things;

    void addExit(Exit *exit) { m_exits.append(exit); }
    Exit *bestExit(MappableDirection direction) const;
    void pickBestExits();

private:
    Exit *pickBestExit(MappableDirection direction);

    QList<Exit *> m_exits;
    QHash<int, Exit *> m_bestByDirection; // int(MappableDirection) -> Exit
};

} // namespace trizbort

#endif // TRIZBORT_EXPORT_MODEL_H
