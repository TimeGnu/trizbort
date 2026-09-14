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

#include "CodeExporter.h"

#include <algorithm>

#include <QFileInfo>
#include <QRegularExpression>

namespace trizbort {

CodeExporter::CodeExporter(const Map &map, const QString &sourcePath)
    : m_map(map), m_sourcePath(sourcePath)
{
}

CodeExporter::~CodeExporter()
{
    qDeleteAll(m_ownedExits);
    qDeleteAll(m_ownedThings);
    qDeleteAll(m_ownedLocations);
    qDeleteAll(m_ownedRegions);
}

QString CodeExporter::deaccent(const QString &s)
{
    QString x;
    x.reserve(s.size());
    for (const QChar qc : s) {
        const ushort c = qc.unicode();
        if (c >= 0x00E0 && c <= 0x00E5) x += QLatin1Char('a');
        else if (c >= 0x00C0 && c <= 0x00C5) x += QLatin1Char('A');
        else if (c == 0x00C7) x += QLatin1Char('C');
        else if (c == 0x00E7) x += QLatin1Char('c');
        else if (c >= 0x00E8 && c <= 0x00EB) x += QLatin1Char('e');
        else if (c >= 0x00C8 && c <= 0x00CB) x += QLatin1Char('E');
        else if (c >= 0x00EC && c <= 0x00EF) x += QLatin1Char('i');
        else if (c >= 0x00CC && c <= 0x00CF) x += QLatin1Char('I');
        else if (c == 0x00F1) x += QLatin1Char('n');
        else if (c == 0x00D1) x += QLatin1Char('N');
        else if (c >= 0x00D2 && c <= 0x00D6) x += QLatin1Char('o'); // C# quirk: upper -> lower
        else if (c >= 0x00F2 && c <= 0x00F6) x += QLatin1Char('O'); // C# quirk: lower -> upper
        else if (c >= 0x00F9 && c <= 0x00FC) x += QLatin1Char('u');
        else if (c >= 0x00D9 && c <= 0x00DC) x += QLatin1Char('U');
        else x += qc;
    }
    return x;
}

QString CodeExporter::exportToString()
{
    QString title = m_map.title;
    if (title.isEmpty()) {
        title = QFileInfo(m_sourcePath).completeBaseName();
        if (title.isEmpty())
            title = QStringLiteral("A Trizbort Map");
    }

    QString author = m_map.author;
    if (author.isEmpty())
        author = QStringLiteral("A Trizbort User");

    const QString history = m_map.history;

    prepareContent();

    QString out;
    exportHeader(out, title, author, m_map.description, history);
    exportContent(out);
    return out;
}

void CodeExporter::prepareContent()
{
    findRegions();
    findRooms();
    findExits();
    pickBestExits();
    findThings();
}

void CodeExporter::findRegions()
{
    QSet<QString> used;
    for (const QString &word : reservedWords())
        used.insert(word.toLower());

    for (const Region &region : m_map.regions) {
        if (region.name == QLatin1String("NoRegion"))
            continue;
        QString exportName = getExportName(region.name, std::nullopt);
        if (exportName.isEmpty())
            exportName = QStringLiteral("region");
        int index = 2;
        while (used.contains(exportName.toLower()))
            exportName = getExportName(region.name, index++);
        used.insert(exportName.toLower());
        auto *er = new ExportRegion{region, exportName};
        m_ownedRegions.append(er);
        m_regionsInExportOrder.append(er);
    }
}

void CodeExporter::findRooms()
{
    QSet<QString> used;
    for (const QString &word : reservedWords())
        used.insert(word.toLower());
    for (const ExportRegion *er : m_regionsInExportOrder)
        used.insert(er->exportName.toLower());

    for (const Room &room : m_map.rooms) {
        QString exportName = getExportName(room, std::nullopt);
        if (exportName.isEmpty())
            exportName = QStringLiteral("object");
        int index = 2;
        while (used.contains(exportName.toLower()))
            exportName = getExportName(room, index++);
        used.insert(exportName.toLower());

        auto *loc = new Location(&room, exportName);
        m_ownedLocations.append(loc);
        m_locationsInExportOrder.append(loc);
        m_roomIdToLocation.insert(room.id, loc);
    }
}

const Room *CodeExporter::sourceRoom(const Connection &c, CompassPoint &cp) const
{
    QList<Vertex> v = c.vertices;
    std::sort(v.begin(), v.end(), [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
    if (!v.isEmpty() && v.first().docked && compassPointFromPort(v.first().port, cp))
        return m_map.roomById(v.first().roomId);
    cp = CompassPoint::North;
    return nullptr;
}

const Room *CodeExporter::targetRoom(const Connection &c, CompassPoint &cp) const
{
    QList<Vertex> v = c.vertices;
    std::sort(v.begin(), v.end(), [](const Vertex &a, const Vertex &b) { return a.index < b.index; });
    if (v.size() > 1 && v.last().docked && compassPointFromPort(v.last().port, cp))
        return m_map.roomById(v.last().roomId);
    cp = CompassPoint::North;
    return nullptr;
}

void CodeExporter::findExits()
{
    for (const Connection &connection : m_map.connections) {
        CompassPoint sourceCp = CompassPoint::North;
        CompassPoint targetCp = CompassPoint::North;
        const Room *src = sourceRoom(connection, sourceCp);
        const Room *tgt = targetRoom(connection, targetCp);

        if (src == nullptr || tgt == nullptr)
            continue;
        if (src == tgt && sourceCp == targetCp)
            continue;

        Location *srcLoc = m_roomIdToLocation.value(src->id, nullptr);
        Location *tgtLoc = m_roomIdToLocation.value(tgt->id, nullptr);
        if (srcLoc == nullptr || tgtLoc == nullptr)
            continue;

        auto *e1 = new Exit(srcLoc, tgtLoc, sourceCp, connection.startText, connection);
        m_ownedExits.append(e1);
        srcLoc->addExit(e1);

        if (connection.flow == ConnectionFlow::TwoWay) {
            auto *e2 = new Exit(tgtLoc, srcLoc, targetCp, connection.endText, connection);
            m_ownedExits.append(e2);
            tgtLoc->addExit(e2);
        }
    }
}

void CodeExporter::pickBestExits()
{
    for (Location *loc : m_locationsInExportOrder)
        loc->pickBestExits();
}

void CodeExporter::findThings()
{
    QSet<QString> used;
    for (const QString &word : reservedWords())
        used.insert(word.toLower());
    for (const Location *loc : m_locationsInExportOrder)
        used.insert(loc->exportName.toLower());
    for (const ExportRegion *er : m_regionsInExportOrder)
        used.insert(er->exportName.toLower());

    const QRegularExpression bracket(QStringLiteral("\\[[^\\]\\[]*\\]"));
    const QRegularExpression throughLastOpen(QStringLiteral(".*\\["));
    const QRegularExpression fromFirstClose(QStringLiteral("\\].*"));

    for (Location *loc : m_locationsInExportOrder) {
        const QString objectsText = loc->room->objectsText;
        if (objectsText.isEmpty())
            continue;

        QString normalized = objectsText;
        normalized.remove(QLatin1Char('\r'));
        const QStringList objectNames = normalized.split(QLatin1Char('\n'), Qt::SkipEmptyParts);

        for (const QString &objectName : objectNames) {
            QString displayName = objectName.trimmed();
            QString propString;

            const QRegularExpressionMatch match = bracket.match(displayName);
            if (match.hasMatch()) {
                propString = displayName;
                displayName.remove(bracket);
                propString.remove(throughLastOpen);
                propString.remove(fromFirstClose);
            }

            if (displayName.isEmpty())
                continue;

            QString exportName = getExportName(displayName, std::nullopt);
            int index = 2;
            while (used.contains(exportName.toLower()))
                exportName = getExportName(displayName, index++);
            used.insert(exportName.toLower());

            int indent = 0;
            while (indent < objectName.size() && objectName.at(indent) == QLatin1Char(' '))
                ++indent;

            Thing *container = nullptr;
            for (int i = loc->things.size() - 1; i >= 0; --i) {
                Thing *prior = loc->things.at(i);
                if (indent > prior->indent) {
                    container = prior;
                    break;
                }
            }

            auto *thing = new Thing;
            thing->displayName = displayName;
            thing->exportName = exportName;
            thing->location = loc;
            thing->container = container;
            thing->indent = indent;
            thing->propString = propString;
            m_ownedThings.append(thing);
            if (container != nullptr)
                container->contents.append(thing);
            loc->things.append(thing);
        }
    }
}

} // namespace trizbort
