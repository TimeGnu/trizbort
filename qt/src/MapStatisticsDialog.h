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
#include <limits>

#include "MapDocument.h"

namespace trizbort {

// Build a human-readable statistics report for a map, mirroring the figures
// shown by the C# MapStatisticsView.
inline QString buildStatisticsReport(const Map &map)
{
    auto objectCount = [](const Room &r) -> int {
        QString t = r.objectsText;
        t.remove(QRegularExpression(QStringLiteral("\\[[^\\]\\[]*\\]")));
        int n = 0;
        for (const QString &line : t.split(QLatin1Char('\n')))
            if (!line.trimmed().isEmpty())
                ++n;
        return n;
    };

    const int rooms = map.rooms.size();
    const int connections = map.connections.size();

    // Per-room connection counts and endpoint bookkeeping.
    QHash<int, int> connectionsPerRoom;
    for (const Room &r : map.rooms)
        connectionsPerRoom.insert(r.id, 0);
    int dangling = 0;
    int selfLoops = 0;
    int oneWay = 0;
    int dashed = 0;
    int doors = 0;
    int lockedDoors = 0;
    int lockableDoors = 0;
    int openableDoors = 0;
    int openDoors = 0;
    for (const Connection &c : map.connections) {
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
        QSet<int> dockedRooms;
        int docked = 0;
        bool hasFree = false;
        for (const Vertex &v : c.vertices) {
            if (v.docked) {
                ++docked;
                dockedRooms.insert(v.roomId);
                if (connectionsPerRoom.contains(v.roomId))
                    connectionsPerRoom[v.roomId] += 1;
            } else {
                hasFree = true;
            }
        }
        if (hasFree)
            ++dangling;
        if (docked >= 2 && dockedRooms.size() == 1)
            ++selfLoops;
    }

    int dark = 0, described = 0, subtitled = 0, withObjects = 0, totalObjects = 0;
    int rect = 0, rounded = 0, ellipse = 0, octagon = 0, deadEnds = 0, floating = 0;
    QStringList startRooms, endRooms;
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
        totalObjects += oc;
        if (r.ellipse)
            ++ellipse;
        else if (r.octagonal)
            ++octagon;
        else if (r.roundedCorners)
            ++rounded;
        else
            ++rect;
        const int cc = connectionsPerRoom.value(r.id, 0);
        if (cc == 0)
            ++floating;
        else if (cc == 1)
            ++deadEnds;
        if (r.isStartRoom)
            startRooms << (r.name.isEmpty() ? QObject::tr("(unnamed)") : r.name);
        if (r.isEndRoom)
            endRooms << (r.name.isEmpty() ? QObject::tr("(unnamed)") : r.name);
    }

    // Duplicate room names.
    QHash<QString, int> nameCounts;
    for (const Room &r : map.rooms)
        nameCounts[r.name.toLower()] += 1;
    QStringList dupes;
    for (const Room &r : map.rooms) {
        if (!r.name.isEmpty() && nameCounts.value(r.name.toLower()) > 1 &&
            !dupes.contains(QStringLiteral("%1 (%2)").arg(r.name).arg(nameCounts.value(r.name.toLower()))))
            dupes << QStringLiteral("%1 (%2)").arg(r.name).arg(nameCounts.value(r.name.toLower()));
    }

    // Dimensions.
    double minX = std::numeric_limits<double>::max(), minY = minX;
    double maxX = std::numeric_limits<double>::lowest(), maxY = maxX;
    for (const Room &r : map.rooms) {
        minX = qMin(minX, r.x);
        minY = qMin(minY, r.y);
        maxX = qMax(maxX, r.x + r.w);
        maxY = qMax(maxY, r.y + r.h);
    }

    QString out;
    QTextStream s(&out);
    s << QObject::tr("Map statistics") << "\n";
    s << "==============\n\n";
    if (rooms == 0) {
        s << QObject::tr("No rooms to take statistics of.") << "\n";
        return out;
    }

    s << QObject::tr("Rooms: %1").arg(rooms) << "\n";
    s << QObject::tr("Connections: %1").arg(connections) << "\n";
    s << QObject::tr("Dimensions: %1 wide x %2 high")
             .arg(qRound(maxX - minX))
             .arg(qRound(maxY - minY))
      << "\n\n";

    s << QObject::tr("Start room: %1")
             .arg(startRooms.isEmpty() ? QObject::tr("(none)") : startRooms.join(QStringLiteral(", ")))
      << "\n";
    s << QObject::tr("End room: %1")
             .arg(endRooms.isEmpty() ? QObject::tr("(none)") : endRooms.join(QStringLiteral(", ")))
      << "\n\n";

    s << QObject::tr("Dark rooms: %1").arg(dark) << "\n";
    s << QObject::tr("Rooms with a description: %1").arg(described) << "\n";
    s << QObject::tr("Rooms with a subtitle: %1").arg(subtitled) << "\n";
    s << QObject::tr("Rooms with objects: %1 (%2 objects total)").arg(withObjects).arg(totalObjects)
      << "\n";
    s << QObject::tr("Dead ends (one exit): %1").arg(deadEnds) << "\n";
    s << QObject::tr("Floating rooms (no connections): %1").arg(floating) << "\n\n";

    s << QObject::tr("Room shapes: %1 rectangular, %2 rounded, %3 elliptical, %4 octagonal")
             .arg(rect)
             .arg(rounded)
             .arg(ellipse)
             .arg(octagon)
      << "\n\n";

    s << QObject::tr("One-way connections: %1").arg(oneWay) << "\n";
    s << QObject::tr("Dashed connections: %1").arg(dashed) << "\n";
    s << QObject::tr("Dangling connections: %1").arg(dangling) << "\n";
    s << QObject::tr("Self-looping connections: %1").arg(selfLoops) << "\n";
    s << QObject::tr("Doors: %1 (%2 lockable, %3 locked, %4 openable, %5 open)")
             .arg(doors)
             .arg(lockableDoors)
             .arg(lockedDoors)
             .arg(openableDoors)
             .arg(openDoors)
      << "\n\n";

    // Regions.
    int namedRegions = 0;
    QStringList regionLines;
    int withoutRegion = 0;
    for (const Room &r : map.rooms)
        if (r.region.isEmpty() || r.region == kNoRegion)
            ++withoutRegion;
    for (const Region &rg : map.regions) {
        if (rg.name == kNoRegion || rg.name.isEmpty())
            continue;
        ++namedRegions;
        int count = 0;
        for (const Room &r : map.rooms)
            if (r.region == rg.name)
                ++count;
        regionLines << QObject::tr("  %1: %2 room(s)").arg(rg.name).arg(count);
    }
    s << QObject::tr("Regions: %1").arg(namedRegions) << "\n";
    for (const QString &line : regionLines)
        s << line << "\n";
    s << QObject::tr("  Rooms without a region: %1").arg(withoutRegion) << "\n\n";

    if (!dupes.isEmpty())
        s << QObject::tr("Duplicate room names: %1").arg(dupes.join(QStringLiteral(", "))) << "\n";
    else
        s << QObject::tr("No duplicate room names.") << "\n";

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
