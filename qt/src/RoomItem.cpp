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

#include "RoomItem.h"

#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

#include "MapScene.h"

namespace trizbort {

RoomItem::RoomItem(MapScene *scene, int roomId)
    : m_scene(scene)
    , m_roomId(roomId)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setZValue(0);
    syncFromModel();
}

void RoomItem::syncFromModel()
{
    const Map *map = m_scene->document();
    const Room *room = map ? map->roomById(m_roomId) : nullptr;
    if (!room)
        return;
    m_applyingModel = true;
    m_w = room->w;
    m_h = room->h;
    prepareGeometryChange();
    setPos(room->x, room->y);
    m_applyingModel = false;
    update();
}

QRectF RoomItem::boundingRect() const
{
    const double margin = 3.0;
    return QRectF(-margin, -margin, m_w + 2 * margin, m_h + 2 * margin);
}

void RoomItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    const Map *map = m_scene->document();
    const Room *room = map ? map->roomById(m_roomId) : nullptr;
    if (!room)
        return;

    const QRectF rect(0, 0, m_w, m_h);

    QColor fill = room->fill.isValid() ? room->fill : map->regionFill(room->region);
    if (!fill.isValid())
        fill = QColor(Qt::white);
    QColor border = room->border.isValid() ? room->border : map->borderColor();
    if (!border.isValid())
        border = QColor(Qt::black);

    QPen pen(border);
    pen.setWidthF(2.0);
    if (room->borderStyle == QLatin1String("None"))
        pen.setStyle(Qt::NoPen);
    else if (room->borderStyle.startsWith(QLatin1String("Dash")))
        pen.setStyle(Qt::DashLine);
    else if (room->borderStyle == QLatin1String("Dot"))
        pen.setStyle(Qt::DotLine);

    // Build the outline for the room's shape.
    QPainterPath path;
    if (room->ellipse) {
        path.addEllipse(rect);
    } else if (room->octagonal) {
        const double c = qMin(m_w, m_h) * 0.25;
        QPolygonF poly;
        poly << QPointF(c, 0) << QPointF(m_w - c, 0) << QPointF(m_w, c)
             << QPointF(m_w, m_h - c) << QPointF(m_w - c, m_h) << QPointF(c, m_h)
             << QPointF(0, m_h - c) << QPointF(0, c);
        path.addPolygon(poly);
        path.closeSubpath();
    } else if (room->roundedCorners) {
        path.addRoundedRect(rect, 8, 8);
    } else {
        path.addRect(rect);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->setBrush(fill);
    painter->drawPath(path);

    // Dark rooms get a diagonal corner stripe, as in Trizbort.
    if (room->isDark) {
        painter->save();
        painter->setClipPath(path);
        const double s = qMin(m_w, m_h) * 0.35;
        QPolygonF tri;
        tri << QPointF(m_w - s, 0) << QPointF(m_w, 0) << QPointF(m_w, s);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 0, 0, 60));
        painter->drawPolygon(tri);
        painter->restore();
    }

    // Start/end room markers.
    if (room->isStartRoom || room->isEndRoom) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(room->isStartRoom ? map->settings.colors[ColorStartRoom]
                                            : map->settings.colors[ColorEndRoom]);
        painter->drawEllipse(QPointF(6, 6), 3, 3);
    }

    // Room name, scaled to fit.
    if (!room->name.isEmpty()) {
        QColor textColor = room->largeText.isValid() ? room->largeText : QColor(Qt::black);
        painter->setPen(textColor);
        QFont font = painter->font();
        font.setPointSizeF(11);
        painter->setFont(font);
        QRectF textRect = rect.adjusted(3, 3, -3, -3);
        painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, room->name);
    }

    // Selection highlight.
    if (isSelected()) {
        QPen sel(QColor(30, 120, 220));
        sel.setWidthF(1.5);
        sel.setStyle(Qt::DashLine);
        painter->setPen(sel);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect.adjusted(-2, -2, 2, 2));
    }
}

QVariant RoomItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene() && !m_applyingModel) {
        // Snap the proposed top-left to the grid.
        return m_scene->snap(value.toPointF());
    }
    if (change == ItemPositionHasChanged && !m_applyingModel) {
        m_scene->roomMovedTo(m_roomId, pos());
    }
    return QGraphicsItem::itemChange(change, value);
}

void RoomItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_scene->activateRoom(m_roomId);
    event->accept();
}

} // namespace trizbort
