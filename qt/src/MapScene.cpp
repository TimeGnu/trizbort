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

#include "MapScene.h"

#include <QBrush>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPainterPath>
#include <QPen>
#include <QPolygonF>
#include <QtGlobal>

#include <algorithm>
#include <cmath>

namespace trizbort {

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

QPointF MapScene::portPoint(const Room &room, const QString &port) const
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
    // Extended 16-point compass values fall back to the room center for now.
    return {cx, cy};
}

void MapScene::addArrowHead(const QPointF &from, const QPointF &to, const QColor &color)
{
    const QPointF d = to - from;
    const double len = std::hypot(d.x(), d.y());
    if (len < 1e-6)
        return;
    const QPointF u = d / len;             // unit direction
    const QPointF normal(-u.y(), u.x());   // perpendicular
    const double length = 11.0;
    const double halfWidth = 5.0;
    const QPointF base = to - u * length;
    QPolygonF head;
    head << to << (base + normal * halfWidth) << (base - normal * halfWidth);
    addPolygon(head, QPen(color), QBrush(color));
}

void MapScene::addLabel(const QString &text, const QPointF &pos, const QColor &color)
{
    if (text.isEmpty())
        return;
    QGraphicsSimpleTextItem *item = addSimpleText(text);
    item->setBrush(color.isValid() ? color : QColor(Qt::darkGray));
    const QRectF b = item->boundingRect();
    item->setPos(pos.x() - b.width() / 2.0, pos.y() - b.height() / 2.0);
    item->setZValue(2);
}

void MapScene::addRoomLabel(const QString &text, const QRectF &rect, const QColor &color)
{
    if (text.isEmpty())
        return;
    QGraphicsSimpleTextItem *item = addSimpleText(text);
    item->setBrush(color.isValid() ? color : QColor(Qt::black));

    const QRectF b = item->boundingRect();
    if (b.width() <= 0.0 || b.height() <= 0.0)
        return;

    const double padding = 6.0;
    const double availWidth = qMax(1.0, rect.width() - padding);
    const double availHeight = qMax(1.0, rect.height() - padding);

    double scale = 1.0;
    if (b.width() > availWidth)
        scale = qMin(scale, availWidth / b.width());
    if (b.height() * scale > availHeight)
        scale = qMin(scale, availHeight / b.height());
    if (scale < 1.0)
        item->setScale(scale);

    const double drawnWidth = b.width() * scale;
    const double drawnHeight = b.height() * scale;
    item->setPos(rect.center().x() - drawnWidth / 2.0,
                 rect.center().y() - drawnHeight / 2.0);
    item->setZValue(1);
}

void MapScene::setMap(const Map &map)
{
    clear();
    setBackgroundBrush(map.canvasColor.isValid() ? map.canvasColor : QColor(Qt::white));

    const QColor defaultLine = map.lineColor.isValid() ? map.lineColor : QColor(Qt::darkBlue);
    const QColor defaultBorder = map.borderColor.isValid() ? map.borderColor : QColor(Qt::black);

    // Connections first, so rooms paint over the line ends.
    for (const Connection &c : map.connections) {
        QList<Vertex> vertices = c.vertices;
        std::sort(vertices.begin(), vertices.end(),
                  [](const Vertex &a, const Vertex &b) { return a.index < b.index; });

        QVector<QPointF> points;
        for (const Vertex &v : vertices) {
            if (v.docked) {
                if (const Room *room = map.roomById(v.roomId))
                    points.append(portPoint(*room, v.port));
            } else {
                points.append(v.point);
            }
        }
        if (points.size() < 2)
            continue; // orphaned/degenerate connection

        QPainterPath path(points.first());
        for (int i = 1; i < points.size(); ++i)
            path.lineTo(points.at(i));

        const QColor lineColor = c.color.isValid() ? c.color : defaultLine;
        QPen pen(lineColor);
        pen.setWidthF(2.0);
        if (c.style == ConnectionStyle::Dashed)
            pen.setStyle(Qt::DashLine);
        QGraphicsPathItem *item = addPath(path, pen);
        item->setZValue(-1);

        if (c.flow == ConnectionFlow::OneWay)
            addArrowHead(points.at(points.size() - 2), points.last(), lineColor);

        addLabel(c.startText, points.first(), defaultLine);
        addLabel(c.endText, points.last(), defaultLine);
        if (points.size() >= 2) {
            const QPointF mid = (points.first() + points.last()) / 2.0;
            addLabel(c.midText, mid, defaultLine);
        }
    }

    // Rooms.
    for (const Room &r : map.rooms) {
        const QRectF rect(r.x, r.y, r.w, r.h);

        QColor fill = r.fill.isValid() ? r.fill : map.regionFill(r.region);
        if (!fill.isValid())
            fill = QColor(Qt::white);
        const QColor border = r.border.isValid() ? r.border : defaultBorder;

        QPen pen(border);
        pen.setWidthF(2.0);
        if (r.borderStyle == QLatin1String("None"))
            pen.setStyle(Qt::NoPen);
        else if (r.borderStyle == QLatin1String("Dashed"))
            pen.setStyle(Qt::DashLine);

        QGraphicsRectItem *rectItem = addRect(rect, pen, QBrush(fill));
        rectItem->setZValue(0);

        if (!r.name.isEmpty()) {
            const QColor textColor = r.largeText.isValid() ? r.largeText : QColor(Qt::black);
            addRoomLabel(r.name, rect, textColor);
        }
    }
}

} // namespace trizbort
