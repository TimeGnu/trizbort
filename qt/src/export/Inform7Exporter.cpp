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

#include "Inform7Exporter.h"

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

bool containsWord(const QString &text, const QStringList &words)
{
    const QStringList found = text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    for (const QString &word : words)
        for (const QString &f : found)
            if (QString::compare(word, f, Qt::CaseInsensitive) == 0)
                return true;
    return false;
}

// stripOddCharacters(text, exceptChars): keep space, dash, alnum, exceptChars.
QString stripOddCharacters(const QString &text, const QString &except = QString())
{
    QString out;
    for (const QChar c : text)
        if (c == QLatin1Char(' ') || c == QLatin1Char('-') || c.isLetterOrNumber()
            || except.contains(c))
            out += c;
    return out.isEmpty() ? QStringLiteral("object") : out;
}

QString toInform7PrintableString(const QString &text)
{
    QString r = text;
    r.replace(QLatin1String("'"), QLatin1String("[']"));
    r.replace(QLatin1String("\""), QLatin1String("'"));
    return QLatin1Char('"') + r + QLatin1Char('"');
}

QString toInform7UnderstandString(const QString &text)
{
    return QLatin1Char('"') + stripOddCharacters(text, QStringLiteral("'")) + QLatin1Char('"');
}

QString toInform7UnderstandWords(const QString &text)
{
    const QStringList words = text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    QString out;
    for (const QString &word : words) {
        if (!out.isEmpty())
            out += QStringLiteral(" and ");
        out += toInform7UnderstandString(word);
    }
    return out;
}

QString getInform7Name(MappableDirection direction)
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

// Lowercase direction name from the MappableDirection enum's own ToString (used
// by the door writer): e.g. "north", "northeast".
QString enumNameLower(MappableDirection d)
{
    switch (d) {
    case MappableDirection::North:     return QStringLiteral("north");
    case MappableDirection::South:     return QStringLiteral("south");
    case MappableDirection::East:      return QStringLiteral("east");
    case MappableDirection::West:      return QStringLiteral("west");
    case MappableDirection::NorthEast: return QStringLiteral("northeast");
    case MappableDirection::SouthEast: return QStringLiteral("southeast");
    case MappableDirection::SouthWest: return QStringLiteral("southwest");
    case MappableDirection::NorthWest: return QStringLiteral("northwest");
    case MappableDirection::Up:        return QStringLiteral("up");
    case MappableDirection::Down:      return QStringLiteral("down");
    case MappableDirection::In:        return QStringLiteral("in");
    case MappableDirection::Out:       return QStringLiteral("out");
    default:                           return QStringLiteral("none");
    }
}

bool isPlural(const QString &noun)
{
    return !noun.isEmpty() && !noun.at(0).isUpper() && noun.endsWith(QLatin1Char('s'))
           && !noun.endsWith(QLatin1String("ss"));
}

QString genderName(Thing::Gender g)
{
    switch (g) {
    case Thing::Gender::Neuter: return QStringLiteral("Neuter");
    case Thing::Gender::Male:   return QStringLiteral("Male");
    case Thing::Gender::Female: return QStringLiteral("Female");
    }
    return QString();
}

QString getArticle(const Thing *thing)
{
    const QString noun = thing->exportName;
    if (thing->properNamed)
        return QString();
    if (noun.isEmpty() || isPlural(noun) || thing->forceplural == Thing::Amounts::Plural) {
        if (!noun.isEmpty() && noun.at(0).isUpper())
            return QString();
        return QStringLiteral("Some ");
    }
    if (QStringLiteral("aeiou").indexOf(noun.at(0).toLower()) >= 0)
        return QStringLiteral("An ");
    return QStringLiteral("A ");
}

QString whatItIs(const Thing *thing)
{
    QString whatString;
    if (thing->isPerson) {
        whatString += thing->exportName + QStringLiteral(" is a ") + genderName(thing->gender)
                      + QStringLiteral(" person");
    } else if (thing->partOf) {
        whatString += QStringLiteral(" is ");
    } else if (thing->isScenery) {
        whatString += QStringLiteral(". ") + thing->exportName + QStringLiteral(" is scenery");
    } else if (thing->isContainer) {
        whatString += QStringLiteral(". ") + thing->exportName + QStringLiteral(" is a container");
    } else if (thing->isSupporter) {
        whatString += QStringLiteral(". ") + thing->exportName + QStringLiteral(" is a supporter");
    } else {
        whatString = QStringLiteral("is a ")
                     + (thing->forceplural == Thing::Amounts::Plural ? QStringLiteral("plural-named ")
                                                                     : QString())
                     + (thing->properNamed ? QStringLiteral("proper-named ") : QString())
                     + QStringLiteral("thing");
    }
    return whatString;
}

} // namespace

QStringList Inform7Exporter::reservedWords() const
{
    return {QStringLiteral("object"), QStringLiteral("objects"), QStringLiteral("thing"),
            QStringLiteral("things"), QStringLiteral("door"), QStringLiteral("doors"),
            QStringLiteral("is"), QStringLiteral("are"), QStringLiteral("in"),
            QStringLiteral("on"), QStringLiteral("and"), QStringLiteral("outside"),
            QStringLiteral("inside")};
}

QString Inform7Exporter::inform7Name(const QString &nameIn, std::optional<int> suffix)
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

QString Inform7Exporter::getExportName(const Room &room, std::optional<int> suffix)
{
    return inform7Name(room.name, suffix);
}

QString Inform7Exporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    return inform7Name(displayName, suffix);
}

int Inform7Exporter::numberOfRoomsWithoutRegion() const
{
    int n = 0;
    for (const Room &r : m_map.rooms)
        if (r.region == QLatin1String("NoRegion") || r.region.isEmpty())
            ++n;
    return n;
}

QString Inform7Exporter::regionExportNameFor(const QString &regionName) const
{
    for (const ExportRegion *er : m_regionsInExportOrder)
        if (er->region.name == regionName)
            return er->exportName;
    return QString();
}

bool Inform7Exporter::printThisLoc(QString &out, Location *location)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    auto w = [&out](const QString &s) { out += s; };

    wl(QStringLiteral("part ") + location->exportName);
    wl();

    w(QStringLiteral("There is a room called ") + location->exportName + QLatin1Char('.'));
    if (location->exportName != location->room->name)
        w(QStringLiteral(" The printed name of it is ")
          + toInform7PrintableString(location->room->name) + QLatin1Char('.'));
    const QString description = location->room->description;
    if (!description.isEmpty())
        w(QLatin1Char(' ') + toInform7PrintableString(description)
          + (description.endsWith(QLatin1Char('.')) ? QString() : QStringLiteral(".")));
    if (location->room->isDark)
        w(QStringLiteral(" It is dark."));

    if (!location->room->region.isEmpty() && location->room->region != QLatin1String("NoRegion"))
        wl(QStringLiteral(" It is in ") + regionExportNameFor(location->room->region)
           + QLatin1Char('.'));
    else
        wl();

    wl(); // extra blank line

    if (location->room->isStartRoom) {
        w(QStringLiteral("The player is in ") + location->exportName + QLatin1Char('.'));
        wl();
    }

    bool exportedThings = false;
    for (const Thing *thing : location->things) {
        exportedThings = true;

        QString thingText;
        if (!thing->isPerson)
            thingText += getArticle(thing) + thing->exportName + QLatin1Char(' ');
        thingText += whatItIs(thing);
        if (thing->container == nullptr) {
            thingText += QStringLiteral(" in ") + thing->location->exportName + QLatin1Char('.');
        } else if (thing->container->isPerson) {
            thingText += (thing->worn ? QStringLiteral(" worn by ") : QStringLiteral(" carried by "))
                         + thing->container->exportName + QLatin1Char('.');
        } else if (thing->partOf) {
            thingText += QStringLiteral(" part of ") + thing->container->exportName + QLatin1Char('.');
        } else {
            thingText += QStringLiteral(" in ") + thing->container->exportName + QLatin1Char('.');
        }
        w(thingText);

        if (thing->displayName != thing->exportName)
            w(QStringLiteral(" It is privately-named. The printed name of it is ")
              + toInform7PrintableString(thing->displayName)
              + (thing->displayName.endsWith(QLatin1Char('.')) ? QString() : QStringLiteral("."))
              + QStringLiteral(" Understand ") + toInform7UnderstandWords(thing->displayName)
              + QStringLiteral(" as ") + thing->exportName + QLatin1Char('.'));
        wl();
        if (!isNullOrWhitespace(thing->warningText))
            wl(QStringLiteral("[Note: there were errors with your bracketed definitions.\n")
               + thing->warningText + QLatin1Char(']'));
    }

    if (exportedThings)
        wl();

    bool exportedExits = false;
    for (MappableDirection direction : allDirections()) {
        Exit *exit = location->bestExit(direction);
        if (exit == nullptr || exit->exported)
            continue;
        exit->exported = true;
        exportedExits = true;

        if (!exit->hasDoor) {
            // writeNormalExit
            w(getInform7Name(direction) + QStringLiteral(" of ") + location->exportName
              + QStringLiteral(" is ") + exit->target->exportName + QLatin1Char('.'));
            const MappableDirection opposite = oppositeOf(direction);
            if (Exit::isReciprocated(location, direction, exit->target)) {
                if (Exit *reciprocal = exit->target->bestExit(opposite))
                    reciprocal->exported = true;
            } else if (exit->target->bestExit(opposite) == nullptr) {
                w(QLatin1Char(' ') + getInform7Name(opposite) + QStringLiteral(" of ")
                  + exit->target->exportName + QStringLiteral(" is nowhere."));
            }
            wl();
        } else {
            // writeDoor
            const MappableDirection opposite = oppositeOf(direction);
            wl(exit->connectionName + QStringLiteral(" is a door. ") + exit->connectionName
               + QStringLiteral(" is ") + enumNameLower(direction) + QStringLiteral(" of ")
               + location->exportName + QStringLiteral(" and ") + enumNameLower(opposite)
               + QStringLiteral(" of ") + exit->target->exportName + QStringLiteral(".  "));
            wl(exit->connectionName + QStringLiteral(" is ")
               + (exit->door.open ? QStringLiteral("open") : QStringLiteral("closed"))
               + QStringLiteral(" and ")
               + (exit->door.openable ? QStringLiteral("openable") : QStringLiteral("not openable"))
               + QLatin1Char('.'));
            wl(exit->connectionName + QStringLiteral(" is ")
               + (exit->door.locked ? QStringLiteral("locked") : QStringLiteral("unlocked"))
               + QStringLiteral(" and ")
               + (exit->door.lockable ? QStringLiteral("lockable") : QStringLiteral("not lockable"))
               + QLatin1Char('.'));
            wl(QStringLiteral("The description of ") + exit->connectionName + QStringLiteral(" is ")
               + toInform7PrintableString(exit->connectionDescription) + QLatin1Char('.'));
            if (Exit *reciprocal = exit->target->bestExit(opposite))
                reciprocal->exported = true;
            wl();
        }
    }

    if (exportedExits)
        wl();

    bool wroteConditionalExit = false;
    for (MappableDirection direction : allDirections()) {
        Exit *exit = location->bestExit(direction);
        if (exit != nullptr && exit->conditional) {
            wroteConditionalExit = true;
            wl(QStringLiteral("Instead of going ") + getInform7Name(direction).toLower()
               + QStringLiteral(" from ") + location->exportName
               + QStringLiteral(", block conditional exits."));
        }
    }

    if (wroteConditionalExit)
        wl();
    return wroteConditionalExit;
}

void Inform7Exporter::exportHeader(QString &out, const QString &title, const QString &author,
                                   const QString &description, const QString &history)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QLatin1Char('"') + title + QStringLiteral("\" by \"") + author + QLatin1Char('"'));
    wl();

    if (!description.isEmpty()) {
        wl(QStringLiteral("The story description is ") + toInform7PrintableString(description)
           + (description.endsWith(QLatin1Char('.')) ? QString() : QStringLiteral(".")));
        wl();
    }

    wl(QStringLiteral("Volume Trizbort generated map"));
    wl();

    if (!isNullOrWhitespace(history)) {
        QString historyCoded = history;
        historyCoded.replace(QLatin1String("\r\n"), QLatin1String("\r\n[line break]"));
        wl(QStringLiteral("chapter about"));
        wl(QStringLiteral(""));
        wl(QStringLiteral("abouting is an action out of world."));
        wl(QStringLiteral("understand the command \"about\" as something new."));
        wl(QStringLiteral("understand \"about\" as abouting."));
        wl(QStringLiteral("carry out abouting: say \"") + historyCoded + QStringLiteral("\"."));
        wl(QStringLiteral(""));
    }
}

void Inform7Exporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    bool anyConditionalExits = false;

    if (numberOfRoomsWithoutRegion() > 0) {
        wl(QStringLiteral("book Regionless Rooms"));
        wl();
    }

    for (Location *location : m_locationsInExportOrder) {
        if (location->room->region != QLatin1String("NoRegion"))
            continue;
        anyConditionalExits = printThisLoc(out, location) || anyConditionalExits;
    }

    for (const ExportRegion *region : m_regionsInExportOrder) {
        wl(QStringLiteral("book ") + getExportName(region->exportName, std::nullopt));
        wl();
        wl(QStringLiteral("There is a region called ")
           + getExportName(region->exportName, std::nullopt) + QLatin1Char('.'));
        wl();
        for (Location *location : m_locationsInExportOrder) {
            if (location->room->region == region->region.name
                || location->room->region == region->exportName)
                anyConditionalExits = printThisLoc(out, location) || anyConditionalExits;
        }
    }

    if (anyConditionalExits) {
        wl(QStringLiteral("book conditional exit warning"));
        wl();
        wl(QStringLiteral("To block conditional exits:"));
        wl(QStringLiteral("\tsay \"An export nymph appears on your keyboard. She says, 'You can't "
                          "go that way, as that exit was marked as conditional, you know, a dotted "
                          "line, in Trizbort. Obviously in your game you'll have a better rationale "
                          "for this than, er, me.' She looks embarrassed. 'Bye!'\""));
        wl();
    }
}

} // namespace trizbort
