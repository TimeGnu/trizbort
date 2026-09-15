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

#include "MapScene.h"

#include <cmath>

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QUndoStack>

#include "ConnectionItem.h"
#include "EditCommands.h"
#include "RoomItem.h"

namespace trizbort {

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

double MapScene::gridSize() const
{
    if (m_map && m_map->settings.gridSize > 1.0)
        return m_map->settings.gridSize;
    return 32.0;
}

QPointF MapScene::snap(const QPointF &p) const
{
    const double g = gridSize();
    return QPointF(std::round(p.x() / g) * g, std::round(p.y() / g) * g);
}

QPointF MapScene::portPoint(const Room &room, const QString &port)
{
    const double left = room.x;
    const double top = room.y;
    const double right = room.x + room.w;
    const double bottom = room.y + room.h;
    const double cx = room.x + room.w / 2.0;
    const double cy = room.y + room.h / 2.0;

    const QString p = port.toLower();
    if (p == QLatin1String("n"))  return {cx, top};
    if (p == QLatin1String("s"))  return {cx, bottom};
    if (p == QLatin1String("e"))  return {right, cy};
    if (p == QLatin1String("w"))  return {left, cy};
    if (p == QLatin1String("ne")) return {right, top};
    if (p == QLatin1String("nw")) return {left, top};
    if (p == QLatin1String("se")) return {right, bottom};
    if (p == QLatin1String("sw")) return {left, bottom};
    return {cx, cy};
}

void MapScene::setDocument(Map *map)
{
    m_map = map;
    rebuild();
}

void MapScene::rebuild()
{
    clear(); // deletes all items, including any rubber-band line
    m_roomItems.clear();
    m_connItems.clear();
    m_rubberLine = nullptr;
    m_connectFromRoom = -1;
    if (!m_map)
        return;

    setBackgroundBrush(m_map->canvasColor());

    for (const Room &r : m_map->rooms) {
        auto *item = new RoomItem(this, r.id);
        addItem(item);
        m_roomItems.insert(r.id, item);
    }
    rebuildConnections();

    QRectF bounds = itemsBoundingRect().adjusted(-400, -400, 400, 400);
    if (bounds.width() < 1200 || bounds.height() < 900)
        bounds = bounds.united(QRectF(bounds.center() - QPointF(600, 450), QSizeF(1200, 900)));
    setSceneRect(bounds);
    emitSelectionSummary();
}

void MapScene::rebuildConnections()
{
    for (ConnectionItem *item : m_connItems) {
        removeItem(item);
        delete item;
    }
    m_connItems.clear();
    if (!m_map)
        return;
    for (const Connection &c : m_map->connections) {
        auto *item = new ConnectionItem(this, c.id);
        addItem(item);
        m_connItems.append(item);
    }
}

void MapScene::refreshConnectionsFor(int roomId)
{
    if (!m_map)
        return;
    for (ConnectionItem *item : m_connItems) {
        const int idx = m_map->connectionIndex(item->connId());
        if (idx < 0)
            continue;
        bool touches = false;
        for (const Vertex &v : m_map->connections.at(idx).vertices) {
            if (v.docked && v.roomId == roomId) {
                touches = true;
                break;
            }
        }
        if (touches)
            item->updateRoute();
    }
}

void MapScene::refreshRoom(int roomId)
{
    if (RoomItem *item = m_roomItems.value(roomId, nullptr))
        item->syncFromModel();
    refreshConnectionsFor(roomId);
}

void MapScene::refreshConnection(int connId)
{
    for (ConnectionItem *item : m_connItems) {
        if (item->connId() == connId) {
            item->updateRoute();
            return;
        }
    }
}

void MapScene::roomMovedTo(int roomId, const QPointF &topLeft)
{
    if (!m_map)
        return;
    Room *room = m_map->roomById(roomId);
    if (!room)
        return;
    if (room->x == topLeft.x() && room->y == topLeft.y())
        return;
    room->x = topLeft.x();
    room->y = topLeft.y();
    refreshConnectionsFor(roomId);
    emit documentChanged();
}

void MapScene::selectRoomItem(int roomId)
{
    clearSelection();
    if (RoomItem *item = m_roomItems.value(roomId, nullptr))
        item->setSelected(true);
}

int MapScene::selectedRoomId() const
{
    for (QGraphicsItem *item : selectedItems()) {
        if (auto *ri = dynamic_cast<RoomItem *>(item))
            return ri->roomId();
    }
    return -1;
}

int MapScene::addRoomAt(const QPointF &scenePos)
{
    if (!m_map)
        return -1;
    const QPointF tl = snap(scenePos);
    if (m_undo) {
        auto *cmd = new AddRoomCommand(this, tl.x(), tl.y());
        const int id = cmd->roomId();
        m_undo->push(cmd);
        return id;
    }
    // Direct path for headless callers with no undo stack.
    const int id = m_map->addRoom(tl.x(), tl.y());
    auto *item = new RoomItem(this, id);
    addItem(item);
    m_roomItems.insert(id, item);
    setSceneRect(sceneRect().united(item->sceneBoundingRect().adjusted(-400, -400, 400, 400)));
    emit documentChanged();
    return id;
}

void MapScene::deleteSelection()
{
    if (!m_map)
        return;
    QList<int> roomsToDelete;
    QList<int> connsToDelete;
    for (QGraphicsItem *item : selectedItems()) {
        if (auto *ri = dynamic_cast<RoomItem *>(item))
            roomsToDelete.append(ri->roomId());
        else if (auto *ci = dynamic_cast<ConnectionItem *>(item))
            connsToDelete.append(ci->connId());
    }
    if (roomsToDelete.isEmpty() && connsToDelete.isEmpty())
        return;

    if (m_undo) {
        auto *cmd = new DeleteElementsCommand(this, roomsToDelete, connsToDelete);
        if (cmd->isEmpty())
            delete cmd;
        else
            m_undo->push(cmd);
        return;
    }
    for (int id : connsToDelete)
        m_map->removeConnection(id);
    for (int id : roomsToDelete)
        m_map->removeRoom(id); // also drops connections docked to it
    rebuild();
    emit documentChanged();
}

void MapScene::setConnectMode(bool on)
{
    m_connectMode = on;
    // In connect mode, rooms should not drag; toggle their movability.
    for (RoomItem *item : m_roomItems)
        item->setFlag(QGraphicsItem::ItemIsMovable, !on);
}

RoomItem *MapScene::roomItemAt(const QPointF &scenePos) const
{
    const QList<QGraphicsItem *> hit = items(scenePos);
    for (QGraphicsItem *item : hit) {
        if (auto *ri = dynamic_cast<RoomItem *>(item))
            return ri;
    }
    return nullptr;
}

void MapScene::activateRoom(int roomId)
{
    emit editRoomRequested(roomId);
}

void MapScene::activateConnection(int connId)
{
    emit editConnectionRequested(connId);
}

void MapScene::emitSelectionSummary()
{
    if (!m_map) {
        emit selectionSummary(QString());
        return;
    }
    emit selectionSummary(tr("%1 rooms, %2 connections")
                              .arg(m_map->rooms.size())
                              .arg(m_map->connections.size()));
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_connectMode && event->button() == Qt::LeftButton) {
        if (RoomItem *from = roomItemAt(event->scenePos())) {
            m_connectFromRoom = from->roomId();
            m_rubberLine = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
            QPen pen(QColor(30, 120, 220));
            pen.setWidthF(2.0);
            pen.setStyle(Qt::DashLine);
            m_rubberLine->setPen(pen);
            m_rubberLine->setZValue(10);
            addItem(m_rubberLine);
            event->accept();
            return;
        }
    }
    QGraphicsScene::mousePressEvent(event);

    // Snapshot the positions of the rooms about to be dragged, so the move can
    // be pushed as one undoable command on release.
    m_dragStartPos.clear();
    if (event->button() == Qt::LeftButton && !m_connectMode && m_map) {
        for (QGraphicsItem *item : selectedItems()) {
            if (auto *ri = dynamic_cast<RoomItem *>(item)) {
                if (const Room *r = m_map->roomById(ri->roomId()))
                    m_dragStartPos.insert(ri->roomId(), QPointF(r->x, r->y));
            }
        }
    }
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_rubberLine) {
        const QLineF line = m_rubberLine->line();
        m_rubberLine->setLine(QLineF(line.p1(), event->scenePos()));
        event->accept();
        return;
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_rubberLine) {
        removeItem(m_rubberLine);
        delete m_rubberLine;
        m_rubberLine = nullptr;

        RoomItem *target = roomItemAt(event->scenePos());
        if (m_map && target && target->roomId() != m_connectFromRoom) {
            const Room *from = m_map->roomById(m_connectFromRoom);
            const Room *to = m_map->roomById(target->roomId());
            if (from && to) {
                const QString portA = Map::portFacing(*from, *to);
                const QString portB = Map::portFacing(*to, *from);
                if (m_undo) {
                    m_undo->push(
                        new AddConnectionCommand(this, from->id, portA, to->id, portB));
                } else {
                    m_map->addConnection(from->id, portA, to->id, portB);
                    rebuildConnections();
                    emit documentChanged();
                }
            }
        }
        m_connectFromRoom = -1;
        event->accept();
        return;
    }

    QGraphicsScene::mouseReleaseEvent(event);

    // If a drag moved one or more rooms, record it as a single undoable command.
    if (!m_dragStartPos.isEmpty()) {
        QList<RoomMove> moves;
        for (auto it = m_dragStartPos.constBegin(); it != m_dragStartPos.constEnd(); ++it) {
            if (const Room *r = m_map ? m_map->roomById(it.key()) : nullptr) {
                const QPointF cur(r->x, r->y);
                if (cur != it.value())
                    moves.append({it.key(), it.value(), cur});
            }
        }
        m_dragStartPos.clear();
        if (!moves.isEmpty() && m_undo)
            m_undo->push(new MoveRoomsCommand(this, moves));
    }
}

void MapScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
    if (!m_map || !m_map->settings.gridVisible)
        return;

    const double g = gridSize();
    if (g < 2.0)
        return;

    QColor gridColor = m_map->settings.colors[ColorGrid];
    if (!gridColor.isValid())
        gridColor = QColor(235, 235, 235);
    QPen pen(gridColor);
    pen.setWidthF(0.0); // cosmetic
    painter->setPen(pen);

    const double left = std::floor(rect.left() / g) * g;
    const double top = std::floor(rect.top() / g) * g;
    for (double x = left; x < rect.right(); x += g)
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    for (double y = top; y < rect.bottom(); y += g)
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));

    if (m_map->settings.showOrigin) {
        QPen originPen(QColor(200, 160, 160));
        originPen.setWidthF(0.0);
        painter->setPen(originPen);
        painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
        painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));
    }
}

} // namespace trizbort
