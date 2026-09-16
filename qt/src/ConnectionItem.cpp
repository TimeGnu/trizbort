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

#include <QFontMetricsF>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPolygonF>
#include <QUndoStack>

#include "EditCommands.h"
#include "FontUtil.h"
#include "MapScene.h"

namespace trizbort {

namespace {

// The point half way along a poly-line by arc length.
QPointF midpointAlongPath(const QList<QPointF> &pts)
{
    if (pts.size() < 2)
        return pts.isEmpty() ? QPointF() : pts.first();
    double total = 0.0;
    for (int i = 1; i < pts.size(); ++i)
        total += std::hypot(pts[i].x() - pts[i - 1].x(), pts[i].y() - pts[i - 1].y());
    double half = total / 2.0;
    for (int i = 1; i < pts.size(); ++i) {
        const QPointF d = pts[i] - pts[i - 1];
        const double len = std::hypot(d.x(), d.y());
        if (half <= len && len > 0.0)
            return pts[i - 1] + d * (half / len);
        half -= len;
    }
    return pts.last();
}

// Draw one connection annotation (start/mid/end text) offset from an anchor on
// the line, on the side the line points away to. Mirrors Connection.annotate:
// the label sits clear of the line, growing away from the anchor.
void drawConnectionLabel(QPainter *painter, const QPointF &anchor, const QPointF &outward,
                         const QString &text, const QFont &font, const QColor &color,
                         double offset)
{
    if (text.isEmpty())
        return;
    painter->setFont(font);
    painter->setPen(color);
    const QFontMetricsF fm(font);
    const QRectF box = fm.boundingRect(QRectF(0, 0, 1000, 1000),
                                       Qt::AlignLeft | Qt::TextWordWrap, text);
    const double w = box.width();
    const double h = box.height();
    const QPointF at = anchor + outward * offset;
    double left, top;
    if (outward.x() > 0.3)
        left = at.x();
    else if (outward.x() < -0.3)
        left = at.x() - w;
    else
        left = at.x() - w / 2.0;
    if (outward.y() > 0.3)
        top = at.y();
    else if (outward.y() < -0.3)
        top = at.y() - h;
    else
        top = at.y() - h / 2.0;
    painter->drawText(QRectF(left, top, w, h),
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextDontClip, text);
}

// A small upright door glyph centred on pos: a solid leaf when closed, an open
// (outline) leaf when open.
void drawDoorGlyph(QPainter *painter, const QPointF &pos, const QColor &color, bool open)
{
    const QRectF leaf(pos.x() - 5, pos.y() - 7, 10, 14);
    painter->setPen(QPen(color, 1.5));
    painter->setBrush(open ? QBrush(Qt::white) : QBrush(color));
    painter->drawRect(leaf);
    // Door knob.
    painter->setPen(QPen(open ? color : QColor(Qt::white), 1.0));
    painter->setBrush(Qt::NoBrush);
    painter->drawPoint(QPointF(leaf.right() - 2.5, pos.y()));
}

// A small padlock glyph centred on pos: closed shackle when locked, an open
// (offset) shackle when unlocked.
void drawLockGlyph(QPainter *painter, const QPointF &pos, const QColor &color, bool locked)
{
    const QRectF body(pos.x() - 4, pos.y() - 1, 8, 7);
    painter->setPen(QPen(color, 1.2));
    painter->setBrush(QBrush(color));
    painter->drawRect(body);
    painter->setBrush(Qt::NoBrush);
    if (locked) {
        // Closed shackle sitting on top of the body.
        painter->drawArc(QRectF(pos.x() - 3, pos.y() - 6, 6, 8), 0 * 16, 180 * 16);
    } else {
        // Open shackle, pivoted up to one side.
        painter->drawArc(QRectF(pos.x() - 1, pos.y() - 7, 6, 8), 30 * 16, 160 * 16);
    }
}

} // namespace

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

    // Hover tooltip: name, a door marker, and description.
    QString tip = c.name.isEmpty() ? QObject::tr("Connection") : c.name;
    if (c.hasDoor)
        tip += QObject::tr(" (Door)");
    if (!c.description.trimmed().isEmpty())
        tip += QLatin1Char('\n') + c.description.trimmed();
    setToolTip(tip);
    update();
}

QRectF ConnectionItem::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();
    QRectF r(m_points.first(), m_points.first());
    for (const QPointF &p : m_points)
        r = r.united(QRectF(p, p));
    // Margin covers the line width, arrowheads, door/lock glyphs and short
    // end/mid labels drawn just off the line.
    return r.adjusted(-40, -40, 40, 40);
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

    // "Smart" line segments: draw a small gap wherever this line crosses another
    // connection, so crossings read clearly. To avoid gapping both lines of a
    // pair, only the higher-id connection gaps (against lower-id ones).
    const QVector<QPair<QLineF, int>> others = m_scene->connectionSegmentsExcept(m_connId);
    const double gap = 4.0;
    for (int i = 1; i < m_points.size(); ++i) {
        const QPointF a = m_points.at(i - 1);
        const QPointF b = m_points.at(i);
        const QLineF seg(a, b);
        const double len = seg.length();
        QVector<double> ts;
        if (len > 1e-6) {
            for (const QPair<QLineF, int> &o : others) {
                if (o.second >= m_connId)
                    continue; // the other (lower id) will gap instead
                QPointF ip;
                if (seg.intersects(o.first, &ip) == QLineF::BoundedIntersection) {
                    const double t = QLineF(a, ip).length() / len;
                    if (t > 0.03 && t < 0.97) // ignore near shared endpoints
                        ts.append(t);
                }
            }
        }
        if (ts.isEmpty()) {
            painter->drawLine(a, b);
            continue;
        }
        std::sort(ts.begin(), ts.end());
        const double gt = gap / len;
        double cursor = 0.0;
        for (double t : ts) {
            const double stop = std::max(cursor, t - gt);
            if (stop > cursor)
                painter->drawLine(a + (b - a) * cursor, a + (b - a) * stop);
            cursor = std::min(1.0, t + gt);
        }
        if (cursor < 1.0)
            painter->drawLine(a + (b - a) * cursor, b);
    }

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

    // A door is shown as a door-leaf glyph plus a padlock reflecting the
    // open/locked state, at the middle of the connection (matching the C#
    // door + lock icon pair).
    if (c.hasDoor) {
        const QPointF mid = midpointAlongPath(m_points);
        drawDoorGlyph(painter, mid - QPointF(7, 0), color, c.door.open);
        drawLockGlyph(painter, mid + QPointF(7, 0), color, c.door.locked);
    }

    // Start / mid / end text labels, drawn in the line font and line-text colour.
    if (m_scene->textVisible() &&
        (!c.startText.isEmpty() || !c.midText.isEmpty() || !c.endText.isEmpty())) {
        const QFont lineFont = qfontFromSpec(map->settings.lineFont, 9.0);
        QColor textColor = map->settings.colors[ColorLineText];
        if (!textColor.isValid())
            textColor = QColor(Qt::black);
        const double off = map->settings.textOffset > 0.0 ? map->settings.textOffset : 4.0;

        if (!c.startText.isEmpty()) {
            const QPointF a = m_points.first();
            const QPointF b = m_points.at(1);
            QPointF out = a - b;
            const double l = std::hypot(out.x(), out.y());
            out = (l > 1e-6) ? out / l : QPointF(0, -1);
            drawConnectionLabel(painter, a, out, c.startText, lineFont, textColor, off);
        }
        if (!c.endText.isEmpty()) {
            const QPointF a = m_points.last();
            const QPointF b = m_points.at(m_points.size() - 2);
            QPointF out = a - b;
            const double l = std::hypot(out.x(), out.y());
            out = (l > 1e-6) ? out / l : QPointF(0, -1);
            drawConnectionLabel(painter, a, out, c.endText, lineFont, textColor, off);
        }
        if (!c.midText.isEmpty()) {
            const QPointF mid = midpointAlongPath(m_points);
            drawConnectionLabel(painter, mid, QPointF(0, 1), c.midText, lineFont, textColor, off);
        }
    }

    // Editing handles at each vertex when selected: a solid dot for a docked
    // endpoint, a hollow dot for a free waypoint.
    if (isSelected()) {
        const QVector<QPointF> handles = vertexHandlePoints();
        QList<Vertex> sorted = c.vertices;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
        for (int i = 0; i < handles.size(); ++i) {
            const bool docked = (i < sorted.size()) ? sorted.at(i).docked : true;
            painter->setPen(QPen(QColor(30, 120, 220), 1.5));
            painter->setBrush(docked ? QBrush(QColor(30, 120, 220)) : QBrush(Qt::white));
            painter->drawEllipse(handles.at(i), 4.0, 4.0);
        }
    }
}

QVector<QPointF> ConnectionItem::vertexHandlePoints() const
{
    QVector<QPointF> pts;
    const Map *map = m_scene->document();
    if (!map)
        return pts;
    const int idx = map->connectionIndex(m_connId);
    if (idx < 0)
        return pts;
    QList<Vertex> vertices = map->connections.at(idx).vertices;
    std::sort(vertices.begin(), vertices.end(),
              [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
    for (const Vertex &v : vertices) {
        if (v.docked) {
            if (const Room *room = map->roomById(v.roomId))
                pts.append(MapScene::portPoint(*room, v.port));
            else
                pts.append(v.point);
        } else {
            pts.append(v.point);
        }
    }
    return pts;
}

int ConnectionItem::vertexHandleAt(const QPointF &localPos) const
{
    const QVector<QPointF> pts = vertexHandlePoints();
    for (int i = 0; i < pts.size(); ++i) {
        const QPointF d = localPos - pts.at(i);
        if (std::hypot(d.x(), d.y()) <= 7.0)
            return i;
    }
    return -1;
}

void ConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isSelected() && event->button() == Qt::LeftButton) {
        const int v = vertexHandleAt(event->pos());
        if (v >= 0) {
            Map *map = m_scene->document();
            const int idx = map ? map->connectionIndex(m_connId) : -1;
            if (idx >= 0) {
                Connection &c = map->connections[idx];
                std::sort(c.vertices.begin(), c.vertices.end(),
                          [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
                for (int i = 0; i < c.vertices.size(); ++i)
                    c.vertices[i].index = i;
                m_dragVertex = v;
                m_dragBefore = c;
                event->accept();
                return;
            }
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void ConnectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragVertex >= 0) {
        Map *map = m_scene->document();
        const int idx = map ? map->connectionIndex(m_connId) : -1;
        if (idx >= 0 && m_dragVertex < map->connections.at(idx).vertices.size()) {
            Vertex &v = map->connections[idx].vertices[m_dragVertex];
            v.docked = false; // free while dragging; may re-dock on release
            v.point = event->scenePos();
            updateRoute();
        }
        event->accept();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ConnectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragVertex >= 0) {
        const int dragged = m_dragVertex;
        m_dragVertex = -1;
        Map *map = m_scene->document();
        const int idx = map ? map->connectionIndex(m_connId) : -1;
        if (idx >= 0 && dragged < map->connections.at(idx).vertices.size()) {
            Connection &c = map->connections[idx];
            Vertex &v = c.vertices[dragged];
            const bool isEndpoint = (dragged == 0 || dragged == c.vertices.size() - 1);
            const int roomId = m_scene->roomIdAt(event->scenePos());
            if (isEndpoint && roomId >= 0) {
                // Re-dock the endpoint onto the room's port facing the next vertex.
                const int otherIdx = (dragged == 0) ? 1 : dragged - 1;
                QPointF towards = event->scenePos();
                const QVector<QPointF> pts = vertexHandlePoints();
                if (otherIdx >= 0 && otherIdx < pts.size())
                    towards = pts.at(otherIdx);
                if (const Room *r = map->roomById(roomId)) {
                    v.docked = true;
                    v.roomId = roomId;
                    v.port = MapScene::portTowards(*r, towards);
                }
            } else {
                v.docked = false;
                v.point = event->scenePos();
            }
            const Connection after = c;
            if (m_scene->undoStack())
                m_scene->undoStack()->push(
                    new EditConnectionCommand(m_scene, m_connId, m_dragBefore, after));
            updateRoute();
        }
        event->accept();
        return;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void ConnectionItem::addWaypointAt(const QPointF &scenePos)
{
    Map *map = m_scene->document();
    const int idx = map ? map->connectionIndex(m_connId) : -1;
    if (idx < 0)
        return;
    Connection before = map->connections.at(idx);
    Connection &c = map->connections[idx];
    std::sort(c.vertices.begin(), c.vertices.end(),
              [](const Vertex &a, const Vertex &b) { return a.index < b.index; });

    // Find the segment closest to the click and insert the new vertex into it.
    const QVector<QPointF> pts = vertexHandlePoints();
    int insertAfter = 0;
    double best = 1e18;
    for (int i = 0; i + 1 < pts.size(); ++i) {
        const QPointF a = pts.at(i);
        const QPointF b = pts.at(i + 1);
        const QPointF ab = b - a;
        const double len2 = ab.x() * ab.x() + ab.y() * ab.y();
        double t = 0.0;
        if (len2 > 1e-9)
            t = std::clamp(QPointF::dotProduct(scenePos - a, ab) / len2, 0.0, 1.0);
        const QPointF proj = a + ab * t;
        const QPointF d = scenePos - proj;
        const double dist = std::hypot(d.x(), d.y());
        if (dist < best) {
            best = dist;
            insertAfter = i;
        }
    }
    Vertex nv;
    nv.docked = false;
    nv.point = scenePos;
    c.vertices.insert(insertAfter + 1, nv);
    for (int i = 0; i < c.vertices.size(); ++i)
        c.vertices[i].index = i;
    const Connection after = c;
    if (m_scene->undoStack())
        m_scene->undoStack()->push(new EditConnectionCommand(m_scene, m_connId, before, after));
    updateRoute();
}

void ConnectionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_scene->activateConnection(m_connId);
    event->accept();
}

void ConnectionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!isSelected()) {
        if (scene())
            scene()->clearSelection();
        setSelected(true);
    }
    QMenu menu;
    QAction *editAct = menu.addAction(QObject::tr("Edit…"));
    QAction *waypointAct = menu.addAction(QObject::tr("Add Waypoint Here"));
    QAction *deleteAct = menu.addAction(QObject::tr("Delete"));
    QAction *chosen = menu.exec(event->screenPos());
    if (chosen == editAct)
        m_scene->activateConnection(m_connId);
    else if (chosen == waypointAct)
        addWaypointAt(event->scenePos());
    else if (chosen == deleteAct)
        m_scene->deleteSelection();
    event->accept();
}

} // namespace trizbort
