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

// Complete in-memory model of a .trizbort map. Every element and attribute the
// Trizbort file format defines is modelled here as a typed field; nothing is
// dropped or preserved as opaque text. See qt/docs/trizbort-file-format.md and
// the C# originals (Domain/Elements/Room.cs, Connection.cs, Setup/Settings.cs)
// which this mirrors.
namespace trizbort {

// The default region name; rooms and the region list use it for "no region".
inline const QString kNoRegion = QStringLiteral("NoRegion");

// A room's shape, matching the C# RoomShape enum (persisted as an int in
// <settings><rooms><defaultRoomShape>).
enum class RoomShape { SquareCorners, RoundedCorners, Ellipse, Octagonal, NotARoom };

struct Region {
    QString name;
    QColor textColor;   // invalid => default (Blue)
    QColor fillColor;   // invalid => default (White)
    // The exact serialized token each colour was read from (e.g. "#F5F5F5" vs
    // "WhiteSmoke"), so an unedited colour is written back byte-for-byte. Empty
    // => serialize the QColor via ColorTranslator.ToHtml.
    QString textColorToken;
    QString fillColorToken;
};

// Where a room's object list is anchored, persisted as a compass token in the
// <objects at="..."> attribute. South is the default and is omitted on save.
struct ObjectsPosition {
    QString at;                   // compass token ("n","sw",...); empty => default (South)
    bool custom = false;
    int customRight = 0;
    int customDown = 0;
};

struct Room {
    int id = 0;
    int seq = 0;                  // position among all elements, to preserve file order
    QString name;
    QString subtitle;
    QString description;
    QString region = kNoRegion;   // region name; "NoRegion" => none
    int referenceRoom = -1;       // id of the room this references, or -1

    double x = 0.0;               // top-left corner, map units
    double y = 0.0;
    double w = 96.0;
    double h = 64.0;

    // Shape / border.
    bool handDrawn = false;
    bool allCornersEqual = false;
    bool ellipse = false;
    bool roundedCorners = false;
    bool octagonal = false;
    double cornerTopLeft = 0.0;
    double cornerTopRight = 0.0;
    double cornerBottomLeft = 0.0;
    double cornerBottomRight = 0.0;
    QString borderStyle = QStringLiteral("Solid");   // Solid | Dashed | None | Dot | ...

    // Flags.
    bool isDark = false;
    bool isStartRoom = false;
    bool isEndRoom = false;

    // Colours (invalid => inherit the map/settings default).
    QColor fill;
    QColor secondFill;
    QString secondFillLocation = QStringLiteral("Bottom");
    QColor border;
    QColor largeText;             // roomLargeText
    QColor subtitleColor;         // roomSubtitleColor
    QColor smallText;             // roomSmallText

    int zOrder = 0;

    // Objects: the raw list text (indentation denotes containment, [..] encodes
    // properties) with newlines as real '\n'; plus where the list is anchored.
    QString objectsText;
    ObjectsPosition objectsPosition;
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
    int seq = 0;                  // position among all elements, to preserve file order
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

// A font as stored in <settings><fonts>: family plus size and style flags.
struct FontSpec {
    QString family;
    double size = 0.0;
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikeout = false;
};

// Which slot each colour occupies in <settings><colors>, matching Colors.cs.
// The order is the persisted order (Colors.Names).
enum ColorSlot {
    ColorCanvas = 0,
    ColorBorder,
    ColorLine,
    ColorSelectedLine,
    ColorHoverLine,
    ColorSubtitle,
    ColorSmallText,
    ColorLineText,
    ColorGrid,
    ColorStartRoom,
    ColorEndRoom,
    ColorCount
};

// The full <settings> block: per-map when saved, mirroring Setup/Settings.cs.
struct MapSettings {
    QColor colors[ColorCount];    // indexed by ColorSlot; invalid => default
    // Exact serialized token for each palette colour (see Region), so a colour
    // Trizbort stored as hex is not rewritten as its equivalent name and back.
    QString colorTokens[ColorCount];

    FontSpec roomFont;            // <fonts><room>
    FontSpec objectFont;          // <fonts><object>
    FontSpec subtitleFont;        // <fonts><subTitle>
    FontSpec lineFont;            // <fonts><line>

    bool snapToGrid = true;
    bool gridVisible = true;
    bool showOrigin = true;
    double gridSize = 32.0;

    double lineWidth = 2.0;
    double arrowSize = 12.0;
    double textOffset = 4.0;

    double darknessStripeSize = 24.0;
    double objectListOffset = 4.0;
    double connectionStalkLength = 32.0;
    double preferredDistanceBetweenRooms = 64.0;
    QString defaultRoomName = QStringLiteral("Cave");
    int defaultRoomShape = 0;     // RoomShape as int

    double handleSize = 12.0;
    double snapToElementSize = 16.0;

    bool documentSpecificMargins = false;
    double horizontalMargin = 0.0;
    double verticalMargin = 0.0;
    bool wrapTextAtDashes = true;

    QString keypadCreationModifier = QStringLiteral("control");
    QString keypadUnexploredModifier = QStringLiteral("alt");

    // True once a settings block has actually been read from a file, so a
    // freshly-created document can be told apart from a loaded one.
    bool loaded = false;

    MapSettings();
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
    MapSettings settings;

    void clear();
    void reindex();                           // rebuild id -> room lookup
    const Room *roomById(int id) const;       // nullptr if not found
    Room *roomById(int id);
    QColor regionFill(const QString &name) const;   // invalid if unknown
    const Region *regionByName(const QString &name) const;
    int nextRoomId() const;                   // smallest unused positive id
    int nextConnectionId() const;
    int nextSeq() const;                      // element order for a new element

    // --- Editing operations (GUI-independent). Structural changes keep the
    // id index consistent; returned pointers are invalidated by later edits, so
    // re-fetch by id after any further mutation. ---

    // Add a room at (x, y) with the document's default name/size. Returns its id.
    int addRoom(double x, double y);
    // Remove a room and every connection docked to it.
    void removeRoom(int roomId);
    // Add a two-way connection docked between two rooms' ports. Returns its id.
    int addConnection(int roomA, const QString &portA, int roomB, const QString &portB);
    // Remove a connection by id.
    void removeConnection(int connId);
    // Connection index by id, or -1.
    int connectionIndex(int connId) const;

    // The dock port on "from" that best faces "to" (one of n/s/e/w/ne/nw/se/sw).
    static QString portFacing(const Room &from, const Room &to);

    // Convenience accessors for the three colours the renderer reads most; these
    // resolve to the settings palette (ColorCanvas / ColorLine / ColorBorder).
    QColor canvasColor() const;
    QColor lineColor() const;
    QColor borderColor() const;

private:
    QHash<int, int> m_roomIndex;              // room id -> index in rooms
};

} // namespace trizbort

#endif // TRIZBORT_MAPDOCUMENT_H
