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

#include "QuestExporter.h"

#include <charconv>

#include <QDate>
#include <QUuid>

namespace trizbort {

namespace {

QString stripOdd(const QString &text)
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

QString questName(const QString &text, std::optional<int> suffix)
{
    QString name = stripOdd(text);
    if (name.isEmpty())
        name = QStringLiteral("item");
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

QString toQuestPropertyName(MappableDirection direction)
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

// Reproduces System.Drawing.ColorTranslator.ToHtml for the values Trizbort
// stores. Room fill/border colours are loaded from the map file as #RRGGBB
// hex (never named colours) or left at their default Color.Transparent. So an
// unset (invalid) colour renders as the named default "Transparent"; a set
// colour renders as uppercase "#RRGGBB".
QString colorToHtml(const QColor &c)
{
    if (!c.isValid())
        return QStringLiteral("Transparent");
    return QStringLiteral("#")
        + QStringLiteral("%1%2%3")
              .arg(c.red(), 2, 16, QLatin1Char('0'))
              .arg(c.green(), 2, 16, QLatin1Char('0'))
              .arg(c.blue(), 2, 16, QLatin1Char('0'))
              .toUpper();
}

// Reproduces the C# expression `location.Room.Width / 32` formatted with the
// default float ToString(): Width is a single-precision float and 32 an int,
// so the division is done in float and printed with the shortest round-tripping
// representation (e.g. "2", "4.375", "1.5"). std::to_chars gives the same
// shortest form as .NET Core's float formatter.
QString formatGrid(double dimension)
{
    const float value = static_cast<float>(dimension) / 32.0f;
    char buf[64];
    const std::to_chars_result res = std::to_chars(buf, buf + sizeof(buf), value);
    return QString::fromLatin1(buf, static_cast<int>(res.ptr - buf));
}

} // namespace

QStringList QuestExporter::reservedWords() const
{
    return {QStringLiteral("object"), QStringLiteral("game"), QStringLiteral("turnscript")};
}

QString QuestExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return questName(room.name, suffix);
}

QString QuestExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return questName(displayName, suffix);
}

void QuestExporter::exportHeader(QString &out, const QString &title, const QString &author,
                                 const QString &description, const QString &history)
{
    Q_UNUSED(history);
    if (!includeWrapper())
        return;
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("<asl version=\"550\">"));
    wl();
    wl(QStringLiteral("  <include ref=\"English.aslx\"/>"));
    wl(QStringLiteral("  <include ref=\"Core.aslx\"/>"));
    wl(QStringLiteral("  <game name=\"") + title + QStringLiteral("\">"));
    wl(QStringLiteral("    <gameid>")
       + QUuid::createUuid().toString(QUuid::WithoutBraces) + QStringLiteral("</gameid>"));
    wl(QStringLiteral("    <version>1.0</version>"));
    wl(QStringLiteral("    <firstpublished>") + QString::number(QDate::currentDate().year())
       + QStringLiteral("</firstpublished>"));
    wl(QStringLiteral("    <author>") + author + QStringLiteral("</author>"));
    wl(QStringLiteral("    <description>") + description + QStringLiteral("</description>"));
    wl(QStringLiteral("  </game>"));
    wl();
}

void QuestExporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    for (Location *location : m_locationsInExportOrder) {
        const Room *room = location->room;
        wl(QStringLiteral("  <object name=\"") + location->exportName + QStringLiteral("\">"));
        wl(QStringLiteral("    <inherit name=\"editor_room\" />"));
        wl(QStringLiteral("    <alias>") + room->name + QStringLiteral("</alias>"));
        if (room->isDark)
            wl(QStringLiteral("    <dark />"));
        wl(QStringLiteral("    <attr name=\"grid_width\" type=\"int\">") + formatGrid(room->w)
           + QStringLiteral("</attr>"));
        wl(QStringLiteral("    <attr name=\"grid_length\" type=\"int\">") + formatGrid(room->h)
           + QStringLiteral("</attr>"));
        wl(QStringLiteral("    <attr name=\"grid_fill\">") + colorToHtml(room->fill)
           + QStringLiteral("</attr>"));
        wl(QStringLiteral("    <attr name=\"grid_border\">") + colorToHtml(room->border)
           + QStringLiteral("</attr>"));
        wl(QStringLiteral("    <attr name=\"implementation_notes\">") + room->description
           + QStringLiteral("</attr>"));
        if (!room->description.isEmpty())
            wl(QStringLiteral("    <description>") + room->description
               + QStringLiteral("</description>"));

        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit != nullptr) {
                const QString dir = toQuestPropertyName(direction);
                wl(QStringLiteral("    <exit alias=\"") + dir + QStringLiteral("\" to=\"")
                   + exit->target->exportName + QStringLiteral("\">"));
                wl(QStringLiteral("      <inherit name=\"") + dir
                   + QStringLiteral("direction\" />"));
                wl(QStringLiteral("    </exit>"));
            }
        }

        if (room->isStartRoom) {
            wl(QStringLiteral("    <object name=\"player\">"));
            wl(QStringLiteral("      <inherit name=\"editor_object\" />"));
            wl(QStringLiteral("      <inherit name=\"editor_player\" />"));
            wl(QStringLiteral("    </object>"));
        }

        for (const Thing *thing : location->things) {
            wl(QStringLiteral("    <object name=\"") + thing->exportName + QStringLiteral("\">"));
            wl(QStringLiteral("      <inherit name=\"editor_object\" />"));
            if (thing->isScenery)
                wl(QStringLiteral("      <scenery />"));
            if (thing->isContainer) {
                wl(QStringLiteral("      <feature_container />"));
                wl(QStringLiteral("      <inherit name=\"container_closed\" />"));
            }
            if (thing->forceplural == Thing::Amounts::Plural)
                wl(QStringLiteral("      <inherit name=\"plural\" />"));
            if (thing->isPerson) {
                if (thing->gender == Thing::Gender::Female) {
                    if (thing->properNamed)
                        wl(QStringLiteral("      <inherit name=\"namedfemale\" />"));
                    else
                        wl(QStringLiteral("      <inherit name=\"female\" />"));
                } else if (thing->gender == Thing::Gender::Male) {
                    if (thing->properNamed)
                        wl(QStringLiteral("      <inherit name=\"namedmale\" />"));
                    else
                        wl(QStringLiteral("      <inherit name=\"male\" />"));
                }
            }
            wl(QStringLiteral("      <alias>") + thing->displayName + QStringLiteral("</alias>"));
            wl(QStringLiteral("    </object>"));
        }

        wl(QStringLiteral("  </object>"));
        wl();
    }

    wl();

    if (includeWrapper()) {
        wl(QStringLiteral("</asl>"));
        wl();
    }
}

} // namespace trizbort
