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

#include "ConnectionItem.h"

#include <algorithm>
#include <cmath>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPolygonF>

#include "MapScene.h"

namespace trizbort {

ConnectionItem::ConnectionItem(MapScene *scene, int connId)
    : m_scene(scene)
    , m_connId(connId)
{
    setFlags(ItemIsSelectable);
    setZValue(-1);
    updateRoute();
}

void ConnectionItem::updateRoute()
{
    prepareGeometryChange();
    m_points.clear();
    const Map *map = m_scene->document();
    if (!map)
        return;
    const int idx = map->connectionIndex(m_connId);
    if (idx < 0)
        return;
    const Connection &c = map->connections.at(idx);

    QList<Vertex> vertices = c.vertices;
    std::sort(vertices.begin(), vertices.end(),
              [](const Vertex &a, const Vertex &b) { return a.index < b.index; });

    // Build the poly-line through every vertex, and where an endpoint docks to a
    // room, run an outward "stalk" between the room outline and the endpoint so
    // the line leaves the room perpendicular to its edge (matching the C#
    // Connection.getSegments stalk handling).
    const double stalk = map->settings.connectionStalkLength;
    const int n = vertices.size();
    for (int i = 0; i < n; ++i) {
        const Vertex &v = vertices.at(i);
        const Room *room = v.docked ? map->roomById(v.roomId) : nullptr;
        const QPointF base = (room != nullptr) ? MapScene::portPoint(*room, v.port) : v.point;

        QPointF sp;
        bool haveStalk = false;
        if (room != nullptr && n >= 2 && stalk > 0.0) {
            sp = MapScene::portStalkPoint(*room, v.port, stalk);
            haveStalk = (sp != base);
        }

        if (i == n - 1 && haveStalk) // last endpoint: the stalk precedes it
            m_points.append(sp);
        m_points.append(base);
        if (i == 0 && haveStalk)     // first endpoint: the stalk follows it
            m_points.append(sp);
    }
    update();
}

QRectF ConnectionItem::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();
    QRectF r(m_points.first(), m_points.first());
    for (const QPointF &p : m_points)
        r = r.united(QRectF(p, p));
    return r.adjusted(-12, -12, 12, 12);
}

QPainterPath ConnectionItem::shape() const
{
    QPainterPath path;
    if (m_points.size() < 2)
        return path;
    path.moveTo(m_points.first());
    for (int i = 1; i < m_points.size(); ++i)
        path.lineTo(m_points.at(i));
    QPainterPathStroker stroker;
    stroker.setWidth(8);
    return stroker.createStroke(path);
}

void ConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_points.size() < 2)
        return;
    const Map *map = m_scene->document();
    const int idx = map ? map->connectionIndex(m_connId) : -1;
    if (idx < 0)
        return;
    const Connection &c = map->connections.at(idx);

    QColor color = c.color.isValid() ? c.color : map->lineColor();
    if (!color.isValid())
        color = QColor(Qt::darkBlue);

    QPen pen(color);
    pen.setWidthF(2.0);
    if (c.style == ConnectionStyle::Dashed)
        pen.setStyle(Qt::DashLine);
    if (isSelected()) {
        pen.setColor(QColor(30, 120, 220));
        pen.setWidthF(3.0);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QPainterPath path(m_points.first());
    for (int i = 1; i < m_points.size(); ++i)
        path.lineTo(m_points.at(i));
    painter->drawPath(path);

    // One-way chevrons: a ">"-shaped arrowhead at the midpoint of every segment
    // long enough to hold one, pointing along the flow direction. This mirrors
    // the C# Connection.Draw, which draws a chevron per segment sized from
    // Settings.ConnectionArrowSize, rather than a single arrow at the end.
    if (c.flow == ConnectionFlow::OneWay) {
        const double arrow = map->settings.arrowSize > 0.0 ? map->settings.arrowSize : 12.0;
        QPen arrowPen(color);
        arrowPen.setWidthF(pen.widthF());
        painter->setPen(arrowPen);
        painter->setBrush(Qt::NoBrush);
        for (int i = 1; i < m_points.size(); ++i) {
            const QPointF a = m_points.at(i - 1);
            const QPointF b = m_points.at(i);
            const QPointF d = b - a;
            const double len = std::hypot(d.x(), d.y());
            if (len <= arrow)
                continue;
            const QPointF u = d / len;
            const QPointF nrm(-u.y(), u.x());
            const QPointF mid = (a + b) / 2.0;
            const double s = arrow / 2.0;
            const QPointF apex = mid + u * s;
            const QPointF right = mid - u * s + nrm * s;
            const QPointF left = mid - u * s - nrm * s;
            QPolygonF chevron;
            chevron << apex << right << left;
            painter->drawPolygon(chevron);
        }
    }

    // A small square at the midpoint marks a door.
    if (c.hasDoor) {
        const QPointF mid = (m_points.first() + m_points.last()) / 2.0;
        painter->setPen(QPen(color, 1.5));
        painter->setBrush(c.door.open ? QBrush(Qt::white) : QBrush(color));
        painter->drawRect(QRectF(mid.x() - 4, mid.y() - 4, 8, 8));
    }
}

void ConnectionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_scene->activateConnection(m_connId);
    event->accept();
}

} // namespace trizbort
