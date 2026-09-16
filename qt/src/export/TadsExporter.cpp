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

#include "TadsExporter.h"

#include "../AppSettings.h"

namespace trizbort {

namespace {

// The TADS target: adv3Lite (the app default, and how the golden corpus was
// generated) or traditional adv3, per the application settings.
bool adv3Lite() { return AppSettings::instance().saveTadsToAdv3Lite; }

bool isNullOrWhitespace(const QString &s) { return s.trimmed().isEmpty(); }

QString stripOdd(const QString &text, bool keepSpaceDash)
{
    QString result;
    for (const QChar qc : text) {
        const ushort c = qc.unicode();
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'
            || (keepSpaceDash && (qc == QLatin1Char(' ') || qc == QLatin1Char('-'))))
            result += qc;
    }
    return result;
}

QString tadsName(const QString &text, const QString &emptyDefault, std::optional<int> suffix)
{
    QString name = stripOdd(text, false);
    if (name.isEmpty())
        name = emptyDefault;
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

QString toTadsString(const QString &text, QChar quote)
{
    QString r = text;
    r.replace(QString(quote), QStringLiteral("\\") + quote);
    return quote + r + quote;
}

QString toTadsPropertyName(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return QStringLiteral("north");
    case MappableDirection::South:     return QStringLiteral("south");
    case MappableDirection::East:      return QStringLiteral("east");
    case MappableDirection::West:      return QStringLiteral("west");
    case MappableDirection::NorthEast: return QStringLiteral("northeast");
    case MappableDirection::NorthWest: return QStringLiteral("northwest");
    case MappableDirection::SouthEast: return QStringLiteral("southeast");
    case MappableDirection::SouthWest: return QStringLiteral("southwest");
    case MappableDirection::Up:        return QStringLiteral("up");
    case MappableDirection::Down:      return QStringLiteral("down");
    case MappableDirection::In:        return QStringLiteral("in");
    case MappableDirection::Out:       return QStringLiteral("out");
    default:                           return QStringLiteral("north");
    }
}

void exportThings(QString &out, const QList<Thing *> &things, const Thing *container, int indent)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    for (const Thing *thing : things) {
        if (thing->container != container)
            continue;
        const QString cls = thing->contents.isEmpty() ? QStringLiteral("Thing")
                                                       : QStringLiteral("Container");
        const QString vocab =
            toTadsString(stripOdd(thing->displayName, true).trimmed(), QLatin1Char('\''));
        wl(QString(indent, QLatin1Char('+')) + QLatin1Char(' ') + thing->exportName
           + QStringLiteral(": ") + cls + QLatin1Char(' ') + vocab + QLatin1Char(' ') + vocab);
        wl(QStringLiteral(";"));
        wl();
        exportThings(out, thing->contents, thing, indent + 1);
    }
}

} // namespace

QStringList TadsExporter::reservedWords() const
{
    return {QStringLiteral("Room"), QStringLiteral("Actor"), QStringLiteral("Thing"),
            QStringLiteral("Object"), QStringLiteral("Door"), QStringLiteral("Chair"),
            QStringLiteral("Heavy"), QStringLiteral("Fixture"), QStringLiteral("OpenableContainer"),
            QStringLiteral("Food"), QStringLiteral("GameMainDef"), QStringLiteral("if"),
            QStringLiteral("else"), QStringLiteral("me")};
}

QString TadsExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return tadsName(room.name, QStringLiteral("room"), suffix);
}

QString TadsExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return tadsName(displayName, QStringLiteral("item"), suffix);
}

void TadsExporter::exportHeader(QString &out, const QString &title, const QString &author,
                                const QString &description, const QString &history)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("#charset \"us-ascii\""));
    wl();
    if (adv3Lite()) {
        wl(QStringLiteral("#include <tads.h>"));
        wl(QStringLiteral("#include \"advlite.h\""));
    } else {
        wl(QStringLiteral("#include <adv3.h>"));
        wl(QStringLiteral("#include <en_us.h>"));
    }
    wl();
    wl(QStringLiteral("versionInfo : GameID"));
    wl(QStringLiteral("    name = ") + toTadsString(title, QLatin1Char('\'')));
    wl(QStringLiteral("    byline = ")
       + toTadsString(QStringLiteral("By ") + author, QLatin1Char('\'')));
    wl(QStringLiteral("    version = '1'"));
    wl(QStringLiteral("    desc = ") + toTadsString(description, QLatin1Char('\'')));
    if (!isNullOrWhitespace(history)) {
        wl();
        wl(QStringLiteral("    showAbout()"));
        wl(QStringLiteral("    {"));
        wl(QStringLiteral("    \"") + history + QStringLiteral("\";"));
        wl(QStringLiteral("    }"));
    }
    wl(QStringLiteral(";"));
    wl();
}

void TadsExporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    if (adv3Lite()) {
        for (const ExportRegion *region : m_regionsInExportOrder) {
            wl(region->exportName + QStringLiteral(": Region"));
            wl(QStringLiteral(";"));
            wl();
        }
    }

    for (Location *location : m_locationsInExportOrder) {
        wl(location->exportName + QStringLiteral(": ")
           + (location->room->isDark ? QStringLiteral("DarkRoom") : QStringLiteral("Room"))
           + QLatin1Char(' ') + toTadsString(location->room->name, QLatin1Char('\'')));
        if (!location->room->description.isEmpty())
            wl(QStringLiteral("    ")
               + toTadsString(location->room->description, QLatin1Char('"')));
        if (adv3Lite() && location->room->region != QLatin1String("NoRegion"))
            wl(QStringLiteral("    regions = [") + location->room->region + QLatin1Char(']'));

        bool anyExits = false;
        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit != nullptr) {
                if (!anyExits) {
                    wl();
                    anyExits = true;
                }
                wl(QStringLiteral("    ") + toTadsPropertyName(direction) + QStringLiteral(" = ")
                   + exit->target->exportName);
            }
        }

        wl(QStringLiteral(";"));
        wl();

        exportThings(out, location->things, nullptr, 1);
    }

    wl(QStringLiteral("me: Actor"));
    if (!m_locationsInExportOrder.isEmpty()) {
        bool foundStart = false;
        for (Location *location : m_locationsInExportOrder) {
            if (location->room->isStartRoom) {
                if (foundStart)
                    wl(QStringLiteral("/( ") + location->exportName
                       + QStringLiteral(" is an extra StartRoom. /*"));
                wl(QStringLiteral("    location = ") + location->exportName);
                foundStart = true;
            }
        }
        if (!foundStart)
            wl(QStringLiteral("    location = ") + m_locationsInExportOrder.first()->exportName);
    } else {
        wl(QStringLiteral("    /* location = ... */"));
    }
    wl(QStringLiteral(";"));
    wl();

    wl(QStringLiteral("gameMain: GameMainDef"));
    wl(QStringLiteral("    initialPlayerChar = me"));
    wl(QStringLiteral(";"));
    wl();
}

} // namespace trizbort
