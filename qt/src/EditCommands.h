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

#ifndef TRIZBORT_EDITCOMMANDS_H
#define TRIZBORT_EDITCOMMANDS_H

#include <QList>
#include <QPointF>
#include <QUndoCommand>

#include "MapDocument.h"

// Undoable editing operations. Each command mutates the scene's document and
// refreshes the affected items, so both directions leave a consistent canvas.
namespace trizbort {

class MapScene;

class AddRoomCommand : public QUndoCommand {
public:
    AddRoomCommand(MapScene *scene, double x, double y);
    void redo() override;
    void undo() override;
    int roomId() const { return m_room.id; }

private:
    MapScene *m_scene;
    Room m_room;
};

class DeleteElementsCommand : public QUndoCommand {
public:
    DeleteElementsCommand(MapScene *scene, const QList<int> &roomIds, const QList<int> &connIds);
    void redo() override;
    void undo() override;
    bool isEmpty() const { return m_rooms.isEmpty() && m_conns.isEmpty(); }

private:
    template <class T> struct Entry {
        int index;
        T value;
    };
    MapScene *m_scene;
    QList<Entry<Room>> m_rooms;
    QList<Entry<Connection>> m_conns;
};

struct RoomMove {
    int id;
    QPointF oldPos;
    QPointF newPos;
};

class MoveRoomsCommand : public QUndoCommand {
public:
    MoveRoomsCommand(MapScene *scene, const QList<RoomMove> &moves);
    void redo() override;
    void undo() override;

private:
    MapScene *m_scene;
    QList<RoomMove> m_moves;
};

class EditRoomCommand : public QUndoCommand {
public:
    EditRoomCommand(MapScene *scene, int roomId, const Room &oldRoom, const Room &newRoom);
    void redo() override;
    void undo() override;

private:
    void ensureRegion(const QString &name);
    MapScene *m_scene;
    int m_id;
    Room m_old;
    Room m_new;
};

// Automap primitive: create a new room one grid step away from an existing room
// in a compass direction, and connect the two. A single undoable step.
class AddConnectedRoomCommand : public QUndoCommand {
public:
    AddConnectedRoomCommand(MapScene *scene, int fromRoomId, const QString &direction);
    void redo() override;
    void undo() override;
    bool valid() const { return m_valid; }
    int newRoomId() const { return m_room.id; }

private:
    MapScene *m_scene;
    Room m_room;
    Connection m_conn;
    bool m_valid = false;
};

class AddConnectionCommand : public QUndoCommand {
public:
    AddConnectionCommand(MapScene *scene, int fromId, const QString &portA, int toId,
                         const QString &portB);
    void redo() override;
    void undo() override;

private:
    MapScene *m_scene;
    Connection m_conn;
};

class EditConnectionCommand : public QUndoCommand {
public:
    EditConnectionCommand(MapScene *scene, int connId, const Connection &oldC,
                          const Connection &newC);
    void redo() override;
    void undo() override;

private:
    MapScene *m_scene;
    int m_id;
    Connection m_old;
    Connection m_new;
};

// Title/author/description/history live on the map, not on any item.
class EditMapInfoCommand : public QUndoCommand {
public:
    struct Info {
        QString title;
        QString author;
        QString description;
        QString history;
    };
    EditMapInfoCommand(MapScene *scene, const Info &oldInfo, const Info &newInfo);
    void redo() override;
    void undo() override;

private:
    void apply(const Info &info);
    MapScene *m_scene;
    Info m_old;
    Info m_new;
};

// Replace the whole map content (rooms, connections, regions) as one step.
// Used by bulk operations such as transcript import.
class ReplaceContentCommand : public QUndoCommand {
public:
    struct Content {
        QList<Room> rooms;
        QList<Connection> connections;
        QList<Region> regions;
    };
    ReplaceContentCommand(MapScene *scene, const Content &oldContent, const Content &newContent,
                          const QString &text);
    void redo() override;
    void undo() override;

private:
    void apply(const Content &content);
    MapScene *m_scene;
    Content m_old;
    Content m_new;
};

// The full settings block (palette, fonts, grid, geometry) plus the region list.
class EditSettingsCommand : public QUndoCommand {
public:
    EditSettingsCommand(MapScene *scene, const MapSettings &oldS, const QList<Region> &oldRegions,
                        const MapSettings &newS, const QList<Region> &newRegions);
    void redo() override;
    void undo() override;

private:
    void apply(const MapSettings &s, const QList<Region> &regions);
    MapScene *m_scene;
    MapSettings m_oldS;
    MapSettings m_newS;
    QList<Region> m_oldRegions;
    QList<Region> m_newRegions;
};

} // namespace trizbort

#endif // TRIZBORT_EDITCOMMANDS_H
