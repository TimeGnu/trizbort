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
#include <QFontMetricsF>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QRegularExpression>

#include "FontUtil.h"
#include "MapScene.h"

namespace trizbort {

namespace {

// The text colour a region contributes for room names (invalid if unknown).
QColor regionTextColor(const Map &map, const QString &region)
{
    if (const Region *rg = map.regionByName(region))
        return rg->textColor;
    return QColor();
}

// Draw a room's name, subtitle and object list, matching the C# Room.Draw
// text layout: the name is centred in the room using the room font and region
// (or room-specific) text colour; the subtitle sits just below it; the object
// list is anchored at one of the compass positions outside the room (or inside,
// below the name, for the in-between positions), with its [..] property tags
// stripped. Reference rooms show "To" over the referenced room's name.
void drawRoomText(QPainter *painter, const Map &map, const Room &room, const QRectF &rect)
{
    // --- Name + subtitle ---
    QRectF textBounds = room.ellipse ? rect.adjusted(11.5, 11.5, -11.5, -11.5)
                                     : rect.adjusted(5, 5, -5, -5);

    QString nameText = room.name;
    QString subtitleText = room.subtitle;
    if (room.referenceRoom >= 0) {
        const Room *ref = map.roomById(room.referenceRoom);
        nameText = QObject::tr("To");
        subtitleText = ref ? ref->name : QString();
    }

    if (textBounds.width() > 0 && textBounds.height() > 0 && !nameText.isEmpty()) {
        QColor nameColor = room.largeText.isValid() ? room.largeText
                                                    : regionTextColor(map, room.region);
        if (!nameColor.isValid())
            nameColor = QColor(Qt::black);
        const QFont nameFont = qfontFromSpec(map.settings.roomFont, 13.0);
        painter->setFont(nameFont);
        painter->setPen(nameColor);
        const QRectF nameBox = painter->boundingRect(
            textBounds, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, nameText);
        painter->drawText(textBounds, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
                          nameText);

        if (!subtitleText.isEmpty()) {
            QColor subColor = room.subtitleColor.isValid()
                                  ? room.subtitleColor
                                  : map.settings.colors[ColorSubtitle];
            if (!subColor.isValid())
                subColor = QColor(90, 90, 90);
            const QFont subFont = qfontFromSpec(map.settings.subtitleFont, 9.0);
            painter->setFont(subFont);
            painter->setPen(subColor);
            const QRectF subRect(nameBox.left(), nameBox.bottom(), nameBox.width(),
                                 textBounds.bottom() - nameBox.bottom());
            if (subRect.height() > 0)
                painter->drawText(subRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,
                                  subtitleText);
        }
    }

    // --- Object list ---
    if (room.objectsText.isEmpty())
        return;
    QString objs = room.objectsText;
    objs.remove(QRegularExpression(QStringLiteral("\\[[^\\]\\[]*\\]")));
    objs = objs.trimmed();
    if (objs.isEmpty())
        return;

    const double off = map.settings.objectListOffset;
    QColor objColor = room.smallText.isValid() ? room.smallText
                                               : map.settings.colors[ColorSmallText];
    if (!objColor.isValid())
        objColor = QColor(60, 60, 60);
    const QFont objFont = qfontFromSpec(map.settings.objectFont, 9.0);
    painter->setFont(objFont);
    painter->setPen(objColor);
    const QFontMetricsF ofm(objFont);
    const QRectF ob =
        ofm.boundingRect(QRectF(0, 0, 4000, 4000), Qt::AlignLeft | Qt::TextWordWrap, objs);
    const double ow = ob.width();
    const double oh = ob.height();

    const QString tok = room.objectsPosition.at.isEmpty()
                            ? QStringLiteral("s")
                            : room.objectsPosition.at.toLower();
    const double dx = room.objectsPosition.custom ? room.objectsPosition.customRight : 0;
    const double dy = room.objectsPosition.custom ? room.objectsPosition.customDown : 0;

    // The "in-between" compass positions draw the list inside the room, below
    // the name (matching SetAlignmentFromCardinalOrOrdinalDirection returning
    // false for those directions).
    const bool inside = (tok == QLatin1String("nne") || tok == QLatin1String("ene") ||
                         tok == QLatin1String("ese") || tok == QLatin1String("sse") ||
                         tok == QLatin1String("ssw") || tok == QLatin1String("wsw") ||
                         tok == QLatin1String("wnw"));

    if (inside) {
        const double height = rect.height() / 2.0 - ofm.height() / 2.0;
        QRectF b(rect.left() + off, rect.bottom() - height, rect.width() - off, height - off);
        b.translate(dx, dy);
        if (b.width() > 0 && b.height() > 0)
            painter->drawText(b, Qt::AlignLeft | Qt::AlignBottom | Qt::TextWordWrap, objs);
        return;
    }

    // Otherwise anchor the list at a compass corner of the room inflated by the
    // object-list offset, growing away from the room.
    QPointF pos = MapScene::squareCorner(rect.left() - off, rect.top() - off,
                                         rect.width() + 2 * off, rect.height() + 2 * off, tok);
    if (tok == QLatin1String("n") || tok == QLatin1String("s"))
        pos.rx() += off;
    pos += QPointF(dx, dy);

    // Horizontal: grow right except for the west-ish anchors, which grow left.
    const bool growLeft = (tok == QLatin1String("w") || tok == QLatin1String("sw") ||
                           tok == QLatin1String("nw") || tok == QLatin1String("nnw"));
    // Vertical: grow up for the north-ish anchors, down otherwise.
    const bool growUp = (tok == QLatin1String("n") || tok == QLatin1String("ne") ||
                         tok == QLatin1String("nw") || tok == QLatin1String("nnw"));
    const double left = growLeft ? pos.x() - ow : pos.x();
    const double top = growUp ? pos.y() - oh : pos.y();
    painter->drawText(QRectF(left, top, ow, oh),
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextDontClip | Qt::TextWordWrap, objs);
}

} // namespace

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
    // Padded generously so subtitle text and the object list drawn around the
    // room (in any compass direction) fall inside the item's update region.
    const double margin = 90.0;
    return QRectF(-margin, -margin, m_w + 2 * margin, m_h + 2 * margin);
}

QPainterPath RoomItem::shape() const
{
    QPainterPath p;
    p.addRect(QRectF(0, 0, m_w, m_h));
    return p;
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

    drawRoomText(painter, *map, *room, rect);

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
