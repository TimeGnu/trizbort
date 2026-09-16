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

#include <algorithm>
#include <cmath>
#include <random>

#include <QCursor>
#include <QFont>
#include <QFontMetricsF>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QHash>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QRegularExpression>
#include <QUndoStack>

#include "AppSettings.h"
#include "EditCommands.h"
#include "FontUtil.h"
#include "MapScene.h"

namespace trizbort {

namespace {

// Append a hand-drawn (jittered) edge from a to b to a path whose current point
// is already a, matching Drawing.AddLine: the edge is split into a handful of
// sub-segments whose interior points wander by +/-1px, giving the sketchy look.
void addHandDrawnEdge(QPainterPath &path, const QPointF &a, const QPointF &b, std::mt19937 &rng)
{
    auto nextInt = [&rng](int lo, int hiExclusive) {
        if (hiExclusive <= lo)
            return lo;
        return std::uniform_int_distribution<int>(lo, hiExclusive - 1)(rng);
    };
    const double dx = b.x() - a.x();
    const double dy = b.y() - a.y();
    const double dist = std::hypot(dx, dy);
    int lo = std::max(3, static_cast<int>(dist / 15.0));
    int hi = std::max(6, static_cast<int>(dist / 8.0));
    if (hi <= lo)
        hi = lo + 1;
    const int lines = nextInt(lo, hi) - 1;
    for (int line = 0; line < lines; ++line) {
        QPointF next;
        if (line == 0)
            next = a;
        else if (line == lines - 1)
            next = b;
        else {
            const double f = static_cast<double>(line) / (lines - 1);
            next = QPointF(a.x() + dx * f + nextInt(-1, 2), a.y() + dy * f + nextInt(-1, 2));
        }
        path.lineTo(next);
    }
}

// A per-room deterministic RNG, seeded from the room name so the sketch is
// stable across repaints (as the C# renderer seeds Random(Name.GetHashCode)).
std::mt19937 handDrawnRng(const Room &room)
{
    return std::mt19937(static_cast<unsigned>(qHash(room.name)) ^ 0x9e3779b9u);
}

// Local-coordinate centre of resize handle i (0=N,1=NE,2=E,3=SE,4=S,5=SW,6=W,
// 7=NW) on a room of the given size.
QPointF handlePoint(int i, double w, double h)
{
    switch (i) {
    case 0: return {w / 2.0, 0};
    case 1: return {w, 0};
    case 2: return {w, h / 2.0};
    case 3: return {w, h};
    case 4: return {w / 2.0, h};
    case 5: return {0, h};
    case 6: return {0, h / 2.0};
    case 7: return {0, 0};
    default: return {0, 0};
    }
}

// The text colour a region contributes for room names (invalid if unknown).
QColor regionTextColor(const Map &map, const QString &region)
{
    if (const Region *rg = map.regionByName(region))
        return rg->textColor;
    return QColor();
}

// The Qt pen style for a stored border token, or Qt::NoPen for "None".
Qt::PenStyle penStyleForBorder(const QString &style)
{
    if (style == QLatin1String("None"))
        return Qt::NoPen;
    if (style == QLatin1String("Dot"))
        return Qt::DotLine;
    if (style == QLatin1String("Dash") || style == QLatin1String("Dashed") ||
        style == QLatin1String("Custom"))
        return Qt::DashLine;
    if (style == QLatin1String("DashDot"))
        return Qt::DashDotLine;
    if (style == QLatin1String("DashDotDot"))
        return Qt::DashDotDotLine;
    return Qt::SolidLine;
}

// Build the outline path for a room's shape within the given rectangle: ellipse,
// octagon (independent per-axis quarter chamfers), rounded (per-corner radii) or
// plain rectangle. Mirrors Room.createRoomPath / the octagon quarter-points.
QPainterPath buildRoomPath(const Room &room, const QRectF &r)
{
    QPainterPath path;
    if (room.ellipse) {
        path.addEllipse(r);
        return path;
    }
    if (room.octagonal) {
        const double cx = r.width() * 0.25;
        const double cy = r.height() * 0.25;
        QVector<QPointF> v{
            QPointF(r.left() + cx, r.top()),    QPointF(r.right() - cx, r.top()),
            QPointF(r.right(), r.top() + cy),   QPointF(r.right(), r.bottom() - cy),
            QPointF(r.right() - cx, r.bottom()), QPointF(r.left() + cx, r.bottom()),
            QPointF(r.left(), r.bottom() - cy), QPointF(r.left(), r.top() + cy)};
        if (room.handDrawn) {
            std::mt19937 rng = handDrawnRng(room);
            path.moveTo(v.first());
            for (int i = 0; i < v.size(); ++i)
                addHandDrawnEdge(path, v.at(i), v.at((i + 1) % v.size()), rng);
        } else {
            path.addPolygon(QPolygonF(v));
        }
        path.closeSubpath();
        return path;
    }
    if (room.roundedCorners) {
        const double maxR = qMin(r.width(), r.height()) / 2.0;
        const double tl = qBound(0.0, room.cornerTopLeft, maxR);
        const double tr = qBound(0.0, room.cornerTopRight, maxR);
        const double br = qBound(0.0, room.cornerBottomRight, maxR);
        const double bl = qBound(0.0, room.cornerBottomLeft, maxR);
        path.moveTo(r.left() + tl, r.top());
        path.lineTo(r.right() - tr, r.top());
        if (tr > 0.0)
            path.arcTo(r.right() - 2 * tr, r.top(), 2 * tr, 2 * tr, 90, -90);
        path.lineTo(r.right(), r.bottom() - br);
        if (br > 0.0)
            path.arcTo(r.right() - 2 * br, r.bottom() - 2 * br, 2 * br, 2 * br, 0, -90);
        path.lineTo(r.left() + bl, r.bottom());
        if (bl > 0.0)
            path.arcTo(r.left(), r.bottom() - 2 * bl, 2 * bl, 2 * bl, 270, -90);
        path.lineTo(r.left(), r.top() + tl);
        if (tl > 0.0)
            path.arcTo(r.left(), r.top(), 2 * tl, 2 * tl, 180, -90);
        path.closeSubpath();
        return path;
    }
    if (room.handDrawn) {
        std::mt19937 rng = handDrawnRng(room);
        const QPointF tl = r.topLeft();
        const QPointF tr = r.topRight();
        const QPointF br = r.bottomRight();
        const QPointF bl = r.bottomLeft();
        path.moveTo(tl);
        addHandDrawnEdge(path, tl, tr, rng);
        addHandDrawnEdge(path, tr, br, rng);
        addHandDrawnEdge(path, br, bl, rng);
        addHandDrawnEdge(path, bl, tl, rng);
        path.closeSubpath();
        return path;
    }
    path.addRect(r);
    return path;
}

// Build the closed sub-region filled by a room's second fill, matching the eight
// SecondFillLocation layouts in Room.Draw (half-and-half splits and corners).
QPainterPath buildSecondFillPath(const Room &room, const QRectF &r)
{
    const double w = r.width();
    const double h = r.height();
    const QPointF tl(r.left(), r.top());
    const QPointF tr(r.right(), r.top());
    const QPointF bl(r.left(), r.bottom());
    const QPointF br(r.right(), r.bottom());
    const QPointF tc(r.left() + w / 2.0, r.top());
    const QPointF rc(r.right(), r.top() + h / 2.0);
    const QPointF bc(r.left() + w / 2.0, r.bottom());
    const QPointF lc(r.left(), r.top() + h / 2.0);

    QPolygonF poly;
    const QString loc = room.secondFillLocation;
    if (loc == QLatin1String("Bottom"))
        poly << lc << rc << br << bl;
    else if (loc == QLatin1String("Top"))
        poly << lc << rc << tr << tl;
    else if (loc == QLatin1String("Left"))
        poly << tc << tl << bl << bc;
    else if (loc == QLatin1String("Right"))
        poly << tc << tr << br << bc;
    else if (loc == QLatin1String("BottomRight"))
        poly << bl << tr << br;
    else if (loc == QLatin1String("BottomLeft"))
        poly << tl << bl << br;
    else if (loc == QLatin1String("TopRight"))
        poly << tl << tr << br;
    else if (loc == QLatin1String("TopLeft"))
        poly << tl << tr << bl;
    else
        poly << lc << rc << br << bl; // default: Bottom

    QPainterPath path;
    path.addPolygon(poly);
    path.closeSubpath();
    return path;
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
    nameText = applyDashWrapping(nameText, map.settings.wrapTextAtDashes);
    subtitleText = applyDashWrapping(subtitleText, map.settings.wrapTextAtDashes);

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
    objs = applyDashWrapping(objs, map.settings.wrapTextAtDashes);

    const double off = map.settings.objectListOffset;
    QColor objColor = room.smallText.isValid() ? room.smallText
                                               : map.settings.colors[ColorSmallText];
    if (!objColor.isValid())
        objColor = QColor(60, 60, 60);
    const QFont objFont = qfontFromSpec(map.settings.objectFont, 11.0);
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
    setAcceptHoverEvents(true); // for resize-handle cursors
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
    setZValue(room->zOrder); // honour per-room stacking order (connections are at -1)
    prepareGeometryChange();
    setPos(room->x, room->y);
    m_applyingModel = false;

    // Hover tooltip: name, subtitle, and — per the application settings — the
    // description and object list (optionally truncated).
    const AppSettings &app = AppSettings::instance();
    QString tip = room->name.isEmpty() ? QObject::tr("(unnamed room)") : room->name;
    if (!room->subtitle.isEmpty())
        tip += QLatin1Char('\n') + room->subtitle;
    if (app.showDescriptionsInTooltips && !room->description.trimmed().isEmpty()) {
        QString desc = room->description.trimmed();
        if (app.limitRoomDescriptionChars && app.roomDescriptionChars > 0
            && desc.length() > app.roomDescriptionChars)
            desc = desc.left(app.roomDescriptionChars).trimmed() + QStringLiteral("…");
        tip += QStringLiteral("\n\n") + desc;
    }
    if (app.showObjectsInTooltips) {
        QString objs = room->objectsText;
        objs.remove(QRegularExpression(QStringLiteral("\\[[^\\]\\[]*\\]")));
        objs = objs.trimmed();
        if (!objs.isEmpty())
            tip += QObject::tr("\n\nObjects:\n") + objs;
    }
    setToolTip(tip);
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
    const bool isReference = room->referenceRoom >= 0;

    QColor fill = room->fill.isValid() ? room->fill : map->regionFill(room->region);
    if (!fill.isValid())
        fill = QColor(Qt::white);
    QColor border = room->border.isValid() ? room->border : map->borderColor();
    if (!border.isValid())
        border = QColor(Qt::black);

    const QPainterPath path = buildRoomPath(*room, rect);
    painter->setRenderHint(QPainter::Antialiasing, true);

    // Start / end / reference rooms get an inflated outline that follows the
    // room's shape (green/red from the palette; light blue for a reference).
    if (room->isStartRoom || room->isEndRoom || isReference) {
        const QPainterPath halo = buildRoomPath(*room, rect.adjusted(-5, -5, 5, 5));
        QColor haloColor = isReference
                               ? QColor(173, 216, 230)
                               : (room->isStartRoom ? map->settings.colors[ColorStartRoom]
                                                    : map->settings.colors[ColorEndRoom]);
        if (!haloColor.isValid())
            haloColor = room->isStartRoom ? QColor(Qt::green) : QColor(Qt::red);
        painter->setPen(QPen(haloColor, 2.0));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(halo);
    }

    // The room outline, fill, second fill and darkness stripe are only drawn
    // when the border style isn't "None" (matching Room.Draw, where a None
    // border suppresses the whole shape and leaves only the text).
    if (room->borderStyle != QLatin1String("None")) {
        // Fill the interior.
        painter->setPen(Qt::NoPen);
        painter->setBrush(fill);
        painter->drawPath(path);

        // Second fill: a coloured sub-region, clipped to the room shape.
        if (room->secondFill.isValid()) {
            painter->save();
            painter->setClipPath(path);
            painter->setPen(Qt::NoPen);
            painter->setBrush(room->secondFill);
            painter->drawPath(buildSecondFillPath(*room, rect));
            painter->restore();
        }

        // Darkness stripe: a corner triangle drawn in the border colour, sized
        // from the darkness-stripe setting and adjusted per shape.
        if (room->isDark) {
            double dx = map->settings.darknessStripeSize;
            double dy = map->settings.darknessStripeSize;
            if (room->ellipse) {
                dx = 2.0 * m_w / 5.0;
                dy = 2.0 * m_h / 5.0;
            } else if (room->octagonal) {
                dx = m_w * 7.0 / 20.0;
                dy = m_h * 7.0 / 20.0;
            } else if (room->roundedCorners &&
                       room->cornerTopRight > 2.0 * map->settings.darknessStripeSize) {
                dx = dy = room->cornerTopRight / 2.0;
            }
            painter->save();
            painter->setClipPath(path);
            painter->setPen(QPen(border, map->settings.lineWidth > 0 ? map->settings.lineWidth : 2.0));
            painter->setBrush(Qt::NoBrush);
            QPolygonF tri;
            tri << QPointF(m_w, 0) << QPointF(m_w - dx, 0) << QPointF(m_w, dy);
            painter->drawPolygon(tri);
            painter->restore();
        }

        // The border on top. Reference rooms always use a dotted border.
        QPen pen(border);
        pen.setWidthF(map->settings.lineWidth > 0 ? map->settings.lineWidth : 2.0);
        pen.setCapStyle(Qt::RoundCap);
        pen.setStyle(isReference ? Qt::DotLine : penStyleForBorder(room->borderStyle));
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);
    }

    if (m_scene->textVisible())
        drawRoomText(painter, *map, *room, rect);

    // Validation: a red X across rooms that fail the active validation rules.
    if (m_scene->roomInvalid(*room)) {
        painter->setPen(QPen(QColor(Qt::red), 2.0));
        painter->setBrush(Qt::NoBrush);
        painter->drawLine(rect.topLeft(), rect.bottomRight());
        painter->drawLine(rect.topRight(), rect.bottomLeft());
    }

    // Selection highlight: a gold outline following the room shape, plus the
    // eight compass resize handles.
    if (isSelected()) {
        const QPainterPath sel = buildRoomPath(*room, rect.adjusted(-5, -5, 5, 5));
        painter->setPen(QPen(QColor(255, 215, 0), 2.0));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(sel);

        const double hs = handleSize();
        painter->setPen(QPen(QColor(80, 80, 80), 1.0));
        painter->setBrush(QColor(255, 255, 255));
        for (int i = 0; i < 8; ++i) {
            const QPointF c = handlePoint(i, m_w, m_h);
            painter->drawRect(QRectF(c.x() - hs / 2.0, c.y() - hs / 2.0, hs, hs));
        }
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

void RoomItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!isSelected()) {
        if (scene())
            scene()->clearSelection();
        setSelected(true);
    }
    QMenu menu;
    QAction *editAct = menu.addAction(QObject::tr("Edit…"));
    QAction *deleteAct = menu.addAction(QObject::tr("Delete"));
    QAction *chosen = menu.exec(event->screenPos());
    if (chosen == editAct)
        m_scene->activateRoom(m_roomId);
    else if (chosen == deleteAct)
        m_scene->deleteSelection();
    event->accept();
}

double RoomItem::handleSize() const
{
    if (const Map *map = m_scene ? m_scene->document() : nullptr)
        if (map->settings.handleSize > 1.0)
            return map->settings.handleSize;
    return 10.0;
}

int RoomItem::handleAt(const QPointF &localPos) const
{
    if (!isSelected())
        return -1;
    const double tol = handleSize() / 2.0 + 2.0;
    for (int i = 0; i < 8; ++i) {
        const QPointF c = handlePoint(i, m_w, m_h);
        if (std::abs(localPos.x() - c.x()) <= tol && std::abs(localPos.y() - c.y()) <= tol)
            return i;
    }
    return -1;
}

void RoomItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    const int h = handleAt(event->pos());
    if (h == 0 || h == 4)
        setCursor(Qt::SizeVerCursor);
    else if (h == 2 || h == 6)
        setCursor(Qt::SizeHorCursor);
    else if (h == 1 || h == 5)
        setCursor(Qt::SizeBDiagCursor);
    else if (h == 3 || h == 7)
        setCursor(Qt::SizeFDiagCursor);
    else
        unsetCursor();
    QGraphicsItem::hoverMoveEvent(event);
}

void RoomItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    unsetCursor();
    QGraphicsItem::hoverLeaveEvent(event);
}

void RoomItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const int h = handleAt(event->pos());
        if (h >= 0) {
            m_resizeHandle = h;
            m_resizeStartScene = event->scenePos();
            if (const Room *r = m_scene->document()->roomById(m_roomId)) {
                m_startX = r->x;
                m_startY = r->y;
                m_startW = r->w;
                m_startH = r->h;
            }
            m_scene->setRoomResizeActive(true); // suppress the move-undo snapshot
            event->accept();
            return;
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void RoomItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizeHandle >= 0) {
        const int i = m_resizeHandle;
        const QPointF d = event->scenePos() - m_resizeStartScene;
        double x = m_startX, y = m_startY, w = m_startW, h = m_startH;
        if (i == 1 || i == 2 || i == 3)
            w = m_startW + d.x(); // right edge
        if (i == 5 || i == 6 || i == 7) {
            x = m_startX + d.x();
            w = m_startW - d.x(); // left edge
        }
        if (i == 3 || i == 4 || i == 5)
            h = m_startH + d.y(); // bottom edge
        if (i == 0 || i == 1 || i == 7) {
            y = m_startY + d.y();
            h = m_startH - d.y(); // top edge
        }
        const double minS = 16.0;
        const bool left = (i == 5 || i == 6 || i == 7);
        const bool top = (i == 0 || i == 1 || i == 7);
        if (w < minS) {
            if (left)
                x = m_startX + m_startW - minS;
            w = minS;
        }
        if (h < minS) {
            if (top)
                y = m_startY + m_startH - minS;
            h = minS;
        }
        if (Room *r = m_scene->document()->roomById(m_roomId)) {
            r->x = x;
            r->y = y;
            r->w = w;
            r->h = h;
            m_w = w;
            m_h = h;
            m_applyingModel = true;
            prepareGeometryChange();
            setPos(x, y);
            m_applyingModel = false;
            update();
            m_scene->refreshConnectionsFor(m_roomId);
        }
        event->accept();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void RoomItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizeHandle >= 0) {
        m_resizeHandle = -1;
        m_scene->setRoomResizeActive(false);
        Room *r = m_scene->document() ? m_scene->document()->roomById(m_roomId) : nullptr;
        if (r) {
            // Snap the edges to the grid if snapping is on.
            if (m_scene->document()->settings.snapToGrid) {
                const double g = m_scene->gridSize();
                const double nx = std::round(r->x / g) * g;
                const double ny = std::round(r->y / g) * g;
                const double nr = std::round((r->x + r->w) / g) * g;
                const double nb = std::round((r->y + r->h) / g) * g;
                r->x = nx;
                r->y = ny;
                r->w = std::max(g, nr - nx);
                r->h = std::max(g, nb - ny);
                m_w = r->w;
                m_h = r->h;
                m_applyingModel = true;
                prepareGeometryChange();
                setPos(r->x, r->y);
                m_applyingModel = false;
                update();
                m_scene->refreshConnectionsFor(m_roomId);
            }
            Room before = *r;
            before.x = m_startX;
            before.y = m_startY;
            before.w = m_startW;
            before.h = m_startH;
            const Room after = *r;
            if (m_scene->undoStack() &&
                (after.x != before.x || after.y != before.y || after.w != before.w ||
                 after.h != before.h)) {
                m_scene->undoStack()->push(new EditRoomCommand(m_scene, m_roomId, before, after));
            }
        }
        event->accept();
        return;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

} // namespace trizbort
