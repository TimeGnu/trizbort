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

#include "MapView.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>

namespace trizbort {

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    // Connections are long, thin items whose bounding rectangle is a large,
    // mostly-empty diagonal band anchored at the scene origin (see
    // ConnectionItem::boundingRect). Qt's default MinimalViewportUpdate mode
    // scrolls by blitting the existing pixels and repainting only the newly
    // exposed strip, driven by the scene's item index; that interacts poorly
    // with such items and can leave connection lines unpainted until a
    // transform change (a zoom step) forces a full redraw. Repainting the whole
    // viewport on every scroll keeps the lines present and the scrolling smooth.
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void MapView::zoomIn() { scale(1.15, 1.15); }
void MapView::zoomOut() { scale(1.0 / 1.15, 1.0 / 1.15); }
void MapView::resetZoom() { resetTransform(); }

void MapView::setZoomPercent(double percent)
{
    resetTransform();
    const double f = percent / 100.0;
    scale(f, f);
}

void MapView::microZoom(bool in)
{
    const double f = in ? 1.01 : 1.0 / 1.01;
    scale(f, f);
}

void MapView::resetOrigin()
{
    resetTransform();
    centerOn(0, 0);
}

void MapView::zoomToFit()
{
    if (scene())
        fitInView(scene()->itemsBoundingRect().adjusted(-40, -40, 40, 40), Qt::KeepAspectRatio);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    const double step = 1.15;
    bool zoomIn = event->angleDelta().y() > 0;
    if (m_invertWheel)
        zoomIn = !zoomIn;
    const double factor = zoomIn ? step : 1.0 / step;
    scale(factor, factor);
    event->accept();
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        m_panning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning) {
        const QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton && m_panning) {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

} // namespace trizbort
