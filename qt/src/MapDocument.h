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

#ifndef TRIZBORT_MAPDOCUMENT_H
#define TRIZBORT_MAPDOCUMENT_H

#include <QColor>
#include <QHash>
#include <QList>
#include <QPointF>
#include <QString>
#include <QStringList>

// In-memory model of a .trizbort map. See qt/docs/trizbort-file-format.md.
namespace trizbort {

struct Region {
    QString name;
    QColor textColor;   // invalid => default
    QColor fillColor;   // invalid => default
};

struct Room {
    int id = 0;
    QString name;
    QString subtitle;
    QString description;
    QString region;               // region name; "NoRegion" or empty => none
    double x = 0.0;               // top-left corner, map units
    double y = 0.0;
    double w = 96.0;
    double h = 64.0;
    QString borderStyle = QStringLiteral("Solid");   // Solid | Dashed | None
    QColor fill;                  // invalid => inherit map default
    QColor secondFill;
    QColor border;
    QColor largeText;
    QColor smallText;
    bool isDark = false;
    bool isStartRoom = false;
    QString objectsText;          // raw <objects> text (indentation + [props] preserved)
};

enum class ConnectionStyle { Solid, Dashed };
enum class ConnectionFlow { TwoWay, OneWay };

struct Door {
    bool lockable = false;
    bool locked = false;
    bool open = false;
    bool openable = false;
};

struct Vertex {
    int index = 0;
    bool docked = false;          // true => (roomId, port); false => point
    int roomId = 0;
    QString port;                 // "n","s","e","w","ne","nw","se","sw",...
    QPointF point;                // free vertex, map units
};

struct Connection {
    int id = 0;
    ConnectionStyle style = ConnectionStyle::Solid;
    ConnectionFlow flow = ConnectionFlow::TwoWay;
    QString name;
    QString description;
    QString startText;
    QString midText;
    QString endText;
    QColor color;                 // invalid => inherit map line color
    bool hasDoor = false;
    Door door;
    QList<Vertex> vertices;       // ordered; >= 2 endpoints when valid
};

class Map {
public:
    QString version;
    QString title;
    QString author;
    QString description;
    QString history;

    QList<Room> rooms;
    QList<Connection> connections;
    QList<Region> regions;

    // A small subset of <settings>/<colors> the renderer uses.
    QColor canvasColor;
    QColor lineColor;
    QColor borderColor;

    void clear();
    void reindex();                           // rebuild id -> room lookup
    const Room *roomById(int id) const;       // nullptr if not found
    QColor regionFill(const QString &name) const;   // invalid if unknown

private:
    QHash<int, int> m_roomIndex;              // room id -> index in rooms
};

} // namespace trizbort

#endif // TRIZBORT_MAPDOCUMENT_H
