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

#include <memory>

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
#include "export/AdventuronExporter.h"
#include "export/AlanExporter.h"
#include "export/CodeExporter.h"
#include "export/HugoExporter.h"
#include "export/TadsExporter.h"
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

// Construct an exporter by short format name. Adding a format is one line here.
static std::unique_ptr<trizbort::CodeExporter> makeExporter(const QString &fmt,
                                                            const trizbort::Map &map,
                                                            const QString &path)
{
    using namespace trizbort;
    if (fmt == QLatin1String("zil"))
        return std::make_unique<ZilExporter>(map, path);
    if (fmt == QLatin1String("adventuron"))
        return std::make_unique<AdventuronExporter>(map, path);
    if (fmt == QLatin1String("hugo"))
        return std::make_unique<HugoExporter>(map, path);
    if (fmt == QLatin1String("alan"))
        return std::make_unique<AlanExporter>(map, path);
    if (fmt == QLatin1String("tads"))
        return std::make_unique<TadsExporter>(map, path);
    return nullptr;
}

// Headless export: load a map, run the named exporter, write the result. Used
// to validate the C++ exporters against the golden fixtures.
static int runExport(const QString &fmt, const QString &mapPath, const QString &outPath)
{
    QTextStream err(stderr);

    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        err << "load failed: " << error << Qt::endl;
        return 1;
    }

    auto exporter = makeExporter(fmt, map, mapPath);
    if (!exporter) {
        err << "unknown/unimplemented format: " << fmt << Qt::endl;
        return 2;
    }
    const QString text = exporter->exportToString();

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
    QString exportFmt;
    QString exportOut;
    static const struct {
        const char *flag;
        const char *fmt;
    } kFormats[] = {
        {"--zil", "zil"},         {"--adventuron", "adventuron"}, {"--inform6", "inform6"},
        {"--inform7", "inform7"}, {"--tads", "tads"},             {"--alan", "alan"},
        {"--hugo", "hugo"},       {"--quest", "quest"},
    };

    const QStringList args = QApplication::arguments();
    for (int i = 1; i < args.size(); ++i) {
        if (args.at(i) == QLatin1String("--render") && i + 1 < args.size()) {
            renderPath = args.at(++i);
            continue;
        }
        bool matched = false;
        for (const auto &f : kFormats) {
            if (args.at(i) == QLatin1String(f.flag) && i + 1 < args.size()) {
                exportFmt = QLatin1String(f.fmt);
                exportOut = args.at(++i);
                matched = true;
                break;
            }
        }
        if (!matched)
            mapPath = args.at(i);
    }

    if (!exportFmt.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --<format> <out>" << Qt::endl;
            return 2;
        }
        return runExport(exportFmt, mapPath, exportOut);
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
