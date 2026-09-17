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

#include "TranscriptAutomapper.h"

#include <cmath>

#include <QHash>
#include <QRectF>
#include <QRegularExpression>

namespace trizbort {

namespace {

const int kMaxCharactersBeforePrompt = 42;

// Maps a movement word to a canonical direction key. Ported from
// s_namesForMovementCommands.
QString directionKeyForWord(const QString &word)
{
    static const QHash<QString, QString> kWords = {
        {"north", "n"}, {"n", "n"}, {"fore", "n"}, {"f", "n"},
        {"south", "s"}, {"s", "s"}, {"aft", "s"}, {"a", "s"},
        {"east", "e"}, {"e", "e"}, {"starboard", "e"}, {"sb", "e"},
        {"west", "w"}, {"w", "w"}, {"port", "w"}, {"p", "w"},
        {"northeast", "ne"}, {"ne", "ne"},
        {"southeast", "se"}, {"se", "se"},
        {"southwest", "sw"}, {"sw", "sw"},
        {"northwest", "nw"}, {"nw", "nw"},
        {"up", "u"}, {"u", "u"},
        {"down", "d"}, {"d", "d"},
        {"in", "in"}, {"inside", "in"},
        {"out", "out"}, {"outside", "out"},
    };
    return kWords.value(word.toLower());
}

// The screen-space unit step for a direction key (Automap direction vectors:
// up/down/in/out map onto north/south/east/west).
void directionVector(const QString &dir, int &dx, int &dy)
{
    dx = 0;
    dy = 0;
    if (dir == "n" || dir == "u") { dy = -1; }
    else if (dir == "s" || dir == "d") { dy = 1; }
    else if (dir == "e" || dir == "in") { dx = 1; }
    else if (dir == "w" || dir == "out") { dx = -1; }
    else if (dir == "ne") { dx = 1; dy = -1; }
    else if (dir == "nw") { dx = -1; dy = -1; }
    else if (dir == "se") { dx = 1; dy = 1; }
    else if (dir == "sw") { dx = -1; dy = 1; }
}

QString portFromVector(int dx, int dy)
{
    if (dx == 0 && dy < 0) return QStringLiteral("n");
    if (dx == 0 && dy > 0) return QStringLiteral("s");
    if (dx > 0 && dy == 0) return QStringLiteral("e");
    if (dx < 0 && dy == 0) return QStringLiteral("w");
    if (dx > 0 && dy < 0) return QStringLiteral("ne");
    if (dx < 0 && dy < 0) return QStringLiteral("nw");
    if (dx > 0 && dy > 0) return QStringLiteral("se");
    return QStringLiteral("sw");
}

// A label for a vertical/inside connection, or empty for a compass one.
QString stalkLabel(const QString &dir)
{
    if (dir == "u") return QStringLiteral("up");
    if (dir == "d") return QStringLiteral("down");
    if (dir == "in") return QStringLiteral("in");
    if (dir == "out") return QStringLiteral("out");
    return QString();
}

bool startsWithCapitalOrNonLetter(const QString &word)
{
    if (word.isEmpty())
        return false;
    const QChar c = word.at(0);
    if (c.isLetter() && !c.isUpper())
        return false;
    return true;
}

bool isRoomDescriptionWord(const QString &word)
{
    if (word.isEmpty())
        return false;
    const QChar c = word.at(0);
    return c.isLetterOrNumber() || c == QLatin1Char('#');
}

bool isAllCaps(const QString &word) { return word.toUpper() == word; }

} // namespace

TranscriptAutomapper::TranscriptAutomapper(const AutomapSettings &settings)
    : m_settings(settings)
{
}

bool TranscriptAutomapper::isPrompt(const QString &line, QString &command) const
{
    const int start = line.lastIndexOf(QLatin1Char('>'));
    if (start != -1 && start < kMaxCharactersBeforePrompt) {
        command = line.mid(start + 1).trimmed();
        return true;
    }
    command.clear();
    return false;
}

bool TranscriptAutomapper::extractRoomName(const QString &lineIn, const QString &previousLine,
                                           QString &name) const
{
    name.clear();
    QString unused;
    if (!previousLine.isNull() && previousLine.trimmed().length() > 0
        && !isPrompt(previousLine, unused))
        return false; // the previous line, if any, must be blank or a prompt

    if (lineIn.isEmpty())
        return false;
    if (lineIn.at(0).isSpace())
        return false; // lines which start with whitespace aren't room names

    QString line = lineIn.trimmed();
    if (line.isEmpty())
        return false;

    static const QStringList suffixMarkers = {",", "(", "[", "{", " - "};
    bool stripped = true;
    while (stripped) {
        stripped = false;
        for (const QString &marker : suffixMarkers) {
            const int idx = line.indexOf(marker);
            if (idx >= 0) {
                if (line.length() - idx > 30)
                    return false; // looks like a sentence
                line = line.left(idx);
                stripped = true;
                break;
            }
        }
    }

    line = line.trimmed();
    if (line.isEmpty())
        return false;
    if (!line.at(line.length() - 1).isLetterOrNumber())
        return false;
    if (!line.at(0).isLetterOrNumber())
        return false;
    if (!startsWithCapitalOrNonLetter(line))
        return false;

    const QStringList words = line.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (words.isEmpty())
        return false;

    int maxWordLength = 0;
    int allCapsCount = 0;
    for (const QString &word : words) {
        if (!isRoomDescriptionWord(word))
            return false;
        if (!startsWithCapitalOrNonLetter(word) && word.length() >= 4)
            return false;
        maxWordLength = qMax(maxWordLength, word.length());
        if (isAllCaps(word))
            ++allCapsCount;
    }
    if (words.size() > 1 && maxWordLength < 3)
        return false;
    if (allCapsCount == words.size())
        return false;

    name = line;
    return true;
}

bool TranscriptAutomapper::extractParagraph(const QStringList &lines, int lineIndex,
                                            QString &paragraph) const
{
    paragraph.clear();
    bool have = false;
    while (lineIndex < lines.size()) {
        QString line = lines.at(lineIndex).trimmed();
        if (line == QLatin1String("[Previous turn undone.]")) {
            ++lineIndex;
            continue;
        }
        if (line.isEmpty())
            break;
        QString unused;
        QString unusedName;
        if (isPrompt(line, unused)
            || extractRoomName(line, lineIndex > 0 ? lines.at(lineIndex - 1) : QString(), unusedName))
            break;
        paragraph = have ? paragraph + QLatin1Char(' ') + line : line;
        have = true;
        if (line.length() < 65)
            break;
        ++lineIndex;
    }
    return have;
}

int TranscriptAutomapper::findRoom(const QString &name)
{
    QList<int> candidates;
    for (const Room &r : m_map->rooms) {
        if (r.name == name)
            candidates.append(r.id);
    }
    if (candidates.isEmpty())
        return -1;
    // A single match is always the same room. A terse (non-verbose) transcript
    // has no descriptions to tell rooms apart, so same-named rooms are taken to
    // be the same room without asking (mirrors Automap.cs).
    if (candidates.size() == 1 || m_settings.assumeSameNameSameRoom ||
        !m_settings.verboseTranscript)
        return candidates.first();
    // Ambiguous: let the controller decide (returns -1 to make a new room).
    return m_controller ? m_controller->disambiguateRoom(name, candidates) : candidates.first();
}

bool TranscriptAutomapper::anyRoomIntersects(const QRectF &bounds, int exceptId) const
{
    for (const Room &r : m_map->rooms) {
        if (r.id == exceptId)
            continue;
        if (QRectF(r.x, r.y, r.w, r.h).intersects(bounds))
            return true;
    }
    return false;
}

void TranscriptAutomapper::shiftMap(const QRectF &origin, double dx, double dy)
{
    // Push rooms on the far side of the origin away by the delta, opening space.
    for (Room &r : m_map->rooms) {
        const QRectF b(r.x, r.y, r.w, r.h);
        if (dx < 0) {
            if (b.center().x() < origin.right())
                r.x += dx;
        } else if (dx > 0) {
            if (b.center().x() > origin.left())
                r.x += dx;
        }
    }
    for (Room &r : m_map->rooms) {
        const QRectF b(r.x, r.y, r.w, r.h);
        if (dy < 0) {
            if (b.center().y() < origin.bottom())
                r.y += dy;
        } else if (b.center().y() > origin.top()) {
            if (b.bottom() > origin.y())
                r.y += dy;
        }
    }
}

int TranscriptAutomapper::createRoomInDirection(int existingId, const QString &dir,
                                                const QString &roomName, const QString &line)
{
    const Room *existing = m_map->roomById(existingId);
    if (!existing)
        return -1;

    const double newW = 96.0;
    const double newH = 64.0;
    int dx = 0;
    int dy = 0;
    directionVector(dir, dx, dy);
    const double deltaX = dx * (m_settings.preferredDistanceBetweenRooms + newW);
    const double deltaY = dy * (m_settings.preferredDistanceBetweenRooms + newH);

    const double g = m_settings.gridSize > 1 ? m_settings.gridSize : 32.0;
    const double centerX = existing->x + existing->w / 2.0 + deltaX;
    const double centerY = existing->y + existing->h / 2.0 + deltaY;
    const double nx = std::round((centerX - newW / 2.0) / g) * g;
    const double ny = std::round((centerY - newH / 2.0) / g) * g;

    Room room;
    room.id = m_map->nextRoomId();
    room.seq = m_map->nextSeq();
    room.name = roomName;
    if (line != roomName) {
        QString subtitle = line;
        subtitle.replace(roomName, QString());
        room.subtitle = subtitle;
    }
    room.w = newW;
    room.h = newH;
    room.x = nx;
    room.y = ny;

    if (anyRoomIntersects(QRectF(room.x, room.y, room.w, room.h), room.id))
        shiftMap(QRectF(room.x, room.y, room.w, room.h), deltaX, deltaY);

    m_map->rooms.append(room);
    m_map->reindex();
    ++m_roomsAdded;
    return room.id;
}

int TranscriptAutomapper::roomInDirection(int fromId, const QString &dir) const
{
    const Room *from = m_map->roomById(fromId);
    if (!from)
        return -1;
    int dx = 0;
    int dy = 0;
    directionVector(dir, dx, dy);
    if (dx == 0 && dy == 0)
        return -1;
    const double newW = 96.0;
    const double newH = 64.0;
    const double deltaX = dx * (m_settings.preferredDistanceBetweenRooms + newW);
    const double deltaY = dy * (m_settings.preferredDistanceBetweenRooms + newH);
    const double g = m_settings.gridSize > 1 ? m_settings.gridSize : 32.0;
    const double cx = from->x + from->w / 2.0 + deltaX;
    const double cy = from->y + from->h / 2.0 + deltaY;
    const QRectF cell(std::round((cx - newW / 2.0) / g) * g, std::round((cy - newH / 2.0) / g) * g,
                      newW, newH);
    for (const Room &r : m_map->rooms) {
        if (r.id == fromId)
            continue;
        if (QRectF(r.x, r.y, r.w, r.h).intersects(cell))
            return r.id;
    }
    return -1;
}

void TranscriptAutomapper::removeRoomById(int roomId)
{
    m_map->removeRoom(roomId); // also drops connections docked to it
    m_map->reindex();
}

void TranscriptAutomapper::addExitStub(int roomId, const QString &dir)
{
    const Room *r = m_map->roomById(roomId);
    if (!r)
        return;
    int dx = 0;
    int dy = 0;
    directionVector(dir, dx, dy);
    if (dx == 0 && dy == 0)
        return;
    // Don't stub where a stub already exists.
    const QString port = portFromVector(dx, dy);
    for (const Connection &c : m_map->connections) {
        bool dockHere = false;
        bool hasFree = false;
        for (const Vertex &v : c.vertices) {
            if (v.docked && v.roomId == roomId && v.port == port)
                dockHere = true;
            if (!v.docked)
                hasFree = true;
        }
        if (dockHere && hasFree)
            return;
    }
    const double px = r->x + r->w * (dx > 0 ? 1.0 : dx < 0 ? 0.0 : 0.5);
    const double py = r->y + r->h * (dy > 0 ? 1.0 : dy < 0 ? 0.0 : 0.5);
    const double stub = m_settings.preferredDistanceBetweenRooms / 2.0 + 16.0;

    Connection c;
    c.id = m_map->nextConnectionId();
    c.seq = m_map->nextSeq();
    Vertex a;
    a.index = 0;
    a.docked = true;
    a.roomId = roomId;
    a.port = port;
    Vertex b;
    b.index = 1;
    b.docked = false;
    b.point = QPointF(px + dx * stub, py + dy * stub);
    c.vertices << a << b;
    m_map->connections.append(c);
    ++m_connectionsAdded;
}

void TranscriptAutomapper::removeExitStub(int roomId, const QString &dir)
{
    int dx = 0;
    int dy = 0;
    directionVector(dir, dx, dy);
    if (dx == 0 && dy == 0)
        return;
    const QString port = portFromVector(dx, dy);
    for (int i = m_map->connections.size() - 1; i >= 0; --i) {
        const Connection &c = m_map->connections.at(i);
        bool dockHere = false;
        bool hasFree = false;
        for (const Vertex &v : c.vertices) {
            if (v.docked && v.roomId == roomId && v.port == port)
                dockHere = true;
            if (!v.docked)
                hasFree = true;
        }
        if (dockHere && hasFree) {
            m_map->connections.removeAt(i);
            return;
        }
    }
}

void TranscriptAutomapper::guessExitsFromDescription(int roomId, const QString &description)
{
    if (!m_settings.guessExits || description.isEmpty())
        return;
    static const QStringList dirWords = {"north",     "south",     "east",     "west",
                                         "northeast", "northwest", "southeast", "southwest",
                                         "up",        "down"};
    const QString lower = description.toLower();
    for (const QString &w : dirWords) {
        if (lower.contains(QRegularExpression(QStringLiteral("\\b") + w + QStringLiteral("\\b")))) {
            const QString dir = directionKeyForWord(w);
            if (!dir.isEmpty() && roomInDirection(roomId, dir) < 0)
                addExitStub(roomId, dir);
        }
    }
}

int TranscriptAutomapper::createRoomTeleport(const QString &name)
{
    const double newW = 96.0;
    const double newH = 64.0;
    const double g = m_settings.gridSize > 1 ? m_settings.gridSize : 32.0;

    const Room *existing = m_map->roomById(m_lastRoomId);
    double baseX = existing ? existing->x : 0.0;
    double baseY = existing ? existing->y : 0.0;
    baseX = std::round(baseX / g) * g;
    baseY = std::round(baseY / g) * g;

    Room room;
    room.id = m_map->nextRoomId();
    room.seq = m_map->nextSeq();
    room.name = name;
    room.w = newW;
    room.h = newH;
    room.x = baseX;
    room.y = baseY;

    // Alternate left then right, expanding, until a blank space is found.
    bool tryOtherSideNext = false;
    bool tryLeft = true;
    int distance = 0;
    const double initialX = room.x;
    while (anyRoomIntersects(QRectF(room.x, room.y, room.w, room.h), room.id)) {
        if (tryOtherSideNext) {
            tryLeft = !tryLeft;
            tryOtherSideNext = false;
        } else {
            tryOtherSideNext = true;
            ++distance;
        }
        const double f = distance * (m_settings.preferredDistanceBetweenRooms + room.w);
        const double vx = tryLeft ? initialX - f : initialX + f;
        room.x = std::round(vx / g) * g;
    }

    m_map->rooms.append(room);
    m_map->reindex();
    ++m_roomsAdded;
    return room.id;
}

void TranscriptAutomapper::connectRooms(int sourceId, const QString &dir, int targetId)
{
    if (!m_map->roomById(sourceId) || !m_map->roomById(targetId) || sourceId == targetId)
        return;

    // A real move supersedes any exit stub that pointed this way.
    removeExitStub(sourceId, dir);

    int dx = 0;
    int dy = 0;
    directionVector(dir, dx, dy);
    const QString srcPort = portFromVector(dx, dy);
    const QString tgtPort = portFromVector(-dx, -dy);

    // If the rooms are already connected, leave it (upgrading a one-way that now
    // goes the other way to two-way), rather than adding a duplicate edge.
    for (Connection &c : m_map->connections) {
        if (c.vertices.size() < 2)
            continue;
        const int a = c.vertices.at(0).docked ? c.vertices.at(0).roomId : -1;
        const int b = c.vertices.at(1).docked ? c.vertices.at(1).roomId : -1;
        if ((a == sourceId && b == targetId) || (a == targetId && b == sourceId)) {
            if (a == targetId && c.flow == ConnectionFlow::OneWay)
                c.flow = ConnectionFlow::TwoWay;
            return;
        }
    }

    Connection conn;
    conn.id = m_map->nextConnectionId();
    conn.seq = m_map->nextSeq();
    conn.flow = m_settings.assumeTwoWayConnections ? ConnectionFlow::TwoWay : ConnectionFlow::OneWay;
    conn.style = m_useDottedConnection ? ConnectionStyle::Dashed : ConnectionStyle::Solid;
    m_useDottedConnection = false;
    const QString label = stalkLabel(dir);
    if (!label.isEmpty())
        conn.startText = label;

    Vertex v0;
    v0.index = 0;
    v0.docked = true;
    v0.roomId = sourceId;
    v0.port = srcPort;
    Vertex v1;
    v1.index = 1;
    v1.docked = true;
    v1.roomId = targetId;
    v1.port = tgtPort;
    conn.vertices << v0 << v1;

    m_map->connections.append(conn);
    ++m_connectionsAdded;
}

void TranscriptAutomapper::processTranscriptText(const QStringList &lines)
{
    QString previousLine; // null
    for (int index = 0; index < lines.size(); ++index) {
        const QString &line = lines.at(index);
        QString roomName;
        if (extractRoomName(line, previousLine, roomName)) {
            QString roomDescription;
            extractParagraph(lines, index + 1, roomDescription);

            int roomId = findRoom(roomName);
            if (roomId < 0) {
                // new room
                if (m_lastRoomId >= 0 && !m_lastMoveDir.isEmpty()) {
                    const int existingThere = roomInDirection(m_lastRoomId, m_lastMoveDir);
                    if (existingThere >= 0 && existingThere != m_lastRoomId && m_controller) {
                        // A different room already sits where we moved: ask.
                        const AutomapController::SameDir choice =
                            m_controller->sameDirection(m_lastRoomId, existingThere, m_lastMoveDir);
                        if (choice == AutomapController::SameDir::KeepExisting) {
                            connectRooms(m_lastRoomId, m_lastMoveDir, existingThere);
                            roomId = existingThere;
                        } else {
                            if (choice == AutomapController::SameDir::KeepNew)
                                removeRoomById(existingThere);
                            roomId = createRoomInDirection(m_lastRoomId, m_lastMoveDir, roomName, line);
                            connectRooms(m_lastRoomId, m_lastMoveDir, roomId);
                        }
                    } else {
                        roomId = createRoomInDirection(m_lastRoomId, m_lastMoveDir, roomName, line);
                        connectRooms(m_lastRoomId, m_lastMoveDir, roomId);
                    }
                } else {
                    if (m_firstRoom || m_gameName == roomName) {
                        // most likely the game title
                        m_firstRoom = false;
                        m_gameName = roomName;
                        roomId = -1;
                    } else {
                        // teleported to a new room; don't connect it up
                        const bool wasFirst = (m_lastRoomId < 0);
                        roomId = createRoomTeleport(roomName);
                        if (wasFirst) {
                            if (Room *r = m_map->roomById(roomId))
                                r->isStartRoom = true;
                        }
                    }
                }
                if (roomId >= 0)
                    m_lastRoomId = roomId;
            } else if (roomId != m_lastRoomId) {
                // moved to an existing room
                if (m_lastRoomId >= 0 && !m_lastMoveDir.isEmpty())
                    connectRooms(m_lastRoomId, m_lastMoveDir, roomId);
                m_lastRoomId = roomId;
            }
            // else: still in the same room

            if (roomId >= 0 && !roomDescription.isEmpty()) {
                if (Room *r = m_map->roomById(roomId)) {
                    if (r->description.isEmpty())
                        r->description = roomDescription;
                }
                guessExitsFromDescription(roomId, roomDescription);
            }
            m_lastMoveDir.clear();
        }
        previousLine = line;
    }
}

void TranscriptAutomapper::processPromptCommand(const QString &commandIn)
{
    m_lastMoveDir.clear();
    const QString command = commandIn;

    // <region command> <name>  (default "tb region")
    if (command.startsWith(m_settings.regionCommand, Qt::CaseInsensitive)) {
        const QString regionName = command.mid(m_settings.regionCommand.length()).trimmed();
        if (!regionName.isEmpty() && m_lastRoomId >= 0) {
            if (!m_map->regionByName(regionName))
                m_map->regions.append(
                    Region{regionName, QColor(25, 25, 112), QColor(255, 255, 255), QString(), QString()});
            if (Room *r = m_map->roomById(m_lastRoomId))
                r->region = regionName;
        }
        return;
    }
    // <object command> <object>  (default "tb see")
    if (command.startsWith(m_settings.objectCommand, Qt::CaseInsensitive)) {
        const QString objectName = command.mid(m_settings.objectCommand.length()).trimmed();
        if (!objectName.isEmpty() && m_lastRoomId >= 0) {
            if (Room *r = m_map->roomById(m_lastRoomId)) {
                QString text = r->objectsText;
                text.remove(QLatin1Char('\r'));
                const QStringList existing = text.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
                bool found = false;
                for (const QString &o : existing) {
                    if (o.trimmed().compare(objectName, Qt::CaseInsensitive) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    r->objectsText = r->objectsText.isEmpty()
                        ? objectName
                        : r->objectsText + QStringLiteral("\r\n") + objectName;
            }
        }
        return;
    }

    QStringList parts = command.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return;

    static const QStringList strip = {"the", "a", "to", "on"};
    QStringList words;
    for (const QString &word : parts) {
        bool skip = false;
        for (const QString &s : strip) {
            if (word.compare(s, Qt::CaseInsensitive) == 0) {
                skip = true;
                break;
            }
        }
        if (!word.isEmpty() && word.at(0) == QLatin1Char('[')) {
            // strip bracketed turn counters like "[12]"
            for (int i = 1; i < word.length(); ++i) {
                if (word.at(i) == QLatin1Char(']')) {
                    skip = true;
                    break;
                }
                if (!word.at(i).isDigit())
                    break;
            }
        }
        if (!skip)
            words.append(word);
    }

    static const QStringList goWords = {"go", "walk", "move"};
    if (!words.isEmpty()) {
        for (const QString &g : goWords) {
            if (words.first().compare(g, Qt::CaseInsensitive) == 0) {
                words.removeFirst();
                break;
            }
        }
    }

    // trypush <dir>: nudge the current room one step in a direction.
    if (!words.isEmpty()
        && words.first().compare(QLatin1String("trypush"), Qt::CaseInsensitive) == 0) {
        if (words.size() > 1 && m_lastRoomId >= 0) {
            const QString d = directionKeyForWord(words.at(1));
            int dx = 0;
            int dy = 0;
            directionVector(d, dx, dy);
            if ((dx != 0 || dy != 0)) {
                if (Room *r = m_map->roomById(m_lastRoomId)) {
                    const double gap = m_settings.preferredDistanceBetweenRooms;
                    r->x += dx * (r->w + gap);
                    r->y += dy * (r->h + gap);
                }
            }
        }
        return;
    }

    // tb dotted / tb exit <dir> / tb noexit <dir>
    if (!words.isEmpty() && words.first().compare(QLatin1String("tb"), Qt::CaseInsensitive) == 0) {
        if (words.size() > 1) {
            const QString sub = words.at(1).toLower();
            if (sub == QLatin1String("dotted")) {
                m_useDottedConnection = true;
            } else if (sub == QLatin1String("exit") && words.size() > 2 && m_lastRoomId >= 0) {
                const QString d = directionKeyForWord(words.at(2));
                if (!d.isEmpty())
                    addExitStub(m_lastRoomId, d);
            } else if (sub == QLatin1String("noexit") && words.size() > 2 && m_lastRoomId >= 0) {
                const QString d = directionKeyForWord(words.at(2));
                if (!d.isEmpty())
                    removeExitStub(m_lastRoomId, d);
            }
        }
        return;
    }

    if (words.size() != 1)
        return;

    const QString dir = directionKeyForWord(words.first());
    if (!dir.isEmpty())
        m_lastMoveDir = dir;
}

namespace {
// The non-interactive default controller used by batch run().
AutomapController &defaultController()
{
    static AutomapController controller;
    return controller;
}
} // namespace

void TranscriptAutomapper::begin(Map &map, AutomapController *controller)
{
    m_map = &map;
    m_controller = controller ? controller : &defaultController();
    m_lastRoomId = -1;
    m_lastMoveDir.clear();
    m_firstRoom = true;
    m_gameName.clear();
    m_useDottedConnection = false;
    m_buffer.clear();
    m_pos = 0;
    m_between.clear();
    m_cancelled = false;
    m_roomsAdded = 0;
    m_connectionsAdded = 0;
}

bool TranscriptAutomapper::feed(const QStringList &lines, bool final)
{
    m_buffer += lines;
    processBuffer(final);
    if (m_map)
        m_map->reindex();
    return !m_cancelled;
}

void TranscriptAutomapper::processBuffer(bool final)
{
    while (m_pos < m_buffer.size()) {
        if (m_cancelled)
            return;
        const QString line = m_buffer.at(m_pos);
        QString command;
        if (isPrompt(line, command)) {
            // Gate for single-stepping before each command.
            if (m_controller && !m_controller->step()) {
                m_cancelled = true;
                return;
            }
            processTranscriptText(m_between);
            m_between.clear();
            processPromptCommand(command);
        } else {
            m_between.append(line);
        }
        ++m_pos;
    }
    if (final) {
        // The last room in a saved transcript often has no following prompt.
        if (!m_between.isEmpty() && m_controller && !m_controller->step()) {
            m_cancelled = true;
            return;
        }
        processTranscriptText(m_between);
        m_between.clear();
    }
}

int TranscriptAutomapper::run(Map &map, const QStringList &lines)
{
    begin(map, nullptr);
    feed(lines, true);
    return m_roomsAdded;
}

int TranscriptAutomapper::run(Map &map, const QString &text)
{
    QString normalized = text;
    normalized.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    normalized.replace(QLatin1Char('\r'), QLatin1Char('\n'));
    return run(map, normalized.split(QLatin1Char('\n')));
}

} // namespace trizbort
