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
#include <QDir>
#include <QFile>
#include <QImage>
#include <QUndoStack>
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "ConnectionItem.h"
#include "EditCommands.h"
#include "MainWindow.h"
#include "MapDocument.h"
#include "MapRender.h"
#include "MapScene.h"
#include "MapStatisticsDialog.h"
#include "RoomItem.h"
#include "SettingsDialog.h"
#include "TranscriptAutomapper.h"
#include "TrizbortReader.h"
#include "TrizbortWriter.h"
#include "export/CodeExporter.h"
#include "export/ExporterFactory.h"

// Headless render: load a map and write a PNG or PDF, no window. Useful for CI
// smoke tests and thumbnails. Run with QT_QPA_PLATFORM=offscreen on a headless
// host.
static int renderToFile(const QString &mapPath, const QString &outPath, bool pdf)
{
    QTextStream err(stderr);

    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        err << "load failed: " << error << Qt::endl;
        return 1;
    }

    const bool ok = pdf ? trizbort::renderMapToPdf(map, outPath, &error)
                        : trizbort::renderMapToImage(map, outPath, &error);
    if (!ok) {
        err << error << Qt::endl;
        return 3;
    }
    QTextStream(stdout) << "rendered " << map.rooms.size() << " rooms, "
                        << map.connections.size() << " connections -> " << outPath << Qt::endl;
    return 0;
}

using trizbort::makeExporter;

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

// Headless transcript import: build a new map from a play transcript and save
// it as a .trizbort file.
static int runImportTranscript(const QString &transcriptPath, const QString &outPath)
{
    QTextStream err(stderr);
    QFile file(transcriptPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        err << "could not read " << transcriptPath << Qt::endl;
        return 1;
    }
    const QString text = QString::fromUtf8(file.readAll());

    trizbort::Map map;
    map.regions.append(trizbort::Region{trizbort::kNoRegion, QColor(0, 0, 255), QColor(255, 255, 255),
                                        QString(), QString()});
    trizbort::TranscriptAutomapper mapper;
    const int added = mapper.run(map, text);

    QString error;
    if (!trizbort::TrizbortWriter::save(outPath, map, &error)) {
        err << "save failed: " << error << Qt::endl;
        return 3;
    }
    QTextStream(stdout) << "imported transcript: " << added << " rooms, "
                        << mapper.connectionsAdded() << " connections -> " << outPath << Qt::endl;
    return 0;
}

// Headless save: load a map and write it back out via the .trizbort writer.
// Used to validate that a load/save round trip preserves the whole document.
static int runSave(const QString &mapPath, const QString &outPath)
{
    QTextStream err(stderr);

    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        err << "load failed: " << error << Qt::endl;
        return 1;
    }
    if (!trizbort::TrizbortWriter::save(outPath, map, &error)) {
        err << "save failed: " << error << Qt::endl;
        return 3;
    }
    return 0;
}

static int runStats(const QString &mapPath)
{
    trizbort::Map map;
    QString error;
    if (!trizbort::TrizbortReader::load(mapPath, map, &error)) {
        QTextStream(stderr) << "load failed: " << error << Qt::endl;
        return 1;
    }
    QTextStream(stdout) << trizbort::buildStatisticsReport(map) << Qt::endl;
    return 0;
}

// Headless self-test for the GUI-independent editing API: build a small map,
// round-trip it through the writer/reader, and export it. Prints PASS/FAIL.
static int runEditSelftest()
{
    using namespace trizbort;
    QTextStream out(stdout);
    int failures = 0;
    auto check = [&](bool ok, const char *what) {
        if (!ok) {
            out << "  FAIL: " << what << Qt::endl;
            ++failures;
        }
    };

    Map map;
    const int a = map.addRoom(0, 0);
    const int b = map.addRoom(0, -128);
    check(map.rooms.size() == 2, "two rooms added");
    check(a != b, "distinct room ids");

    Room *ra = map.roomById(a);
    Room *rb = map.roomById(b);
    check(ra && rb, "rooms fetchable by id");
    if (ra) {
        ra->name = QStringLiteral("Start Room");
        ra->isStartRoom = true;
        ra->objectsText = QStringLiteral("a brass lantern");
    }
    if (rb)
        rb->name = QStringLiteral("North Room");

    const QString portA = Map::portFacing(*map.roomById(a), *map.roomById(b));
    const QString portB = Map::portFacing(*map.roomById(b), *map.roomById(a));
    check(portA == QLatin1String("n"), "portFacing north");
    check(portB == QLatin1String("s"), "portFacing south");
    map.addConnection(a, portA, b, portB);
    check(map.connections.size() == 1, "one connection added");

    // Save, reload, and verify the model survived.
    const QString path = QDir(QDir::tempPath()).filePath(QStringLiteral("tz-edit-selftest.trizbort"));
    QString err;
    check(TrizbortWriter::save(path, map, &err), "save");
    Map reloaded;
    check(TrizbortReader::load(path, reloaded, &err), "reload");
    check(reloaded.rooms.size() == 2, "reloaded two rooms");
    check(reloaded.connections.size() == 1, "reloaded one connection");
    if (const Room *r = reloaded.roomById(a)) {
        check(r->name == QLatin1String("Start Room"), "reloaded room name");
        check(r->isStartRoom, "reloaded start flag");
        check(r->objectsText == QLatin1String("a brass lantern"), "reloaded objects");
    }

    // The reloaded map must export.
    auto exporter = makeExporter(QStringLiteral("zil"), reloaded, path);
    const QString zil = exporter ? exporter->exportToString() : QString();
    check(zil.contains(QLatin1String("START-ROOM")) || zil.contains(QLatin1String("ROOM")),
          "export produced rooms");

    // Deleting the start room drops its connection too.
    reloaded.removeRoom(a);
    check(reloaded.rooms.size() == 1, "removeRoom left one room");
    check(reloaded.connections.isEmpty(), "removeRoom dropped its connection");

    QFile::remove(path);
    out << (failures == 0 ? "edit-selftest: PASS" : "edit-selftest: FAIL") << Qt::endl;
    return failures == 0 ? 0 : 1;
}

// Headless smoke test for the editing canvas: exercises the scene's item
// machinery (add/move/connect/delete) and confirms MainWindow loads a map.
static int runGuiSelftest(const QString &samplePath)
{
    using namespace trizbort;
    QTextStream out(stdout);
    int failures = 0;
    auto check = [&](bool ok, const char *what) {
        if (!ok) {
            out << "  FAIL: " << what << Qt::endl;
            ++failures;
        }
    };

    if (!samplePath.isEmpty()) {
        MainWindow win;
        check(win.loadFile(samplePath), "MainWindow loads a sample map");

        // Settings dialog plumbing: a no-op edit reads its inputs back unchanged.
        Map sm;
        QString err;
        if (TrizbortReader::load(samplePath, sm, &err)) {
            SettingsDialog dlg(sm.settings, sm.regions);
            const MapSettings rs = dlg.resultSettings();
            check(rs.colors[ColorCanvas] == sm.settings.colors[ColorCanvas]
                      && rs.colors[ColorGrid] == sm.settings.colors[ColorGrid]
                      && rs.gridSize == sm.settings.gridSize
                      && rs.defaultRoomName == sm.settings.defaultRoomName,
                  "settings dialog reads back its inputs");
            check(dlg.resultRegions().size() == sm.regions.size(),
                  "settings dialog preserves the region count");
        }
    }

    Map map;
    MapScene scene;
    scene.setDocument(&map);
    const int a = scene.addRoomAt(QPointF(0, 0));
    const int b = scene.addRoomAt(QPointF(0, -128));
    check(map.rooms.size() == 2 && a >= 0 && b >= 0, "addRoomAt created two rooms");

    scene.roomMovedTo(a, QPointF(64, -64));
    check(map.roomById(a) && map.roomById(a)->x == 64 && map.roomById(a)->y == -64,
          "roomMovedTo persisted to the model");

    map.addConnection(a, QStringLiteral("n"), b, QStringLiteral("s"));
    scene.setDocument(&map);
    int roomItems = 0;
    int connItems = 0;
    for (QGraphicsItem *it : scene.items()) {
        if (dynamic_cast<RoomItem *>(it))
            ++roomItems;
        else if (dynamic_cast<ConnectionItem *>(it))
            ++connItems;
    }
    check(roomItems == 2, "scene has two room items");
    check(connItems == 1, "scene has one connection item");

    for (QGraphicsItem *it : scene.items()) {
        if (auto *ri = dynamic_cast<RoomItem *>(it)) {
            if (ri->roomId() == a)
                ri->setSelected(true);
        }
    }
    scene.deleteSelection();
    check(map.rooms.size() == 1, "deleteSelection removed the room");
    check(map.connections.isEmpty(), "deleteSelection removed its connection");

    out << (failures == 0 ? "gui-selftest: PASS" : "gui-selftest: FAIL") << Qt::endl;
    return failures == 0 ? 0 : 1;
}

// Headless test for the undo/redo command classes.
static int runUndoSelftest()
{
    using namespace trizbort;
    QTextStream out(stdout);
    int failures = 0;
    auto check = [&](bool ok, const char *what) {
        if (!ok) {
            out << "  FAIL: " << what << Qt::endl;
            ++failures;
        }
    };

    Map map;
    MapScene scene;
    QUndoStack stack;
    scene.setUndoStack(&stack);
    scene.setDocument(&map);

    check(stack.isClean(), "starts clean");
    const int a = scene.addRoomAt(QPointF(0, 0));
    const int b = scene.addRoomAt(QPointF(0, -128));
    check(map.rooms.size() == 2, "two rooms after add");
    check(!stack.isClean(), "dirty after edits");

    stack.push(new AddConnectionCommand(&scene, a, QStringLiteral("n"), b, QStringLiteral("s")));
    check(map.connections.size() == 1, "connection added");

    // Undo the connection, then redo it.
    stack.undo();
    check(map.connections.isEmpty(), "undo removed connection");
    stack.redo();
    check(map.connections.size() == 1, "redo restored connection");

    // Edit a room name, then undo.
    {
        Room before = *map.roomById(a);
        Room after = before;
        after.name = QStringLiteral("Renamed");
        stack.push(new EditRoomCommand(&scene, a, before, after));
        check(map.roomById(a)->name == QLatin1String("Renamed"), "room renamed");
        stack.undo();
        check(map.roomById(a)->name == before.name, "undo restored room name");
    }

    // Move a room, then undo.
    {
        const QPointF oldPos(map.roomById(a)->x, map.roomById(a)->y);
        QList<RoomMove> moves{{a, oldPos, QPointF(96, 96)}};
        stack.push(new MoveRoomsCommand(&scene, moves));
        check(map.roomById(a)->x == 96 && map.roomById(a)->y == 96, "room moved");
        stack.undo();
        check(map.roomById(a)->x == oldPos.x() && map.roomById(a)->y == oldPos.y(),
              "undo restored room position");
    }

    // Delete a room (and its connection), then undo to restore both.
    {
        const int roomsBefore = map.rooms.size();
        const int connsBefore = map.connections.size();
        stack.push(new DeleteElementsCommand(&scene, QList<int>{a}, QList<int>{}));
        check(map.rooms.size() == roomsBefore - 1, "delete removed the room");
        check(map.connections.isEmpty(), "delete removed its connection");
        stack.undo();
        check(map.rooms.size() == roomsBefore, "undo restored the room");
        check(map.connections.size() == connsBefore, "undo restored its connection");
    }

    // Clean-state tracking round-trips.
    stack.setClean();
    check(stack.isClean(), "clean after setClean");
    stack.undo();
    check(!stack.isClean(), "dirty after undoing past the clean point");

    out << (failures == 0 ? "undo-selftest: PASS" : "undo-selftest: FAIL") << Qt::endl;
    return failures == 0 ? 0 : 1;
}

// Headless test for the automap primitive: create connected rooms in each
// direction and check placement and the resulting connection.
static int runAutomapSelftest()
{
    using namespace trizbort;
    QTextStream out(stdout);
    int failures = 0;
    auto check = [&](bool ok, const char *what) {
        if (!ok) {
            out << "  FAIL: " << what << Qt::endl;
            ++failures;
        }
    };

    struct Case {
        const char *dir;
        double x;
        double y;
    };
    // From a 96x64 room at (0,0) with a 64-unit gap: north/west subtract the new
    // room size + gap; east/south add the source size + gap; all snapped.
    const Case cases[] = {
        {"n", 0, -128}, {"s", 0, 128}, {"e", 160, 0}, {"w", -160, 0},
        {"ne", 160, -128}, {"sw", -160, 128},
    };

    for (const Case &c : cases) {
        Map map;
        MapScene scene;
        QUndoStack stack;
        scene.setUndoStack(&stack);
        scene.setDocument(&map);
        const int a = scene.addRoomAt(QPointF(0, 0));
        scene.selectRoomItem(a);

        auto *cmd = new AddConnectedRoomCommand(&scene, a, QString::fromLatin1(c.dir));
        check(cmd->valid(), "command valid");
        const int nid = cmd->newRoomId();
        stack.push(cmd);

        check(map.rooms.size() == 2, "room added");
        check(map.connections.size() == 1, "connection added");
        const Room *nr = map.roomById(nid);
        check(nr && nr->x == c.x && nr->y == c.y, c.dir);
        if (!(nr && nr->x == c.x && nr->y == c.y))
            out << "    (" << c.dir << " got " << (nr ? nr->x : 0) << "," << (nr ? nr->y : 0)
                << ")" << Qt::endl;
        // The connection docks the source's direction port to the new room.
        const Connection &conn = map.connections.first();
        check(conn.vertices.size() == 2 && conn.vertices.at(0).roomId == a
                  && conn.vertices.at(0).port == QLatin1String(c.dir),
              "connection docks source port");

        stack.undo();
        check(map.rooms.size() == 1 && map.connections.isEmpty(), "undo removes both");
    }

    out << (failures == 0 ? "automap-selftest: PASS" : "automap-selftest: FAIL") << Qt::endl;
    return failures == 0 ? 0 : 1;
}

// Headless test for the transcript automapper: a short walk builds a 2x2 grid.
static int runTranscriptSelftest()
{
    using namespace trizbort;
    QTextStream out(stdout);
    int failures = 0;
    auto check = [&](bool ok, const char *what) {
        if (!ok) {
            out << "  FAIL: " << what << Qt::endl;
            ++failures;
        }
    };

    const QString transcript = QStringLiteral(
        "Cave Adventure\n"
        "An interactive fiction\n"
        "\n"
        "West of House\n"
        "You are standing in an open field west of a white house.\n"
        "\n"
        ">north\n"
        "North of House\n"
        "You are facing the north side of a white house.\n"
        "\n"
        ">east\n"
        "Behind House\n"
        "You are behind the white house.\n"
        "\n"
        ">south\n"
        "South of House\n"
        "This is the south side of a white house.\n"
        "\n"
        ">tb see brass lantern\n"
        ">\n");

    Map map;
    TranscriptAutomapper mapper;
    mapper.run(map, transcript);

    check(map.rooms.size() == 4, "four rooms created");
    check(map.connections.size() == 3, "three connections created");

    auto roomNamed = [&](const QString &n) -> const Room * {
        for (const Room &r : map.rooms)
            if (r.name == n)
                return &r;
        return nullptr;
    };
    const Room *west = roomNamed(QStringLiteral("West of House"));
    const Room *north = roomNamed(QStringLiteral("North of House"));
    const Room *behind = roomNamed(QStringLiteral("Behind House"));
    const Room *south = roomNamed(QStringLiteral("South of House"));
    check(west && north && behind && south, "all four rooms present by name");
    check(west && west->isStartRoom, "first room is the start room");
    if (west && north)
        check(north->x == west->x && north->y == west->y - 128, "north placed above west");
    if (behind && north)
        check(behind->y == north->y && behind->x == north->x + 160, "behind placed east of north");
    // The 'tb see' command adds an object to the last room (South of House).
    check(south && south->objectsText.contains(QLatin1String("brass lantern")),
          "tb see added an object");

    // Re-exports cleanly.
    auto exporter = makeExporter(QStringLiteral("inform7"), map, QStringLiteral("t.trizbort"));
    check(exporter && exporter->exportToString().contains(QLatin1String("West of House")),
          "automapped map exports");

    out << (failures == 0 ? "transcript-selftest: PASS" : "transcript-selftest: FAIL") << Qt::endl;
    return failures == 0 ? 0 : 1;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Trizbort (Qt)"));

    if (argc >= 2 && QString::fromLocal8Bit(argv[1]) == QLatin1String("--edit-selftest"))
        return runEditSelftest();
    if (argc >= 2 && QString::fromLocal8Bit(argv[1]) == QLatin1String("--transcript-selftest"))
        return runTranscriptSelftest();
    if (argc >= 2 && QString::fromLocal8Bit(argv[1]) == QLatin1String("--automap-selftest"))
        return runAutomapSelftest();
    if (argc >= 2 && QString::fromLocal8Bit(argv[1]) == QLatin1String("--undo-selftest"))
        return runUndoSelftest();
    if (argc >= 2 && QString::fromLocal8Bit(argv[1]) == QLatin1String("--gui-selftest"))
        return runGuiSelftest(argc >= 3 ? QString::fromLocal8Bit(argv[2]) : QString());

    QString mapPath;
    QString renderPath;
    QString pdfPath;
    QString savePath;
    QString transcriptPath;
    QString transcriptOut;
    QString exportFmt;
    QString exportOut;
    bool statsRequested = false;
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
        if (args.at(i) == QLatin1String("--pdf") && i + 1 < args.size()) {
            pdfPath = args.at(++i);
            continue;
        }
        if (args.at(i) == QLatin1String("--save") && i + 1 < args.size()) {
            savePath = args.at(++i);
            continue;
        }
        if (args.at(i) == QLatin1String("--stats")) {
            statsRequested = true;
            continue;
        }
        if (args.at(i) == QLatin1String("--import-transcript") && i + 2 < args.size()) {
            transcriptPath = args.at(++i);
            transcriptOut = args.at(++i);
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

    if (statsRequested) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --stats" << Qt::endl;
            return 2;
        }
        return runStats(mapPath);
    }

    if (!transcriptPath.isEmpty()) {
        return runImportTranscript(transcriptPath, transcriptOut);
    }

    if (!savePath.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --save <out.trizbort>"
                                << Qt::endl;
            return 2;
        }
        return runSave(mapPath, savePath);
    }

    if (!pdfPath.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --pdf <out.pdf>" << Qt::endl;
            return 2;
        }
        return renderToFile(mapPath, pdfPath, /*pdf=*/true);
    }

    if (!renderPath.isEmpty()) {
        if (mapPath.isEmpty()) {
            QTextStream(stderr) << "usage: trizbort-qt <map.trizbort> --render <out.png>"
                                << Qt::endl;
            return 2;
        }
        return renderToFile(mapPath, renderPath, /*pdf=*/false);
    }

    trizbort::MainWindow window;
    window.resize(1000, 700);
    if (!mapPath.isEmpty())
        window.loadFile(mapPath);
    window.show();
    return app.exec();
}
