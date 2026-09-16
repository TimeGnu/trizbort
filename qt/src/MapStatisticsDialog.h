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

#ifndef TRIZBORT_MAPSTATISTICSDIALOG_H
#define TRIZBORT_MAPSTATISTICSDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QHash>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSet>
#include <QTextStream>
#include <QVBoxLayout>
#include <algorithm>
#include <limits>

#include "MapDocument.h"

namespace trizbort {

// Build a human-readable statistics report for a map, mirroring the figures and
// wording of the C# MapStatisticsView so the two read identically.
inline QString buildStatisticsReport(const Map &map)
{
    auto plur = [](int x) { return x == 1 ? QString() : QStringLiteral("s"); };

    auto objectCount = [](const Room &r) -> int {
        QString t = r.objectsText;
        t.remove(QRegularExpression(QStringLiteral("\\[[^\\]\\[]*\\]")));
        int n = 0;
        for (const QString &line : t.split(QLatin1Char('\n')))
            if (!line.trimmed().isEmpty())
                ++n;
        return n;
    };

    // A connection's endpoints in index order.
    auto sortedVerts = [](const Connection &c) {
        QList<Vertex> v = c.vertices;
        std::sort(v.begin(), v.end(), [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
        return v;
    };
    // Source/target room ids (−1 when that end isn't docked to a room).
    auto sourceRoom = [&](const Connection &c) -> int {
        const QList<Vertex> v = sortedVerts(c);
        return (!v.isEmpty() && v.first().docked) ? v.first().roomId : -1;
    };
    auto targetRoom = [&](const Connection &c) -> int {
        const QList<Vertex> v = sortedVerts(c);
        return (v.size() > 1 && v.last().docked) ? v.last().roomId : -1;
    };
    // Compass port index 0..15 (n, nne, ne, …), or −1 for a non-compass end.
    auto compassIndex = [](const QString &port) -> int {
        static const char *const names[16] = {"n",  "nne", "ne", "ene", "e",  "ese", "se", "sse",
                                               "s",  "ssw", "sw", "wsw", "w",  "wnw", "nw", "nnw"};
        const QString p = port.toLower();
        for (int i = 0; i < 16; ++i)
            if (p == QLatin1String(names[i]))
                return i;
        return -1;
    };
    auto firstPortIndex = [&](const Connection &c) -> int {
        const QList<Vertex> v = sortedVerts(c);
        return (!v.isEmpty() && v.first().docked) ? compassIndex(v.first().port) : -1;
    };
    auto secondPortIndex = [&](const Connection &c) -> int {
        const QList<Vertex> v = sortedVerts(c);
        return (v.size() > 1 && v.at(1).docked) ? compassIndex(v.at(1).port) : -1;
    };
    auto opposite = [](int idx) { return idx < 0 ? -1 : (idx + 8) % 16; };
    // Two compass directions in the same cardinal group (diagonals match only
    // themselves), matching Canvas.EqualEnough.
    auto equalEnough = [](int a, int b) -> bool {
        if (a < 0 || b < 0)
            return false;
        if (a == b)
            return true;
        auto group = [](int i) -> int {
            switch (i) {
            case 0: case 1: case 15: return 0;   // N, NNE, NNW
            case 4: case 3: case 5:  return 1;   // E, ENE, ESE
            case 8: case 7: case 9:  return 2;   // S, SSE, SSW
            case 12: case 13: case 11: return 3; // W, WNW, WSW
            default: return -1;                  // NE, SE, SW, NW
            }
        };
        const int ga = group(a);
        return ga >= 0 && ga == group(b);
    };

    const int rooms = map.rooms.size();
    const int connections = map.connections.size();

    // Rooms docked to each connection, and the connections touching each room.
    QHash<int, QVector<int>> roomConnections; // roomId -> connection indices
    for (const Room &r : map.rooms)
        roomConnections.insert(r.id, {});

    int dangling = 0, selfLoops = 0, oneWay = 0, dashed = 0;
    int doors = 0, lockedDoors = 0, lockableDoors = 0, openableDoors = 0, openDoors = 0;
    int upDown = 0, inOut = 0, labeled = 0, middleText = 0;
    int diagonal2 = 0, diagonal1 = 0, bentAll = 0, bentNoText = 0;
    for (int ci = 0; ci < map.connections.size(); ++ci) {
        const Connection &c = map.connections.at(ci);
        if (c.flow == ConnectionFlow::OneWay)
            ++oneWay;
        if (c.style == ConnectionStyle::Dashed)
            ++dashed;
        if (c.hasDoor) {
            ++doors;
            if (c.door.locked)
                ++lockedDoors;
            if (c.door.lockable)
                ++lockableDoors;
            if (c.door.openable)
                ++openableDoors;
            if (c.door.open)
                ++openDoors;
        }

        const bool hasStart = !c.startText.isEmpty();
        const bool hasEnd = !c.endText.isEmpty();
        if (hasStart || hasEnd)
            ++labeled;
        if (!c.midText.isEmpty())
            ++middleText;
        if ((c.endText == QLatin1String("up") && c.startText == QLatin1String("down"))
            || (c.endText == QLatin1String("down") && c.startText == QLatin1String("up")))
            ++upDown;
        if ((c.endText == QLatin1String("in") && c.startText == QLatin1String("out"))
            || (c.endText == QLatin1String("out") && c.startText == QLatin1String("in")))
            ++inOut;

        const int p1 = firstPortIndex(c);
        const int p2 = secondPortIndex(c);
        int diags = 0;
        if (p1 >= 0 && (p1 % 4) == 2 && !hasStart)
            ++diags;
        if (p2 >= 0 && (p2 % 4) == 2 && !hasEnd)
            ++diags;
        if (diags == 2)
            ++diagonal2;
        else if (diags == 1)
            ++diagonal1;

        if (p1 >= 0 && p2 >= 0 && !equalEnough(p1, opposite(p2))) {
            ++bentAll;
            if (!hasStart && !hasEnd)
                ++bentNoText;
        }

        const int src = sourceRoom(c);
        const int tgt = targetRoom(c);
        if (src < 0 || tgt < 0)
            ++dangling;
        if (src >= 0 && src == tgt)
            ++selfLoops;
        if (roomConnections.contains(src))
            roomConnections[src].append(ci);
        if (tgt != src && roomConnections.contains(tgt))
            roomConnections[tgt].append(ci);
    }
    // Custom connections: labelled ones that aren't the up/down or in/out pairs.
    const int customConnections = labeled - inOut - upDown;

    int dark = 0, described = 0, subtitled = 0, withObjects = 0, totalObjects = 0;
    int obj1 = 0, obj2 = 0, obj3plus = 0;
    int rect = 0, rounded = 0, ellipse = 0, octagon = 0;
    int startRoomCount = 0, endRoomCount = 0;
    QString startRoomName;
    QStringList endNames;
    for (const Room &r : map.rooms) {
        if (r.isDark)
            ++dark;
        if (!r.description.trimmed().isEmpty())
            ++described;
        if (!r.subtitle.trimmed().isEmpty())
            ++subtitled;
        const int oc = objectCount(r);
        if (oc > 0)
            ++withObjects;
        if (oc == 1)
            ++obj1;
        else if (oc == 2)
            ++obj2;
        else if (oc >= 3)
            ++obj3plus;
        totalObjects += oc;
        if (r.ellipse)
            ++ellipse;
        else if (r.octagonal)
            ++octagon;
        else if (r.roundedCorners)
            ++rounded;
        else
            ++rect;
        if (r.isStartRoom) {
            ++startRoomCount;
            if (startRoomName.isEmpty())
                startRoomName = r.name.isEmpty() ? QObject::tr("(unnamed)") : r.name;
        }
        if (r.isEndRoom) {
            ++endRoomCount;
            endNames << (r.name.isEmpty() ? QObject::tr("(unnamed)") : r.name);
        }
    }

    // Dead ends and unconnected (floating) rooms, matching the C# definitions.
    int deadEnds = 0, floating = 0;
    if (rooms == 1) {
        deadEnds = 1; // the single room is where you start, so it's a dead end
    }
    for (const Room &r : map.rooms) {
        const QVector<int> &conns = roomConnections.value(r.id);
        // Unconnected: no connections, or every non-dangling one loops on itself.
        bool allLoops = true;
        for (int ci : conns) {
            const Connection &c = map.connections.at(ci);
            const int src = sourceRoom(c);
            const int tgt = targetRoom(c);
            if (src >= 0 && tgt >= 0 && src != tgt)
                allLoops = false;
        }
        if (conns.isEmpty() || allLoops)
            ++floating;

        if (rooms != 1) {
            if (conns.isEmpty()) {
                if (r.isStartRoom)
                    ++deadEnds;
            } else {
                bool isDead = true;
                int waysIn = 0;
                for (int ci : conns) {
                    const Connection &c = map.connections.at(ci);
                    const int src = sourceRoom(c);
                    const int tgt = targetRoom(c);
                    if (src < 0 || tgt < 0)
                        continue; // dangling: no way in
                    if (c.flow == ConnectionFlow::TwoWay) {
                        if (src != tgt) {
                            isDead = false;
                            break;
                        }
                    } else if (src == r.id) {
                        isDead = false; // room starts a one-way exit
                        break;
                    } else {
                        ++waysIn;
                    }
                }
                if (isDead && waysIn > 0)
                    ++deadEnds;
            }
        }
    }

    // Duplicate room names: "Name(count), …" or "None".
    QHash<QString, int> nameCounts;
    for (const Room &r : map.rooms)
        nameCounts[r.name.toLower()] += 1;
    QStringList dupes;
    QSet<QString> dupeSeen;
    for (const Room &r : map.rooms) {
        const QString key = r.name.toLower();
        if (nameCounts.value(key) > 1 && !dupeSeen.contains(key)) {
            dupeSeen.insert(key);
            dupes << QStringLiteral("%1(%2)").arg(r.name).arg(nameCounts.value(key));
        }
    }

    // Rooms with a duplicated exit label in a given direction word.
    auto dupConnectionList = [&](const QString &word) -> QString {
        QStringList names;
        QList<Room> sorted = map.rooms;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Room &a, const Room &b) { return a.name < b.name; });
        for (const Room &r : sorted) {
            int dupCount = 0;
            for (int ci : roomConnections.value(r.id)) {
                const Connection &c = map.connections.at(ci);
                if (targetRoom(c) == r.id && c.endText == word)
                    ++dupCount;
                if (sourceRoom(c) == r.id && c.startText == word)
                    ++dupCount;
            }
            if (dupCount > 1)
                names << r.name;
        }
        if (names.isEmpty())
            return QObject::tr("No rooms with duplicate %1 exits.").arg(word);
        return QObject::tr("Rooms with duplicate %1 exits: %2.").arg(word, names.join(QStringLiteral(", ")));
    };

    // Element bounds (rooms and free connection vertices), with and without the
    // page margins.
    double minX = std::numeric_limits<double>::max(), minY = minX;
    double maxX = std::numeric_limits<double>::lowest(), maxY = maxX;
    auto expand = [&](double x, double y) {
        minX = qMin(minX, x);
        minY = qMin(minY, y);
        maxX = qMax(maxX, x);
        maxY = qMax(maxY, y);
    };
    for (const Room &r : map.rooms) {
        expand(r.x, r.y);
        expand(r.x + r.w, r.y + r.h);
    }
    for (const Connection &c : map.connections)
        for (const Vertex &v : c.vertices)
            if (!v.docked)
                expand(v.point.x(), v.point.y());
    const double rawW = maxX - minX;
    const double rawH = maxY - minY;
    double hMargin = 0.0, vMargin = 0.0;
    if (map.settings.documentSpecificMargins) {
        hMargin = map.settings.horizontalMargin;
        vMargin = map.settings.verticalMargin;
    } else {
        // The C# font-height fudge: max(subtitle height, 4x object height).
        const double subH = (map.settings.subtitleFont.size > 0 ? map.settings.subtitleFont.size : 11.0) * 1.4;
        const double objH = (map.settings.objectFont.size > 0 ? map.settings.objectFont.size : 11.0) * 1.4;
        hMargin = vMargin = qMax(subH, objH * 4.0);
    }

    QString out;
    QTextStream s(&out);
    if (rooms == 0) {
        s << QObject::tr("No rooms to take stats of.");
        return out;
    }

    s << QObject::tr("# of Rooms: %1").arg(rooms) << "\n";
    s << QObject::tr("# of Dark Rooms: %1").arg(dark) << "\n";
    s << QObject::tr("# of Unconnected Rooms: %1").arg(floating) << "\n\n";

    if (startRoomCount == 1)
        s << QObject::tr("Start room = %1").arg(startRoomName);
    else if (startRoomCount > 1)
        s << QObject::tr("More than one start room.");
    else
        s << QObject::tr("No start room.");
    s << "\n";

    if (endRoomCount == 1)
        s << QObject::tr("End room = %1").arg(endNames.first());
    else if (endRoomCount > 1)
        s << QObject::tr("End rooms = (%1)").arg(endNames.join(QStringLiteral(", ")));
    else
        s << QObject::tr("No end room.");
    s << "\n";

    s << "\n"
      << QObject::tr("Dimensions with margins: height %1, width %2")
             .arg(qRound(rawH + 2 * vMargin))
             .arg(qRound(rawW + 2 * hMargin))
      << "\n";
    s << QObject::tr("Dimensions without margins: height %1, width %2")
             .arg(qRound(rawH))
             .arg(qRound(rawW))
      << "\n\n";

    s << QObject::tr("# of Connections: %1 total, %2 unlabeled, %3 one-way, %4 dashed/dotted, "
                     "%5 up/down, %6 in/out.")
             .arg(connections)
             .arg(labeled)
             .arg(oneWay)
             .arg(dashed)
             .arg(upDown)
             .arg(inOut)
      << "\n";
    s << QObject::tr("# of Dangling Connections: %1").arg(dangling) << "\n";
    s << QObject::tr("# of Self Looping Connections: %1").arg(selfLoops) << "\n";
    s << QObject::tr("# of Dead Ends: %1").arg(deadEnds) << "\n\n";

    s << QObject::tr("%1 door%2, %3 locked, %4 lockable, %5 open, %6 openable")
             .arg(doors)
             .arg(plur(doors))
             .arg(lockedDoors)
             .arg(lockableDoors)
             .arg(openDoors)
             .arg(openableDoors)
      << "\n\n";

    // Regions.
    int namedRegions = 0;
    int withoutRegion = 0;
    for (const Room &r : map.rooms)
        if (r.region.isEmpty() || r.region == kNoRegion)
            ++withoutRegion;
    QList<Region> sortedRegions;
    for (const Region &rg : map.regions)
        if (rg.name != kNoRegion && !rg.name.isEmpty()) {
            sortedRegions << rg;
            ++namedRegions;
        }
    std::sort(sortedRegions.begin(), sortedRegions.end(),
              [](const Region &a, const Region &b) { return a.name < b.name; });
    auto roomsInRegion = [&](const QString &region) {
        int n = 0;
        for (const Room &r : map.rooms)
            if (r.region == region)
                ++n;
        return n;
    };
    s << QObject::tr("# of Regions: %1").arg(namedRegions) << "\n";
    s << QObject::tr("# of Rooms without a region: %1").arg(withoutRegion) << "\n";
    if (namedRegions > 0) {
        s << QObject::tr("Regions:") << "\n";
        for (const Region &rg : sortedRegions) {
            const int n = roomsInRegion(rg.name);
            s << QStringLiteral("%1 (%2 %3)").arg(rg.name).arg(n).arg(n == 1 ? QObject::tr("room")
                                                                             : QObject::tr("rooms"))
              << "\n";
        }
    }

    s << "\n";
    s << QObject::tr("Total # Objects in All Rooms: %1").arg(totalObjects) << "\n";
    s << QObject::tr("Total # Rooms with Objects: %1, %2 with 1, %3 with 2, %4 with 3+")
             .arg(withObjects)
             .arg(obj1)
             .arg(obj2)
             .arg(obj3plus)
      << "\n\n";

    s << QObject::tr("Total # of Rooms with subtitles: %1").arg(subtitled) << "\n";
    s << QObject::tr("Total # of Rooms with descriptions: %1").arg(described) << "\n";

    s << "\n" << QString(30, QLatin1Char('=')) << QObject::tr("Odd stuff below here") << "\n";

    s << "\n"
      << QObject::tr("Duplicate rooms: %1.")
             .arg(dupes.isEmpty() ? QObject::tr("None") : dupes.join(QStringLiteral(", ")))
      << "\n";

    s << "\n"
      << QObject::tr("Room shapes: %1 rectangular, %2 elliptical, %3 round cornered, %4 octagonal.")
             .arg(rect)
             .arg(ellipse)
             .arg(rounded)
             .arg(octagon)
      << "\n";

    if (namedRegions > 0) {
        s << "\n";
        for (const Region &rg : sortedRegions) {
            const int n = roomsInRegion(rg.name);
            if (n == 0)
                s << QObject::tr("%1 has no rooms.").arg(rg.name);
            else if (n == 1)
                s << QObject::tr("Only room in %1: ").arg(rg.name);
            else
                s << QObject::tr("List of rooms in %1: ").arg(rg.name);
            QStringList names;
            QList<Room> sortedRooms = map.rooms;
            std::sort(sortedRooms.begin(), sortedRooms.end(),
                      [](const Room &a, const Room &b) { return a.name < b.name; });
            for (const Room &r : sortedRooms)
                if (r.region == rg.name)
                    names << r.name;
            s << names.join(QStringLiteral(", ")) << "\n";
        }
        s << "\n";
    } else {
        s << "\n";
    }

    s << QObject::tr("%1 up-down connection%2").arg(upDown).arg(plur(upDown)) << "\n";
    s << QObject::tr("%1 in-out connection%2").arg(inOut).arg(plur(inOut)) << "\n";
    s << QObject::tr("%1 custom connection%2").arg(customConnections).arg(plur(customConnections)) << "\n";
    s << QObject::tr("%1 diagonal connection%2 (2-way)").arg(diagonal2).arg(plur(diagonal2)) << "\n";
    s << QObject::tr("%1 diagonal connection%2 (1-way)").arg(diagonal1).arg(plur(diagonal1)) << "\n";
    s << QObject::tr("%1 connection%2 with middle text").arg(middleText).arg(plur(middleText)) << "\n";
    s << QObject::tr("%1 bent connection%2, %3 with no text").arg(bentAll).arg(plur(bentAll)).arg(bentNoText)
      << "\n\n";

    s << dupConnectionList(QStringLiteral("in")) << "\n";
    s << dupConnectionList(QStringLiteral("out")) << "\n";
    s << dupConnectionList(QStringLiteral("up")) << "\n";
    s << dupConnectionList(QStringLiteral("down")) << "\n";

    return out;
}

// A read-only view of the statistics report with an Export-to-file button.
class MapStatisticsDialog : public QDialog {
public:
    MapStatisticsDialog(const Map &map, QWidget *parent = nullptr)
        : QDialog(parent)
        , m_report(buildStatisticsReport(map))
    {
        setWindowTitle(tr("Map Statistics"));
        resize(520, 480);
        auto *layout = new QVBoxLayout(this);
        auto *text = new QPlainTextEdit(this);
        text->setReadOnly(true);
        text->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        text->setPlainText(m_report);
        layout->addWidget(text);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
        auto *exportButton = buttons->addButton(tr("&Export…"), QDialogButtonBox::ActionRole);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);
        connect(exportButton, &QPushButton::clicked, this, [this] { exportReport(); });
        layout->addWidget(buttons);
    }

private:
    void exportReport()
    {
        const QString path = QFileDialog::getSaveFileName(
            this, tr("Export Statistics"), QStringLiteral("statistics.log"),
            tr("Log files (*.log);;Text files (*.txt);;All files (*)"));
        if (path.isEmpty())
            return;
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Export Failed"), file.errorString());
            return;
        }
        QTextStream(&file) << m_report;
    }

    QString m_report;
};

} // namespace trizbort

#endif // TRIZBORT_MAPSTATISTICSDIALOG_H
