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

#ifndef TRIZBORT_ROOMITEM_H
#define TRIZBORT_ROOMITEM_H

#include <QGraphicsItem>

namespace trizbort {

class MapScene;

// An interactive room on the editing canvas. Reads its geometry and appearance
// from the room in the document (looked up by id) and, when dragged, writes the
// new grid-snapped position back through the scene.
class RoomItem : public QGraphicsItem {
public:
    RoomItem(MapScene *scene, int roomId);

    int roomId() const { return m_roomId; }

    // Re-read geometry from the model and reposition (called after edits).
    void syncFromModel();

    QRectF boundingRect() const override;
    // The clickable/selectable outline is the room rectangle itself, even though
    // boundingRect is padded to cover subtitle and object-list text drawn around
    // the room.
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    double handleSize() const;
    // Index 0..7 of the resize handle under a local point, or -1.
    int handleAt(const QPointF &localPos) const;

    MapScene *m_scene;
    int m_roomId;
    double m_w = 96.0;
    double m_h = 64.0;
    bool m_applyingModel = false; // guard against feedback while syncing

    bool m_hovered = false;       // mouse over this room (for connect-mode ports)
    int m_resizeHandle = -1;      // active resize handle, or -1
    QPointF m_resizeStartScene;   // scene pos where the resize drag began
    double m_startX = 0, m_startY = 0, m_startW = 0, m_startH = 0;
};

} // namespace trizbort

#endif // TRIZBORT_ROOMITEM_H
