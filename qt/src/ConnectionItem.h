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

#ifndef TRIZBORT_CONNECTIONITEM_H
#define TRIZBORT_CONNECTIONITEM_H

#include <QGraphicsItem>
#include <QVector>

#include "MapDocument.h"

namespace trizbort {

class MapScene;

// A connection drawn as a polyline between its endpoints. Recomputes its route
// from the document whenever an attached room moves or the model changes.
class ConnectionItem : public QGraphicsItem {
public:
    ConnectionItem(MapScene *scene, int connId);

    int connId() const { return m_connId; }
    void updateRoute();      // recompute geometry from the model
    const QVector<QPointF> &routePoints() const { return m_points; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    // Insert a free waypoint at a scene point (used from the context menu).
    void addWaypointAt(const QPointF &scenePos);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // Scene-space points of the connection's vertices (endpoints + waypoints),
    // in index order; used to draw and hit-test the editing handles.
    QVector<QPointF> vertexHandlePoints() const;
    int vertexHandleAt(const QPointF &localPos) const;

    MapScene *m_scene;
    int m_connId;
    QVector<QPointF> m_points;
    int m_dragVertex = -1;      // index of the vertex being dragged, or -1
    Connection m_dragBefore;    // snapshot for the undo command
    bool m_hover = false;       // mouse hovering this connection
};

} // namespace trizbort

#endif // TRIZBORT_CONNECTIONITEM_H
