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

#include "MapDocument.h"

namespace trizbort {

// Default palette, fonts and geometry, matching Settings.Reset() in the C#.
MapSettings::MapSettings()
{
    colors[ColorCanvas] = QColor(255, 255, 255);      // White
    colors[ColorBorder] = QColor(25, 25, 112);        // MidnightBlue
    colors[ColorLine] = QColor(25, 25, 112);          // MidnightBlue
    colors[ColorSelectedLine] = QColor(255, 215, 0);  // Gold
    colors[ColorHoverLine] = QColor(255, 140, 0);     // DarkOrange
    colors[ColorSubtitle] = QColor(25, 25, 112);      // MidnightBlue
    colors[ColorSmallText] = QColor(25, 25, 112);     // MidnightBlue
    colors[ColorLineText] = QColor(25, 25, 112);      // MidnightBlue
    colors[ColorGrid] = QColor(232, 232, 232);        // Mix(White, Black, 10, 1)
    colors[ColorStartRoom] = QColor(173, 255, 47);    // GreenYellow
    colors[ColorEndRoom] = QColor(255, 0, 0);         // Red

    // Default serialized tokens match what .NET's ColorTranslator.ToHtml emits
    // for the Reset() palette: the KnownColor names, and hex for the computed
    // grid colour (Mix, which is not a known colour).
    colorTokens[ColorCanvas] = QStringLiteral("White");
    colorTokens[ColorBorder] = QStringLiteral("MidnightBlue");
    colorTokens[ColorLine] = QStringLiteral("MidnightBlue");
    colorTokens[ColorSelectedLine] = QStringLiteral("Gold");
    colorTokens[ColorHoverLine] = QStringLiteral("DarkOrange");
    colorTokens[ColorSubtitle] = QStringLiteral("MidnightBlue");
    colorTokens[ColorSmallText] = QStringLiteral("MidnightBlue");
    colorTokens[ColorLineText] = QStringLiteral("MidnightBlue");
    colorTokens[ColorGrid] = QStringLiteral("#E8E8E8");
    colorTokens[ColorStartRoom] = QStringLiteral("GreenYellow");
    colorTokens[ColorEndRoom] = QStringLiteral("Red");

    const QString family = QStringLiteral("Arial");
    roomFont = FontSpec{family, 13.0, false, false, false, false};
    objectFont = FontSpec{family, 11.0, false, false, false, false};
    subtitleFont = FontSpec{family, 9.0, false, false, false, false};
    lineFont = FontSpec{family, 9.0, false, false, false, false};
}

void Map::clear()
{
    version.clear();
    title.clear();
    author.clear();
    description.clear();
    history.clear();
    rooms.clear();
    connections.clear();
    regions.clear();
    settings = MapSettings();
    m_roomIndex.clear();
}

void Map::reindex()
{
    m_roomIndex.clear();
    for (int i = 0; i < rooms.size(); ++i)
        m_roomIndex.insert(rooms.at(i).id, i);
}

const Room *Map::roomById(int id) const
{
    const auto it = m_roomIndex.constFind(id);
    if (it == m_roomIndex.constEnd())
        return nullptr;
    return &rooms.at(it.value());
}

Room *Map::roomById(int id)
{
    const auto it = m_roomIndex.constFind(id);
    if (it == m_roomIndex.constEnd())
        return nullptr;
    return &rooms[it.value()];
}

const Region *Map::regionByName(const QString &name) const
{
    for (const Region &r : regions) {
        if (r.name == name)
            return &r;
    }
    return nullptr;
}

QColor Map::regionFill(const QString &name) const
{
    if (name.isEmpty() || name == kNoRegion)
        return QColor();
    if (const Region *r = regionByName(name))
        return r->fillColor;
    return QColor();
}

int Map::nextRoomId() const
{
    int maxId = 0;
    for (const Room &r : rooms)
        maxId = qMax(maxId, r.id);
    return maxId + 1;
}

int Map::nextConnectionId() const
{
    int maxId = 0;
    for (const Room &r : rooms)
        maxId = qMax(maxId, r.id);
    for (const Connection &c : connections)
        maxId = qMax(maxId, c.id);
    return maxId + 1;
}

QColor Map::canvasColor() const { return settings.colors[ColorCanvas]; }
QColor Map::lineColor() const { return settings.colors[ColorLine]; }
QColor Map::borderColor() const { return settings.colors[ColorBorder]; }

int Map::nextSeq() const
{
    int maxSeq = 0;
    for (const Room &r : rooms)
        maxSeq = qMax(maxSeq, r.seq);
    for (const Connection &c : connections)
        maxSeq = qMax(maxSeq, c.seq);
    return maxSeq + 1;
}

int Map::addRoom(double x, double y)
{
    Room r;
    r.id = nextRoomId();
    r.seq = nextSeq();
    r.name = settings.defaultRoomName;
    r.x = x;
    r.y = y;
    // The document's default room shape maps onto the boolean shape flags.
    switch (static_cast<RoomShape>(settings.defaultRoomShape)) {
    case RoomShape::RoundedCorners: r.roundedCorners = true; break;
    case RoomShape::Ellipse: r.ellipse = true; break;
    case RoomShape::Octagonal: r.octagonal = true; break;
    default: break;
    }
    rooms.append(r);
    reindex();
    return r.id;
}

void Map::removeRoom(int roomId)
{
    for (int i = connections.size() - 1; i >= 0; --i) {
        for (const Vertex &v : connections.at(i).vertices) {
            if (v.docked && v.roomId == roomId) {
                connections.removeAt(i);
                break;
            }
        }
    }
    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms.at(i).id == roomId) {
            rooms.removeAt(i);
            break;
        }
    }
    reindex();
}

int Map::addConnection(int roomA, const QString &portA, int roomB, const QString &portB)
{
    Connection c;
    c.id = nextConnectionId();
    c.seq = nextSeq();

    Vertex a;
    a.index = 0;
    a.docked = true;
    a.roomId = roomA;
    a.port = portA;
    Vertex b;
    b.index = 1;
    b.docked = true;
    b.roomId = roomB;
    b.port = portB;
    c.vertices.append(a);
    c.vertices.append(b);

    connections.append(c);
    return c.id;
}

int Map::connectionIndex(int connId) const
{
    for (int i = 0; i < connections.size(); ++i)
        if (connections.at(i).id == connId)
            return i;
    return -1;
}

void Map::removeConnection(int connId)
{
    const int i = connectionIndex(connId);
    if (i >= 0)
        connections.removeAt(i);
}

QString Map::portFacing(const Room &from, const Room &to)
{
    const double dx = (to.x + to.w / 2.0) - (from.x + from.w / 2.0);
    const double dy = (to.y + to.h / 2.0) - (from.y + from.h / 2.0);
    // Screen coordinates: +y is down. Classify into 8 compass ports by octant.
    const double adx = qAbs(dx);
    const double ady = qAbs(dy);
    const bool horizontal = adx > ady * 2.0;
    const bool vertical = ady > adx * 2.0;
    if (vertical)
        return dy < 0 ? QStringLiteral("n") : QStringLiteral("s");
    if (horizontal)
        return dx < 0 ? QStringLiteral("w") : QStringLiteral("e");
    if (dx >= 0 && dy < 0) return QStringLiteral("ne");
    if (dx >= 0 && dy >= 0) return QStringLiteral("se");
    if (dx < 0 && dy < 0) return QStringLiteral("nw");
    return QStringLiteral("sw");
}

} // namespace trizbort
