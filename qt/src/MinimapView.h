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

#ifndef TRIZBORT_MINIMAPVIEW_H
#define TRIZBORT_MINIMAPVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>

#include "MapView.h"

namespace trizbort {

// A non-interactive overview of the whole scene. It shares the editing scene,
// always fits the entire map, draws the main view's visible rectangle, and
// recentres the main view when clicked or dragged. Header-only: it overrides
// virtuals and holds a raw pointer to the main view, so it needs no moc.
class MinimapView : public QGraphicsView {
public:
    MinimapView(QGraphicsScene *scene, MapView *mainView, QWidget *parent = nullptr)
        : QGraphicsView(scene, parent)
        , m_main(mainView)
    {
        setInteractive(false);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setRenderHint(QPainter::Antialiasing, true);
        setFrameStyle(QFrame::NoFrame);
        setMinimumSize(140, 110);
        setCursor(Qt::PointingHandCursor);
        fit();

        connect(scene, &QGraphicsScene::sceneRectChanged, this, [this] { fit(); });
        for (QScrollBar *bar : {m_main->horizontalScrollBar(), m_main->verticalScrollBar()}) {
            connect(bar, &QScrollBar::valueChanged, this, [this] { viewport()->update(); });
            connect(bar, &QScrollBar::rangeChanged, this, [this] { viewport()->update(); });
        }
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QGraphicsView::resizeEvent(event);
        fit();
    }

    void drawForeground(QPainter *painter, const QRectF &) override
    {
        if (!m_main || !m_main->scene())
            return;
        const QRectF visible = m_main->mapToScene(m_main->viewport()->rect()).boundingRect();
        QPen pen(QColor(30, 120, 220));
        pen.setCosmetic(true);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(QColor(30, 120, 220, 40));
        painter->drawRect(visible);
    }

    void mousePressEvent(QMouseEvent *event) override { recenter(event->pos()); }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton)
            recenter(event->pos());
    }

private:
    void fit()
    {
        if (scene())
            fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    void recenter(const QPoint &pos)
    {
        if (m_main)
            m_main->centerOn(mapToScene(pos));
    }

    MapView *m_main;
};

} // namespace trizbort

#endif // TRIZBORT_MINIMAPVIEW_H
