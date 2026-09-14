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

#ifndef TRIZBORT_MAPSCENE_H
#define TRIZBORT_MAPSCENE_H

#include <QGraphicsScene>
#include <QHash>
#include <QPointF>

#include "MapDocument.h"

class QUndoStack;

namespace trizbort {

class RoomItem;
class ConnectionItem;

// The editing canvas: an interactive QGraphicsScene backed by a live Map. Rooms
// and connections are individual items; dragging a room writes back to the
// model and reroutes its connections. A connect tool draws new connections by
// dragging between rooms.
class MapScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit MapScene(QObject *parent = nullptr);

    // Bind to a document (not owned) and (re)build all items.
    void setDocument(Map *map);
    Map *document() const { return m_map; }
    void rebuild();

    // The undo stack edits are pushed to (not owned). When unset, edits apply
    // directly (used by headless callers and rendering).
    void setUndoStack(QUndoStack *stack) { m_undo = stack; }
    QUndoStack *undoStack() const { return m_undo; }

    // Select only the given room's item (used after an add).
    void selectRoomItem(int roomId);
    // The id of a currently-selected room (the first one), or -1.
    int selectedRoomId() const;

    double gridSize() const;
    QPointF snap(const QPointF &p) const;

    // The world-space point of a room's port ("n","s","e",...); centre otherwise.
    static QPointF portPoint(const Room &room, const QString &port);

    // Editing entry points used by the window's actions.
    int addRoomAt(const QPointF &scenePos);   // returns new room id
    void deleteSelection();
    void setConnectMode(bool on);
    bool connectMode() const { return m_connectMode; }

    // Called by RoomItem while dragging: persist the new position and reroute.
    void roomMovedTo(int roomId, const QPointF &topLeft);
    // Called by items on double-click.
    void activateRoom(int roomId);
    void activateConnection(int connId);

    // Refresh a single room's visuals and the connections touching it.
    void refreshRoom(int roomId);
    // Re-route and repaint one connection after its properties changed.
    void refreshConnection(int connId);

signals:
    void documentChanged();                 // model mutated; mark dirty
    void editRoomRequested(int roomId);     // open the room properties dialog
    void editConnectionRequested(int connId);
    void selectionSummary(const QString &text);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void rebuildConnections();
    void refreshConnectionsFor(int roomId);
    RoomItem *roomItemAt(const QPointF &scenePos) const;
    void emitSelectionSummary();

    Map *m_map = nullptr;
    QHash<int, RoomItem *> m_roomItems;
    QList<ConnectionItem *> m_connItems;

    bool m_connectMode = false;
    int m_connectFromRoom = -1;
    QGraphicsLineItem *m_rubberLine = nullptr;

    QUndoStack *m_undo = nullptr;
    QHash<int, QPointF> m_dragStartPos; // room id -> position at drag start
};

} // namespace trizbort

#endif // TRIZBORT_MAPSCENE_H
