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

#include <QApplication>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "MainWindow.h"
#include "MapDocument.h"
#include "MapScene.h"
#include "TrizbortReader.h"
#include "export/ZilExporter.h"

// Headless render: load a map and write a PNG, no window. Useful for CI smoke
// tests and thumbnails. Run with QT_QPA_PLATFORM=offscreen on a headless host.
static int renderToPng(const QString &mapPath, const QString &pngPath)
{
    QTextStream err(stderr);

    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        err << "load failed: " << error << Qt::endl;
        return 1;
    }

    trizbort::MapScene scene;
    scene.setMap(map);

    QRectF bounds = scene.itemsBoundingRect().adjusted(-20, -20, 20, 20);
    if (bounds.isEmpty())
        bounds = QRectF(0, 0, 200, 200);

    QImage image(bounds.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    {
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        scene.render(&painter, QRectF(QPointF(0, 0), bounds.size()), bounds);
    }

    if (!image.save(pngPath)) {
        err << "could not write " << pngPath << Qt::endl;
        return 3;
    }
    QTextStream(stdout) << "rendered " << map.rooms.size() << " rooms, "
                        << map.connections.size() << " connections -> " << pngPath
                        << Qt::endl;
    return 0;
}

// Headless ZIL export: load a map and write ZIL source. Used to validate the
// C++ exporter against the golden fixtures.
static int exportZil(const QString &mapPath, const QString &outPath)
{
    QTextStream err(stderr);

    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        err << "load failed: " << error << Qt::endl;
        return 1;
    }

    trizbort::ZilExporter exporter(map, mapPath);
    const QString text = exporter.exportToString();

    QFile file(outPath);
    if (!file.open(QIODevice::WriteOnly)) {
        err << "could not write " << outPath << Qt::endl;
        return 3;
    }
    file.write(text.toUtf8());
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Trizbort (Qt)"));

    QString mapPath;
    QString renderPath;
    QString zilPath;
    const QStringList args = QApplication::arguments();
    for (int i = 1; i < args.size(); ++i) {
        if (args.at(i) == QLatin1String("--render") && i + 1 < args.size())
            renderPath = args.at(++i);
        else if (args.at(i) == QLatin1String("--zil") && i + 1 < args.size())
            zilPath = args.at(++i);
        else
            mapPath = args.at(i);
    }

    if (!zilPath.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --zil <out.zil>" << Qt::endl;
            return 2;
        }
        return exportZil(mapPath, zilPath);
    }

    if (!renderPath.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --render <out.png>"
                                << Qt::endl;
            return 2;
        }
        return renderToPng(mapPath, renderPath);
    }

    trizbort::MainWindow window;
    window.resize(1000, 700);
    if (!mapPath.isEmpty())
        window.loadFile(mapPath);
    window.show();
    return app.exec();
}
