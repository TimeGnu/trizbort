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

#ifndef TRIZBORT_MAPVIEW_H
#define TRIZBORT_MAPVIEW_H

#include <QGraphicsView>

class QWheelEvent;

namespace trizbort {

// A QGraphicsView with mouse-wheel zoom and click-drag panning.
class MapView : public QGraphicsView {
public:
    explicit MapView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

} // namespace trizbort

#endif // TRIZBORT_MAPVIEW_H
