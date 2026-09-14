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

#include "HugoExporter.h"

namespace trizbort {

namespace {

bool isNullOrWhitespace(const QString &s) { return s.trimmed().isEmpty(); }

QString stripOddCharacters(const QString &text)
{
    QString result;
    for (const QChar qc : text) {
        const ushort c = qc.unicode();
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
            || c == '_')
            result += qc;
    }
    return result;
}

QString hugoName(const QString &text, const QString &emptyDefault, std::optional<int> suffix)
{
    QString name = stripOddCharacters(text);
    if (name.isEmpty())
        name = emptyDefault;
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

QString toHugoPropertyName(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return QStringLiteral("n_to");
    case MappableDirection::South:     return QStringLiteral("s_to");
    case MappableDirection::East:      return QStringLiteral("e_to");
    case MappableDirection::West:      return QStringLiteral("w_to");
    case MappableDirection::NorthEast: return QStringLiteral("ne_to");
    case MappableDirection::NorthWest: return QStringLiteral("nw_to");
    case MappableDirection::SouthEast: return QStringLiteral("se_to");
    case MappableDirection::SouthWest: return QStringLiteral("sw_to");
    case MappableDirection::Up:        return QStringLiteral("u_to");
    case MappableDirection::Down:      return QStringLiteral("d_to");
    case MappableDirection::In:        return QStringLiteral("in_to");
    case MappableDirection::Out:       return QStringLiteral("out_to");
    default:                           return QStringLiteral("n_to");
    }
}

void exportThings(QString &out, const QList<Thing *> &things, const Thing *container)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    for (const Thing *thing : things) {
        if (thing->container != container)
            continue;
        wl(QStringLiteral("object ") + thing->exportName);
        wl(QStringLiteral("{"));
        wl(QStringLiteral("\tin ") + thing->location->exportName);
        wl(QStringLiteral("}"));
        wl();
        exportThings(out, thing->contents, thing);
    }
}

} // namespace

QStringList HugoExporter::reservedWords() const
{
    return {QStringLiteral("Room"), QStringLiteral("Actor"), QStringLiteral("Thing"),
            QStringLiteral("Object"), QStringLiteral("Door"), QStringLiteral("Chair"),
            QStringLiteral("Heavy"), QStringLiteral("Fixture"), QStringLiteral("OpenableContainer"),
            QStringLiteral("Food"), QStringLiteral("GameMainDef"), QStringLiteral("if"),
            QStringLiteral("else"), QStringLiteral("me")};
}

QString HugoExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return hugoName(room.name, QStringLiteral("room"), suffix);
}

QString HugoExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return hugoName(displayName, QStringLiteral("item"), suffix);
}

void HugoExporter::exportHeader(QString &out, const QString &title, const QString &author,
                                const QString &description, const QString &history)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("#include \"verblib.g\" ! grammar must come first"));
    wl();
    wl(QStringLiteral("#include \"hugolib.h\""));
    wl();
    wl(QStringLiteral("routine init"));
    wl(QStringLiteral("{"));
    wl(QStringLiteral("\tcls"));
    wl(QStringLiteral("\tverbosity = 2"));
    wl(QStringLiteral("\tcounter = -1"));
    wl(QStringLiteral("\tSTATUSTYPE = 1 !1 = score / turns, 2 = time, 3 = moves: score:"));
    wl(QStringLiteral("\tplayer = you"));

    if (!isNullOrWhitespace(title)) {
        wl(QStringLiteral("\tFont(BOLD_ON)"));
        wl(QStringLiteral("\t\"") + title + QLatin1Char('"'));
        wl(QStringLiteral("\tFont(BOLD_OFF)"));
    }

    if (!isNullOrWhitespace(description) && !isNullOrWhitespace(author))
        wl(QStringLiteral("\t\"") + description + QStringLiteral(", by ") + author + QLatin1Char('"'));
    else if (!isNullOrWhitespace(description))
        wl(QStringLiteral("\t\"") + description + QLatin1Char('"'));
    else if (!isNullOrWhitespace(author))
        wl(QStringLiteral("\t\"by ") + author + QLatin1Char('"'));

    if (!m_locationsInExportOrder.isEmpty()) {
        bool foundStart = false;
        for (Location *location : m_locationsInExportOrder) {
            if (location->room->isStartRoom) {
                if (foundStart)
                    wl(QStringLiteral("! ") + location->exportName
                       + QStringLiteral(" is an extra StartRoom. "));
                wl(QStringLiteral("\tlocation = ") + location->exportName);
                foundStart = true;
            }
        }
        if (!foundStart)
            wl(QStringLiteral("\tlocation = ") + m_locationsInExportOrder.first()->exportName);
    } else {
        wl(QStringLiteral("\t! location = ... "));
    }

    if (!isNullOrWhitespace(history)) {
        wl();
        wl(QStringLiteral("    showAbout()"));
        wl(QStringLiteral("    {"));
        wl(QStringLiteral("    \"") + history + QStringLiteral("\";"));
        wl(QStringLiteral("    }"));
    }

    wl(QStringLiteral("\tMovePlayer(location)"));
    wl(QStringLiteral("}"));
    wl();
}

void HugoExporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    if (!m_regionsInExportOrder.isEmpty())
        wl(QStringLiteral("property region"));

    for (Location *location : m_locationsInExportOrder) {
        wl(QStringLiteral("room ") + location->exportName);
        wl(QStringLiteral("{"));

        if (location->room->region != QLatin1String("NoRegion"))
            wl(QStringLiteral("\tregion ") + location->room->region);

        if (!location->room->description.isEmpty()) {
            wl(QStringLiteral("\tlong_desc"));
            wl(QStringLiteral("\t\""));
            wl(QStringLiteral("\t\t") + location->room->description);
            wl(QStringLiteral("\t\""));
        }

        wl();
        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit != nullptr)
                wl(QStringLiteral("\t") + toHugoPropertyName(direction) + QLatin1Char(' ')
                   + exit->target->exportName);
        }

        wl();
        if (location->room->isDark)
            wl(QStringLiteral("\tis not light"));
        wl(QStringLiteral("}"));
        wl();

        exportThings(out, location->things, nullptr);
    }

    wl(QStringLiteral("player_character you \"you\""));
    wl(QStringLiteral("{"));
    wl(QStringLiteral("}"));
    wl();
}

} // namespace trizbort
