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

#include "AlanExporter.h"

namespace trizbort {

namespace {

bool isNullOrWhitespace(const QString &s) { return s.trimmed().isEmpty(); }

bool containsOddCharacters(const QString &text)
{
    for (const QChar c : text)
        if (c != QLatin1Char(' ') && c != QLatin1Char('-') && !c.isLetterOrNumber())
            return true;
    return false;
}

QString stripOddCharacters(const QString &text)
{
    QString out;
    for (const QChar c : text)
        if (c == QLatin1Char(' ') || c == QLatin1Char('-') || c.isLetterOrNumber())
            out += c;
    return out.isEmpty() ? QStringLiteral("object") : out;
}

bool containsWord(const QString &text, const QStringList &words)
{
    const QStringList found = text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    for (const QString &word : words)
        for (const QString &f : found)
            if (QString::compare(word, f, Qt::CaseInsensitive) == 0)
                return true;
    return false;
}

QString getAlanName(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return QStringLiteral("North");
    case MappableDirection::South:     return QStringLiteral("South");
    case MappableDirection::East:      return QStringLiteral("East");
    case MappableDirection::West:      return QStringLiteral("West");
    case MappableDirection::NorthEast: return QStringLiteral("Northeast");
    case MappableDirection::SouthEast: return QStringLiteral("Southeast");
    case MappableDirection::NorthWest: return QStringLiteral("Northwest");
    case MappableDirection::SouthWest: return QStringLiteral("Southwest");
    case MappableDirection::Up:        return QStringLiteral("Up");
    case MappableDirection::Down:      return QStringLiteral("Down");
    case MappableDirection::In:        return QStringLiteral("Inside");
    case MappableDirection::Out:       return QStringLiteral("Outside");
    default:                           return QString();
    }
}

} // namespace

QStringList AlanExporter::reservedWords() const
{
    return {QStringLiteral("object"), QStringLiteral("objects"), QStringLiteral("thing"),
            QStringLiteral("things"), QStringLiteral("door"), QStringLiteral("doors"),
            QStringLiteral("is"), QStringLiteral("are"), QStringLiteral("in"),
            QStringLiteral("on"), QStringLiteral("and"), QStringLiteral("outside"),
            QStringLiteral("inside")};
}

QString AlanExporter::alanName(const QString &nameIn, std::optional<int> suffix)
{
    QString name = nameIn;
    if (containsOddCharacters(name))
        name = stripOddCharacters(name);
    if (containsWord(name, reservedWords()) && !suffix.has_value())
        suffix = 1;
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

QString AlanExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return alanName(room.name, suffix);
}

QString AlanExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return alanName(displayName, suffix);
}

void AlanExporter::exportHeader(QString &out, const QString &title, const QString &author,
                                const QString &description, const QString &history)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("-- Alan currently does not process metadata such as ")
       + (isNullOrWhitespace(description) ? QStringLiteral("title and author")
                                          : QStringLiteral("title, author, or description"))
       + QStringLiteral(", so those will be in the metadata below."));
    wl(QStringLiteral("-- Trizbort exports History to the 'about' verb."));
    wl(QStringLiteral("-- All other metadata will be in comments below."));
    wl();
    wl(QStringLiteral("-- \"") + title + QStringLiteral("\" by ") + author);

    if (!isNullOrWhitespace(description)) {
        wl(QStringLiteral("-- description: ") + description
           + (description.endsWith(QLatin1Char('.')) ? QString() : QStringLiteral(".")));
        wl();
    }

    if (!isNullOrWhitespace(history)) {
        QString historyCoded = history;
        historyCoded.replace(QLatin1String("\r\n"), QLatin1String("\"\r\n    \""));
        wl(QStringLiteral("Verb about"));
        wl(QStringLiteral("    \"") + historyCoded + QLatin1Char('"'));
        wl(QStringLiteral("End Verb about."));
        wl(QStringLiteral(""));
    }
}

void AlanExporter::printThisLoc(QString &out, Location *location)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("The ") + location->exportName + QStringLiteral(" isa location Name '")
       + location->room->name + QLatin1Char('\''));

    QString nowhereExits;

    const QString description = location->room->description;
    if (!isNullOrWhitespace(description)) {
        wl(QStringLiteral("  Description"));
        wl(QStringLiteral("  \"") + description + QLatin1Char('"'));
    } else {
        wl(QStringLiteral("  Description \"\""));
    }

    for (MappableDirection direction : allDirections()) {
        Exit *exit = location->bestExit(direction);
        if (exit != nullptr && !exit->exported) {
            exit->exported = true;
            wl(QStringLiteral("  Exit ") + getAlanName(direction) + QStringLiteral(" to ")
               + exit->target->exportName + QLatin1Char('.'));
            if (exit->conditional) {
                wl(QStringLiteral("    Check"));
                wl(QStringLiteral("      \"This was marked as a conditional exit in Trizbort, so "
                                  "you'll want to change it.\""));
            }
            wl(QStringLiteral("  End exit."));
        } else {
            if (isNullOrWhitespace(nowhereExits))
                nowhereExits = getAlanName(direction);
            else
                nowhereExits += QLatin1Char(' ') + getAlanName(direction);
        }
    }

    if (!isNullOrWhitespace(nowhereExits)) {
        wl();
        wl(QStringLiteral("  Exit ") + nowhereExits + QStringLiteral(" to nowhere"));
        wl(QStringLiteral("    Check"));
        wl(QStringLiteral("      \"You can't go that way.\""));
        wl(QStringLiteral("  End exit."));
    }

    if (location->room->isDark)
        wl(QStringLiteral("  Is Not lit."));

    wl(QStringLiteral("end The ") + location->exportName + QLatin1Char('.'));
    wl(QStringLiteral(""));

    if (location->room->isStartRoom) {
        wl(QStringLiteral("The hero Isa actor at ") + location->exportName);
        wl(QStringLiteral("End The Hero."));
        wl();
    }

    for (const Thing *thing : location->things) {
        wl(QStringLiteral("The ") + thing->exportName + QStringLiteral(" isa ")
           + (thing->isPerson ? QStringLiteral("actor") : QStringLiteral("thing"))
           + QStringLiteral(" at ") + location->exportName + QLatin1Char('.'));
        wl(QStringLiteral("  IsDisplayedAs ") + thing->displayName + QLatin1Char('.'));
        wl(QStringLiteral("End The ") + thing->exportName + QLatin1Char('.'));
        wl();
    }
}

void AlanExporter::exportContent(QString &out)
{
    for (Location *location : m_locationsInExportOrder)
        printThisLoc(out, location);
}

} // namespace trizbort
