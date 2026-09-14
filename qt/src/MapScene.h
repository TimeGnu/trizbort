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

#ifndef TRIZBORT_MAPSCENE_H
#define TRIZBORT_MAPSCENE_H

#include <QColor>
#include <QGraphicsScene>
#include <QPointF>
#include <QRectF>
#include <QString>

#include "MapDocument.h"

namespace trizbort {

// Builds a read-only QGraphicsScene from a Map: rooms as rectangles with
// labels, connections as polylines (dashed/one-way honored). This is the
// rendering seed for the eventual editing canvas.
class MapScene : public QGraphicsScene {
public:
    explicit MapScene(QObject *parent = nullptr);
    void setMap(const Map &map);

private:
    QPointF portPoint(const Room &room, const QString &port) const;
    void addArrowHead(const QPointF &from, const QPointF &to, const QColor &color);
    void addLabel(const QString &text, const QPointF &pos, const QColor &color);
    // A room name centered and shrunk to stay inside the room rectangle.
    void addRoomLabel(const QString &text, const QRectF &rect, const QColor &color);
};

} // namespace trizbort

#endif // TRIZBORT_MAPSCENE_H
