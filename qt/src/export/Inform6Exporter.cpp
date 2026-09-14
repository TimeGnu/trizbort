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

#include "Inform6Exporter.h"

#include <functional>

namespace trizbort {

namespace {

const QChar SINGLE_QUOTE = QLatin1Char('\'');
const QChar DOUBLE_QUOTE = QLatin1Char('"');

bool isAccentKept(ushort c)
{
    return (c >= 0x00C0 && c <= 0x00CF) || c == 0x00D1 || (c >= 0x00D2 && c <= 0x00D6)
           || (c >= 0x00D9 && c <= 0x00DC) || c == 0x00DF || (c >= 0x00E0 && c <= 0x00EF)
           || c == 0x00F1 || (c >= 0x00F2 && c <= 0x00F6) || (c >= 0x00F9 && c <= 0x00FC);
}

// stripOddCharacters(text, accented..., ' ', '-'): keep alnum-ascii, '_', the
// accented set, space and dash.
QString stripUnaccentedCharacters(const QString &text)
{
    QString result;
    for (const QChar qc : text) {
        const ushort c = qc.unicode();
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'
            || qc == QLatin1Char(' ') || qc == QLatin1Char('-') || isAccentKept(c))
            result += qc;
    }
    return result;
}

QString toI6String(const QString &text, QChar quote)
{
    QString r = text;
    r.replace(DOUBLE_QUOTE, QLatin1Char('~'));
    r.remove(QLatin1Char('\r'));
    r.replace(QLatin1Char('\n'), QLatin1Char('^'));
    return quote + r + quote;
}

QString toI6PropertyName(MappableDirection direction)
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
    default:                           return QStringLiteral("north");
    }
}

} // namespace

QStringList Inform6Exporter::reservedWords() const
{
    return {QStringLiteral("Constant"), QStringLiteral("Story"), QStringLiteral("Headline"),
            QStringLiteral("Include"), QStringLiteral("Object"), QStringLiteral("with"),
            QStringLiteral("has"), QStringLiteral("hasnt"), QStringLiteral("not"),
            QStringLiteral("and"), QStringLiteral("or"), QStringLiteral("n_to"),
            QStringLiteral("s_to"), QStringLiteral("e_to"), QStringLiteral("w_to"),
            QStringLiteral("nw_to"), QStringLiteral("ne_to"), QStringLiteral("sw_to"),
            QStringLiteral("se_to"), QStringLiteral("u_to"), QStringLiteral("d_to"),
            QStringLiteral("in_to"), QStringLiteral("out_to"), QStringLiteral("before"),
            QStringLiteral("after"), QStringLiteral("if"), QStringLiteral("else"),
            QStringLiteral("print"), QStringLiteral("player"), QStringLiteral("location"),
            QStringLiteral("description")};
}

QString Inform6Exporter::getExportName(const Room &room, std::optional<int> suffix)
{
    QString name = deaccent(stripUnaccentedCharacters(room.name));
    name.remove(QLatin1Char(' '));
    name.remove(QLatin1Char('-'));
    if (name.isEmpty())
        name = QStringLiteral("room");
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

QString Inform6Exporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    QString name = deaccent(stripUnaccentedCharacters(displayName));
    name.remove(QLatin1Char(' '));
    name.remove(QLatin1Char('-'));
    if (name.isEmpty())
        name = QStringLiteral("item");
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

// toI6Words needs deaccent; make it a member helper via a small local lambda in
// the users below. Here we implement a file-local version taking a deaccenter.
static QString toI6WordsImpl(const QString &text,
                             const std::function<QString(const QString &)> &deacc)
{
    const QStringList words = text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (words.isEmpty())
        return toI6String(QStringLiteral("thing"), SINGLE_QUOTE);
    QString output;
    for (const QString &word : words) {
        if (!output.isEmpty())
            output += QLatin1Char(' ');
        output += toI6String(deacc(word), SINGLE_QUOTE);
    }
    return output;
}

void Inform6Exporter::writeLocation(QString &out, Location *location)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    const QString cls = location->room->region == QLatin1String("NoRegion")
        ? QStringLiteral("Object")
        : getExportName(location->room->region, std::nullopt);

    wl(cls + QStringLiteral("  ") + location->exportName + QLatin1Char(' ')
       + toI6String(location->room->name, DOUBLE_QUOTE));
    wl(QStringLiteral("  with  description"));
    wl(QStringLiteral("            ") + toI6String(location->room->description, DOUBLE_QUOTE)
       + QLatin1Char(','));

    for (MappableDirection direction : allDirections()) {
        Exit *exit = location->bestExit(direction);
        if (exit != nullptr)
            wl(QStringLiteral("        ") + toI6PropertyName(direction) + QLatin1Char(' ')
               + (exit->hasDoor ? getExportName(exit->connectionName, std::nullopt)
                                : exit->target->exportName)
               + QLatin1Char(','));
    }

    wl(QStringLiteral("   has  ")
       + (location->room->isDark ? QStringLiteral("~") : QString()) + QStringLiteral("light;"));
    wl();
}

void Inform6Exporter::writeDoor(QString &out, Location *location, MappableDirection direction,
                                Exit *exit)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    auto deacc = [this](const QString &s) { return deaccent(s); };

    const MappableDirection opposite = oppositeOf(direction);

    wl(QStringLiteral("Object ") + getExportName(exit->connectionName, std::nullopt)
       + QLatin1Char(' ') + exit->connectionDescription);
    wl(QStringLiteral("  with  name ")
       + toI6WordsImpl(deaccent(stripUnaccentedCharacters(exit->connectionName)), deacc)
       + QLatin1Char(','));
    wl(QStringLiteral("        description ")
       + toI6String(exit->connectionDescription, DOUBLE_QUOTE) + QLatin1Char(','));
    wl(QStringLiteral("        found_in ") + location->exportName + QLatin1Char(' ')
       + exit->target->exportName + QLatin1Char(','));
    wl(QStringLiteral("        door_to [; if (self in ") + location->exportName
       + QStringLiteral(") return ") + exit->target->exportName + QStringLiteral("; return ")
       + location->exportName + QStringLiteral(";],"));
    wl(QStringLiteral("        door_dir [; if (self in ") + location->exportName
       + QStringLiteral(") return ") + toI6PropertyName(direction) + QStringLiteral("; return ")
       + toI6PropertyName(opposite) + QStringLiteral("; ],"));
    wl(QStringLiteral("  has   door ")
       + (exit->door.openable ? QStringLiteral("openable") : QString()) + QLatin1Char(' ')
       + (exit->door.open ? QStringLiteral("open") : QStringLiteral("~open")) + QLatin1Char(' ')
       + (exit->door.lockable ? QStringLiteral("lockable") : QString()) + QLatin1Char(' ')
       + (exit->door.locked ? QStringLiteral("locked") : QStringLiteral("~locked"))
       + QStringLiteral(" ;"));

    if (Exit *reciprocal = exit->target->bestExit(opposite))
        reciprocal->exported = true;
    wl();
}

static QStringList setAttributes(const Thing *thing)
{
    QStringList attributes;
    if (!thing->contents.isEmpty()) {
        bool anyPartOf = false;
        for (const Thing *item : thing->contents)
            if (item->partOf)
                anyPartOf = true;
        if (anyPartOf) {
            attributes << QStringLiteral("transparent");
        } else {
            attributes << QStringLiteral("open");
            attributes << QStringLiteral("container");
        }
    }
    if (thing->properNamed)
        attributes << QStringLiteral("proper");
    if (thing->isPerson) {
        attributes << QStringLiteral("animate");
        switch (thing->gender) {
        case Thing::Gender::Female: attributes << QStringLiteral("female"); break;
        case Thing::Gender::Male:   attributes << QStringLiteral("male"); break;
        case Thing::Gender::Neuter: attributes << QStringLiteral("neuter"); break;
        }
    }
    if (thing->isScenery)
        attributes << QStringLiteral("scenery");
    if (thing->isSupporter)
        attributes << QStringLiteral("supporter");
    if (thing->isContainer && !attributes.contains(QStringLiteral("container")))
        attributes << QStringLiteral("container");
    if (thing->forceplural == Thing::Amounts::Plural)
        attributes << QStringLiteral("pluralname");
    if (thing->worn)
        attributes << QStringLiteral("clothing");
    return attributes;
}

static void writeOneThing(QString &out, const Thing *thing, int indent,
                          const std::function<QString(const QString &)> &deacc)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    auto w = [&out](const QString &s) { out += s; };

    wl(QStringLiteral("Object ") + QStringLiteral("-> ").repeated(indent) + QLatin1Char(' ')
       + thing->exportName + QLatin1Char(' ')
       + toI6String(stripUnaccentedCharacters(thing->displayName).trimmed(), DOUBLE_QUOTE));
    wl(QStringLiteral("  with  name ")
       + toI6WordsImpl(deacc(stripUnaccentedCharacters(thing->displayName)), deacc)
       + QLatin1Char(','));
    w(QStringLiteral("        description ") + toI6String(thing->displayName, DOUBLE_QUOTE));

    const QStringList attributes = setAttributes(thing);
    if (attributes.isEmpty()) {
        wl(QStringLiteral(";"));
    } else {
        wl();
        wl(QStringLiteral("  has ") + attributes.join(QLatin1Char(' ')) + QStringLiteral(";"));
    }
    wl();
}

static void exportThings(QString &out, const QList<Thing *> &things, const Thing *container,
                         int indent, const std::function<QString(const QString &)> &deacc)
{
    for (const Thing *thing : things) {
        if (thing->container != container)
            continue;
        writeOneThing(out, thing, indent, deacc);
        exportThings(out, thing->contents, thing, indent + 1, deacc);
    }
}

void Inform6Exporter::exportHeader(QString &out, const QString &title, const QString &author,
                                   const QString &description, const QString &history)
{
    Q_UNUSED(history);
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    wl(QStringLiteral("Constant Story ") + toI6String(title, DOUBLE_QUOTE) + QLatin1Char(';'));
    wl(QStringLiteral("Constant Headline ")
       + toI6String(QStringLiteral("^By ") + author + QLatin1Char('^') + description
                        + QStringLiteral("^^"),
                    DOUBLE_QUOTE)
       + QLatin1Char(';'));
    wl();
    wl(QStringLiteral("Include \"Parser\";"));
    wl(QStringLiteral("Include \"VerbLib\";"));
    wl();
}

void Inform6Exporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    auto deacc = [this](const QString &s) { return deaccent(s); };

    if (!m_regionsInExportOrder.isEmpty()) {
        for (const ExportRegion *region : m_regionsInExportOrder)
            wl(QStringLiteral("Class ") + region->exportName + QLatin1Char(';'));
        wl();
    }

    for (Location *location : m_locationsInExportOrder) {
        writeLocation(out, location);

        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit == nullptr || !exit->hasDoor || exit->exported)
                continue;
            exit->exported = true;
            writeDoor(out, location, direction, exit);
        }

        exportThings(out, location->things, nullptr, 1, deacc);
    }

    wl(QStringLiteral("[ Initialise;"));
    if (!m_locationsInExportOrder.isEmpty()) {
        bool foundStart = false;
        for (Location *location : m_locationsInExportOrder) {
            if (!location->room->isStartRoom)
                continue;
            if (foundStart) {
                wl(QStringLiteral("! ") + location->exportName
                   + QStringLiteral(" is a second start-room according to Trizbort."));
            } else {
                wl(QStringLiteral("    location = ") + location->exportName + QLatin1Char(';'));
                foundStart = true;
            }
        }
        if (!foundStart)
            wl(QStringLiteral("    location = ") + m_locationsInExportOrder.first()->exportName
               + QLatin1Char(';'));
    } else {
        wl(QStringLiteral("    ! location = ...;"));
    }

    wl(QStringLiteral("    ! \"^^Your opening paragraph here...^^\";"));
    wl(QStringLiteral("];"));
    wl();
    wl(QStringLiteral("Include \"Grammar\";"));
    wl();
    if (!m_map.history.isEmpty()) {
        wl(QStringLiteral("Verb meta 'about' * -> About;"));
        wl();
        wl(QStringLiteral("[ AboutSub ;"));
        wl(QStringLiteral("  print(") + toI6String(m_map.history, DOUBLE_QUOTE) + QStringLiteral(");"));
        wl(QStringLiteral("];"));
        wl();
    }
}

} // namespace trizbort
