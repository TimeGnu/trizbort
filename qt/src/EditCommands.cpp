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

#include "EditCommands.h"

#include <algorithm>

#include "MapScene.h"

namespace trizbort {

// ---- AddRoomCommand -------------------------------------------------------

AddRoomCommand::AddRoomCommand(MapScene *scene, double x, double y)
    : m_scene(scene)
{
    Map *map = scene->document();
    m_room.id = map->nextRoomId();
    m_room.seq = map->nextSeq();
    m_room.name = map->settings.defaultRoomName;
    m_room.x = x;
    m_room.y = y;
    switch (static_cast<RoomShape>(map->settings.defaultRoomShape)) {
    case RoomShape::RoundedCorners: m_room.roundedCorners = true; break;
    case RoomShape::Ellipse: m_room.ellipse = true; break;
    case RoomShape::Octagonal: m_room.octagonal = true; break;
    default: break;
    }
    setText(QObject::tr("Add Room"));
}

void AddRoomCommand::redo()
{
    Map *map = m_scene->document();
    map->rooms.append(m_room);
    map->reindex();
    m_scene->rebuild();
    m_scene->selectRoomItem(m_room.id);
}

void AddRoomCommand::undo()
{
    m_scene->document()->removeRoom(m_room.id);
    m_scene->rebuild();
}

// ---- DeleteElementsCommand ------------------------------------------------

DeleteElementsCommand::DeleteElementsCommand(MapScene *scene, const QList<int> &roomIds,
                                             const QList<int> &connIds)
    : m_scene(scene)
{
    Map *map = scene->document();

    // Every connection docked to a deleted room is removed too; collect the
    // union with the explicitly-selected connections.
    QList<int> allConnIds = connIds;
    for (int rid : roomIds) {
        for (const Connection &c : map->connections) {
            bool docked = false;
            for (const Vertex &v : c.vertices) {
                if (v.docked && v.roomId == rid) {
                    docked = true;
                    break;
                }
            }
            if (docked && !allConnIds.contains(c.id))
                allConnIds.append(c.id);
        }
    }

    for (int i = 0; i < map->connections.size(); ++i) {
        if (allConnIds.contains(map->connections.at(i).id))
            m_conns.append({i, map->connections.at(i)});
    }
    for (int i = 0; i < map->rooms.size(); ++i) {
        if (roomIds.contains(map->rooms.at(i).id))
            m_rooms.append({i, map->rooms.at(i)});
    }
    setText(QObject::tr("Delete"));
}

void DeleteElementsCommand::redo()
{
    Map *map = m_scene->document();
    for (const auto &e : m_conns)
        map->removeConnection(e.value.id);
    for (const auto &e : m_rooms) {
        for (int i = 0; i < map->rooms.size(); ++i) {
            if (map->rooms.at(i).id == e.value.id) {
                map->rooms.removeAt(i);
                break;
            }
        }
    }
    map->reindex();
    m_scene->rebuild();
}

void DeleteElementsCommand::undo()
{
    Map *map = m_scene->document();
    // Re-insert at original indices in ascending order (they were captured
    // ascending), reconstructing the original element order exactly.
    for (const auto &e : m_rooms)
        map->rooms.insert(qMin(e.index, map->rooms.size()), e.value);
    for (const auto &e : m_conns)
        map->connections.insert(qMin(e.index, map->connections.size()), e.value);
    map->reindex();
    m_scene->rebuild();
}

// ---- MoveRoomsCommand -----------------------------------------------------

MoveRoomsCommand::MoveRoomsCommand(MapScene *scene, const QList<RoomMove> &moves)
    : m_scene(scene)
    , m_moves(moves)
{
    setText(QObject::tr("Move"));
}

void MoveRoomsCommand::redo()
{
    Map *map = m_scene->document();
    for (const RoomMove &m : m_moves) {
        if (Room *r = map->roomById(m.id)) {
            r->x = m.newPos.x();
            r->y = m.newPos.y();
            m_scene->refreshRoom(m.id);
        }
    }
}

void MoveRoomsCommand::undo()
{
    Map *map = m_scene->document();
    for (const RoomMove &m : m_moves) {
        if (Room *r = map->roomById(m.id)) {
            r->x = m.oldPos.x();
            r->y = m.oldPos.y();
            m_scene->refreshRoom(m.id);
        }
    }
}

// ---- EditRoomCommand ------------------------------------------------------

EditRoomCommand::EditRoomCommand(MapScene *scene, int roomId, const Room &oldRoom,
                                 const Room &newRoom)
    : m_scene(scene)
    , m_id(roomId)
    , m_old(oldRoom)
    , m_new(newRoom)
{
    setText(QObject::tr("Edit Room"));
}

void EditRoomCommand::ensureRegion(const QString &name)
{
    Map *map = m_scene->document();
    if (name.isEmpty() || name == kNoRegion || map->regionByName(name))
        return;
    map->regions.append(Region{name, QColor(0, 0, 255), QColor(255, 255, 255), QString(), QString()});
}

void EditRoomCommand::redo()
{
    Map *map = m_scene->document();
    ensureRegion(m_new.region);
    if (Room *r = map->roomById(m_id))
        *r = m_new;
    m_scene->refreshRoom(m_id);
}

void EditRoomCommand::undo()
{
    Map *map = m_scene->document();
    if (Room *r = map->roomById(m_id))
        *r = m_old;
    m_scene->refreshRoom(m_id);
}

// ---- AddConnectedRoomCommand ----------------------------------------------

namespace {

QString oppositePort(const QString &p)
{
    if (p == QLatin1String("n")) return QStringLiteral("s");
    if (p == QLatin1String("s")) return QStringLiteral("n");
    if (p == QLatin1String("e")) return QStringLiteral("w");
    if (p == QLatin1String("w")) return QStringLiteral("e");
    if (p == QLatin1String("ne")) return QStringLiteral("sw");
    if (p == QLatin1String("sw")) return QStringLiteral("ne");
    if (p == QLatin1String("nw")) return QStringLiteral("se");
    if (p == QLatin1String("se")) return QStringLiteral("nw");
    return QStringLiteral("n");
}

// Unit step for a compass port in screen coordinates (+y is down).
void directionVector(const QString &dir, int &dx, int &dy)
{
    dx = (dir.contains(QLatin1Char('e'))) ? 1 : (dir.contains(QLatin1Char('w')) ? -1 : 0);
    dy = (dir.contains(QLatin1Char('s'))) ? 1 : (dir.contains(QLatin1Char('n')) ? -1 : 0);
}

} // namespace

AddConnectedRoomCommand::AddConnectedRoomCommand(MapScene *scene, int fromRoomId,
                                                 const QString &direction)
    : m_scene(scene)
{
    Map *map = scene->document();
    const Room *from = map->roomById(fromRoomId);
    if (!from)
        return;

    int dx = 0;
    int dy = 0;
    directionVector(direction, dx, dy);
    if (dx == 0 && dy == 0)
        return;

    const double newW = 96.0;
    const double newH = 64.0;
    double gap = map->settings.preferredDistanceBetweenRooms;
    if (gap <= 0)
        gap = 64.0;

    double nx = from->x;
    double ny = from->y;
    if (dx > 0)
        nx = from->x + from->w + gap;
    else if (dx < 0)
        nx = from->x - newW - gap;
    if (dy > 0)
        ny = from->y + from->h + gap;
    else if (dy < 0)
        ny = from->y - newH - gap;
    const QPointF snapped = scene->snap(QPointF(nx, ny));

    m_room.id = map->nextRoomId();
    m_room.seq = map->nextSeq();
    m_room.name = map->settings.defaultRoomName;
    m_room.w = newW;
    m_room.h = newH;
    m_room.x = snapped.x();
    m_room.y = snapped.y();
    switch (static_cast<RoomShape>(map->settings.defaultRoomShape)) {
    case RoomShape::RoundedCorners: m_room.roundedCorners = true; break;
    case RoomShape::Ellipse: m_room.ellipse = true; break;
    case RoomShape::Octagonal: m_room.octagonal = true; break;
    default: break;
    }

    m_conn.id = map->nextConnectionId() + 1; // distinct from the room's id space
    m_conn.seq = m_room.seq + 1;
    Vertex a;
    a.index = 0;
    a.docked = true;
    a.roomId = fromRoomId;
    a.port = direction;
    Vertex b;
    b.index = 1;
    b.docked = true;
    b.roomId = m_room.id;
    b.port = oppositePort(direction);
    m_conn.vertices << a << b;

    m_valid = true;
    setText(QObject::tr("Add Room %1").arg(direction.toUpper()));
}

void AddConnectedRoomCommand::redo()
{
    if (!m_valid)
        return;
    Map *map = m_scene->document();
    map->rooms.append(m_room);
    map->connections.append(m_conn);
    map->reindex();
    m_scene->rebuild();
    m_scene->selectRoomItem(m_room.id);
}

void AddConnectedRoomCommand::undo()
{
    if (!m_valid)
        return;
    Map *map = m_scene->document();
    map->removeConnection(m_conn.id);
    map->removeRoom(m_room.id);
    m_scene->rebuild();
}

// ---- AddConnectionCommand -------------------------------------------------

AddConnectionCommand::AddConnectionCommand(MapScene *scene, int fromId, const QString &portA,
                                           int toId, const QString &portB)
    : m_scene(scene)
{
    Map *map = scene->document();
    m_conn.id = map->nextConnectionId();
    m_conn.seq = map->nextSeq();
    Vertex a;
    a.index = 0;
    a.docked = true;
    a.roomId = fromId;
    a.port = portA;
    Vertex b;
    b.index = 1;
    b.docked = true;
    b.roomId = toId;
    b.port = portB;
    m_conn.vertices << a << b;
    setText(QObject::tr("Add Connection"));
}

void AddConnectionCommand::redo()
{
    m_scene->document()->connections.append(m_conn);
    m_scene->rebuild();
}

void AddConnectionCommand::undo()
{
    m_scene->document()->removeConnection(m_conn.id);
    m_scene->rebuild();
}

// ---- EditConnectionCommand ------------------------------------------------

EditConnectionCommand::EditConnectionCommand(MapScene *scene, int connId, const Connection &oldC,
                                             const Connection &newC)
    : m_scene(scene)
    , m_id(connId)
    , m_old(oldC)
    , m_new(newC)
{
    setText(QObject::tr("Edit Connection"));
}

void EditConnectionCommand::redo()
{
    Map *map = m_scene->document();
    const int idx = map->connectionIndex(m_id);
    if (idx >= 0)
        map->connections[idx] = m_new;
    m_scene->refreshConnection(m_id);
}

void EditConnectionCommand::undo()
{
    Map *map = m_scene->document();
    const int idx = map->connectionIndex(m_id);
    if (idx >= 0)
        map->connections[idx] = m_old;
    m_scene->refreshConnection(m_id);
}

// ---- EditMapInfoCommand ---------------------------------------------------

EditMapInfoCommand::EditMapInfoCommand(MapScene *scene, const Info &oldInfo, const Info &newInfo)
    : m_scene(scene)
    , m_old(oldInfo)
    , m_new(newInfo)
{
    setText(QObject::tr("Map Properties"));
}

void EditMapInfoCommand::apply(const Info &info)
{
    Map *map = m_scene->document();
    map->title = info.title;
    map->author = info.author;
    map->description = info.description;
    map->history = info.history;
}

void EditMapInfoCommand::redo() { apply(m_new); }
void EditMapInfoCommand::undo() { apply(m_old); }

// ---- ReplaceContentCommand ------------------------------------------------

ReplaceContentCommand::ReplaceContentCommand(MapScene *scene, const Content &oldContent,
                                             const Content &newContent, const QString &text)
    : m_scene(scene)
    , m_old(oldContent)
    , m_new(newContent)
{
    setText(text);
}

void ReplaceContentCommand::apply(const Content &content)
{
    Map *map = m_scene->document();
    map->rooms = content.rooms;
    map->connections = content.connections;
    map->regions = content.regions;
    map->reindex();
    m_scene->rebuild();
}

void ReplaceContentCommand::redo() { apply(m_new); }
void ReplaceContentCommand::undo() { apply(m_old); }

// ---- EditSettingsCommand --------------------------------------------------

EditSettingsCommand::EditSettingsCommand(MapScene *scene, const MapSettings &oldS,
                                         const QList<Region> &oldRegions, const MapSettings &newS,
                                         const QList<Region> &newRegions)
    : m_scene(scene)
    , m_oldS(oldS)
    , m_newS(newS)
    , m_oldRegions(oldRegions)
    , m_newRegions(newRegions)
{
    setText(QObject::tr("Map Settings"));
}

void EditSettingsCommand::apply(const MapSettings &s, const QList<Region> &regions)
{
    Map *map = m_scene->document();
    map->settings = s;
    map->regions = regions;
    m_scene->rebuild();
}

void EditSettingsCommand::redo() { apply(m_newS, m_newRegions); }
void EditSettingsCommand::undo() { apply(m_oldS, m_oldRegions); }

} // namespace trizbort
