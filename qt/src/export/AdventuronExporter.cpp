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

#include "AdventuronExporter.h"

namespace trizbort {

namespace {

const int MaximumHeaderLength = 25;

QString escapeAdventuronId(const QString &input)
{
    QString sb;
    for (const QChar qc : input) {
        const ushort c = qc.unicode();
        if (qc == QLatin1Char(' ') || qc == QLatin1Char('_')) {
            if (!sb.isEmpty() && sb.at(sb.size() - 1) != QLatin1Char('_'))
                sb += QLatin1Char('_');
        } else if (qc == QLatin1Char('\'')) {
            // dropped
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
                   || (c >= 160 && c <= 8231) || (c >= 8234 && c <= 55295)
                   || (c >= 57344 && c <= 65533)) {
            sb += qc;
        }
    }
    return sb;
}

QString escapeAdventuronText(const QString &input)
{
    QString sb;
    for (const QChar c : input) {
        if (c == QLatin1Char('\n')) sb += QLatin1String("\\n");
        else if (c == QLatin1Char('\r')) { /* ignore */ }
        else if (c == QLatin1Char('$')) sb += QLatin1String("$$");
        else if (c == QLatin1Char('\\')) sb += QLatin1String("\\\\");
        else if (c == QLatin1Char('[')) sb += QLatin1String("[[");
        else if (c == QLatin1Char(']')) sb += QLatin1String("]]");
        else if (c == QLatin1Char('{')) sb += QLatin1String("{{");
        else if (c == QLatin1Char('}')) sb += QLatin1String("}}");
        else if (c == QLatin1Char('"')) sb += QLatin1String("\\\"");
        else if (c == QLatin1Char('~')) sb += QLatin1String("~~");
        else if (c == QLatin1Char('<')) sb += QLatin1String("<<");
        else if (c == QLatin1Char('>')) sb += QLatin1String(">>");
        else sb += c;
    }
    return sb;
}

QString padRight(const QString &s, int maxLen)
{
    return maxLen > s.size() ? s + QString(maxLen - s.size(), QLatin1Char(' ')) : s;
}

QString adventuronName(const QString &displayName, std::optional<int> suffix)
{
    QString name = displayName.toLower();
    name.replace(QLatin1String(" "), QLatin1String("_"));
    name.replace(QLatin1String("."), QLatin1String("_"));
    name.replace(QLatin1String("$"), QLatin1String("_"));
    if (name.isEmpty())
        name = QStringLiteral("location");
    if (suffix.has_value())
        name = name + QLatin1Char('_') + QString::number(suffix.value());
    return name;
}

QString toAdventuronDirectionName(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return QStringLiteral("north_oneway");
    case MappableDirection::South:     return QStringLiteral("south_oneway");
    case MappableDirection::East:      return QStringLiteral("east_oneway");
    case MappableDirection::West:      return QStringLiteral("west_oneway");
    case MappableDirection::NorthEast: return QStringLiteral("northeast_oneway");
    case MappableDirection::NorthWest: return QStringLiteral("northwest_oneway");
    case MappableDirection::SouthEast: return QStringLiteral("southeast_oneway");
    case MappableDirection::SouthWest: return QStringLiteral("southwest_oneway");
    case MappableDirection::Up:        return QStringLiteral("up_oneway");
    case MappableDirection::Down:      return QStringLiteral("down_oneway");
    case MappableDirection::In:        return QStringLiteral("enter_oneway");
    case MappableDirection::Out:       return QStringLiteral("leave_oneway");
    default:                           return QString();
    }
}

} // namespace

QStringList AdventuronExporter::reservedWords() const
{
    return {QStringLiteral("ether"), QStringLiteral("objects"), QStringLiteral("inventory"),
            QStringLiteral("root"), QStringLiteral("player")};
}

QString AdventuronExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return adventuronName(room.name, suffix);
}

QString AdventuronExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return adventuronName(displayName, suffix);
}

void AdventuronExporter::exportHeader(QString &, const QString &, const QString &, const QString &,
                                      const QString &)
{
    // Adventuron discards metadata.
}

void AdventuronExporter::exportContent(QString &out)
{
    QString locations = QStringLiteral("\nlocations {\n");
    QString connections = QStringLiteral("\nconnections {\n   from, direction, to = [\n");

    QString startRoom;
    bool isFirst = true;

    int maxLen = -1;
    for (Location *loc : m_locationsInExportOrder) {
        if (loc->exportName.size() > maxLen)
            maxLen = escapeAdventuronId(loc->exportName).size();
    }

    for (Location *location : m_locationsInExportOrder) {
        if (isFirst || location->room->isStartRoom)
            startRoom = location->exportName;

        const QString roomDescription = location->room->description.isEmpty()
            ? QString()
            : escapeAdventuronText(location->room->description);
        QString locationRoomName = location->room->name;
        if (locationRoomName.size() > MaximumHeaderLength)
            locationRoomName = locationRoomName.left(MaximumHeaderLength);
        const QString headerDescNormalized = escapeAdventuronText(locationRoomName);

        locations += QStringLiteral("   ")
                     + padRight(escapeAdventuronId(location->exportName), maxLen)
                     + QStringLiteral(" : location \"") + roomDescription
                     + QStringLiteral("\" header = \"") + headerDescNormalized
                     + QStringLiteral("\";\n");

        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit != nullptr) {
                connections += QStringLiteral("      ")
                               + escapeAdventuronId(location->exportName) + QStringLiteral(", ")
                               + toAdventuronDirectionName(direction) + QStringLiteral(", ")
                               + escapeAdventuronId(exit->target->exportName) + QStringLiteral(",\n");
            }
        }
        isFirst = false;
    }

    connections += QStringLiteral("   ]\n}\n");
    locations += QStringLiteral("}\n");

    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    wl();
    wl(QStringLiteral("start_at = ") + startRoom);
    wl(locations);
    wl(connections);
    wl(QString());
}

} // namespace trizbort
