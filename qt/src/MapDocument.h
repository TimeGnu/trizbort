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
    QStringList objects;
};

enum class ConnectionStyle { Solid, Dashed };
enum class ConnectionFlow { TwoWay, OneWay };

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
