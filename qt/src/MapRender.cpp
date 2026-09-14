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

#include "MapRender.h"

#include <QImage>
#include <QMarginsF>
#include <QObject>
#include <QPageSize>
#include <QPainter>
#include <QPdfWriter>

#include "MapScene.h"

namespace trizbort {

namespace {

QRectF contentRect(QGraphicsScene &scene)
{
    QRectF r = scene.itemsBoundingRect();
    if (r.isEmpty())
        r = QRectF(0, 0, 200, 200);
    return r.adjusted(-24, -24, 24, 24);
}

QColor canvas(const Map &map)
{
    const QColor c = map.canvasColor();
    return c.isValid() ? c : QColor(Qt::white);
}

} // namespace

bool renderMapToImage(const Map &map, const QString &path, QString *errorMessage)
{
    // Render off a copy with the grid suppressed, so the export is clean.
    Map copy = map;
    copy.settings.gridVisible = false;
    MapScene scene;
    scene.setDocument(&copy);

    const QRectF bounds = contentRect(scene);
    const QSize size = bounds.size().toSize();
    QImage image(size, QImage::Format_ARGB32);
    image.fill(canvas(copy));
    {
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        scene.render(&painter, QRectF(QPointF(0, 0), bounds.size()), bounds);
    }
    if (!image.save(path)) {
        if (errorMessage)
            *errorMessage = QObject::tr("Could not write image %1").arg(path);
        return false;
    }
    return true;
}

bool renderMapToPdf(const Map &map, const QString &path, QString *errorMessage)
{
    Map copy = map;
    copy.settings.gridVisible = false;
    MapScene scene;
    scene.setDocument(&copy);

    const QRectF bounds = contentRect(scene);

    QPdfWriter writer(path);
    writer.setResolution(72); // 1 map unit == 1 PostScript point
    writer.setPageSize(QPageSize(QSizeF(bounds.width(), bounds.height()), QPageSize::Point));
    writer.setPageMargins(QMarginsF(0, 0, 0, 0));
    writer.setTitle(map.title.isEmpty() ? QStringLiteral("Trizbort map") : map.title);

    QPainter painter;
    if (!painter.begin(&writer)) {
        if (errorMessage)
            *errorMessage = QObject::tr("Could not open %1 for PDF output").arg(path);
        return false;
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    const QRectF target(0, 0, writer.width(), writer.height());
    painter.fillRect(target, canvas(copy));
    scene.render(&painter, target, bounds);
    painter.end();
    return true;
}

} // namespace trizbort
