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

#include "ZilExporter.h"

namespace trizbort {

namespace {

bool isNullOrWhitespace(const QString &s)
{
    return s.trimmed().isEmpty();
}

bool startsWithVowel(const QString &s)
{
    if (s.trimmed().isEmpty())
        return false;
    const QChar c = s.at(0).toLower();
    return c == QLatin1Char('a') || c == QLatin1Char('e') || c == QLatin1Char('i')
        || c == QLatin1Char('o') || c == QLatin1Char('u');
}

bool containsOddCharacters(const QString &text)
{
    for (const QChar c : text) {
        if (c != QLatin1Char(' ') && c != QLatin1Char('-') && !c.isLetterOrNumber())
            return true;
    }
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

QString stripOddCharacters(const QString &text)
{
    QString out;
    for (const QChar c : text) {
        if (c == QLatin1Char(' ') || c == QLatin1Char('-') || c.isLetterOrNumber())
            out += c;
    }
    return out.isEmpty() ? QStringLiteral("object") : out;
}

QString toZilString(const QString &str)
{
    QString r = str;
    r.replace(QLatin1Char('\n'), QLatin1Char('|'));
    r.replace(QLatin1String("\""), QLatin1String("\\\""));
    return QLatin1Char('"') + r + QLatin1Char('"');
}

QString toZilPropertyName(MappableDirection direction)
{
    switch (direction) {
    case MappableDirection::North:     return QStringLiteral("NORTH");
    case MappableDirection::South:     return QStringLiteral("SOUTH");
    case MappableDirection::East:      return QStringLiteral("EAST");
    case MappableDirection::West:      return QStringLiteral("WEST");
    case MappableDirection::NorthEast: return QStringLiteral("NE");
    case MappableDirection::SouthEast: return QStringLiteral("SE");
    case MappableDirection::SouthWest: return QStringLiteral("SW");
    case MappableDirection::NorthWest: return QStringLiteral("NW");
    case MappableDirection::Up:        return QStringLiteral("UP");
    case MappableDirection::Down:      return QStringLiteral("DOWN");
    case MappableDirection::In:        return QStringLiteral("IN");
    case MappableDirection::Out:       return QStringLiteral("OUT");
    default:                           return QString();
    }
}

QStringList getObjectWords(const Thing *thing)
{
    QStringList list;
    const QStringList words = thing->displayName.split(QLatin1Char(' ')); // keep empties (as C#)
    for (const QString &w : words)
        list.append(stripOddCharacters(w).toUpper());
    return list;
}

QString getFlags(const Thing *thing)
{
    QString flags = QStringLiteral("TAKEBIT");
    if (startsWithVowel(thing->displayName))
        flags += QLatin1String(" VOWELBIT");
    if (!thing->contents.isEmpty())
        flags += QLatin1String(" CONTBIT");
    return flags;
}

void exportThings(QString &out, const QList<Thing *> &things, const Thing *container)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    for (const Thing *thing : things) {
        if (thing->container != container)
            continue;

        wl();
        wl(QStringLiteral("<OBJECT ") + thing->exportName);
        if (thing->container == nullptr)
            wl(QStringLiteral("    (IN ") + thing->location->exportName + QLatin1Char(')'));
        else
            wl(QStringLiteral("    (IN ") + thing->container->exportName + QLatin1Char(')'));
        wl(QStringLiteral("    (DESC ") + toZilString(thing->displayName) + QLatin1Char(')'));

        const QStringList words = getObjectWords(thing);
        if (words.size() > 0)
            wl(QStringLiteral("    (SYNONYM ") + words.at(words.size() - 1) + QLatin1Char(')'));
        if (words.size() > 1)
            wl(QStringLiteral("    (ADJECTIVE ")
               + words.mid(0, words.size() - 1).join(QLatin1Char(' ')) + QLatin1Char(')'));

        wl(QStringLiteral("    (FLAGS ") + getFlags(thing) + QStringLiteral(")>"));
        wl();

        if (!thing->contents.isEmpty())
            exportThings(out, thing->contents, thing);
    }
}

} // namespace

QStringList ZilExporter::reservedWords() const
{
    return {QStringLiteral("object"), QStringLiteral("objects")};
}

QString ZilExporter::getExportName(const Room &room, std::optional<int> suffix)
{
    QString name = room.name.toUpper().replace(QLatin1Char(' '), QLatin1Char('-'));
    if (suffix.has_value() || containsWord(name, reservedWords()) || containsOddCharacters(name))
        name = stripOddCharacters(name.replace(QLatin1Char(' '), QLatin1Char('-')));
    if (suffix.has_value())
        name = name + QLatin1Char('-') + QString::number(suffix.value());
    return name;
}

QString ZilExporter::getExportName(const QString &displayName, std::optional<int> suffix)
{
    QString name = stripOddCharacters(displayName);
    name = name.toUpper().replace(QLatin1Char(' '), QLatin1Char('-'));
    if (name.isEmpty())
        name = QStringLiteral("item");
    if (suffix.has_value())
        name = name + QString::number(suffix.value());
    return name;
}

void ZilExporter::exportHeader(QString &out, const QString &title, const QString &author,
                               const QString &description, const QString &history)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };

    // Determine the starting room.
    QString startingRoomName;
    const Room *startRoom = nullptr;
    for (const Room &r : m_map.rooms) {
        if (r.isStartRoom) {
            startRoom = &r;
            break;
        }
    }
    if (startRoom == nullptr) {
        if (!m_locationsInExportOrder.isEmpty())
            startingRoomName = m_locationsInExportOrder.first()->exportName;
    } else {
        for (const Location *loc : m_locationsInExportOrder) {
            if (loc->room->id == startRoom->id) {
                startingRoomName = loc->exportName;
                break;
            }
        }
    }

    wl(QLatin1Char('"') + title + QStringLiteral(" main file\""));
    wl();
    wl(QStringLiteral("<VERSION ZIP>"));
    wl(QStringLiteral("<CONSTANT RELEASEID 1>"));
    wl();
    wl(QStringLiteral("\"Main Loop\""));
    wl();
    wl(QStringLiteral("<CONSTANT GAME-BANNER \"") + title
       + QStringLiteral("|An interactive fiction by ") + author + QStringLiteral("\">"));
    wl();
    wl(QStringLiteral("<ROUTINE GO ()"));
    wl(QStringLiteral("    <CRLF> <CRLF>"));
    wl(QStringLiteral("    <TELL ") + toZilString(description) + QStringLiteral(" CR CR>"));
    wl(QStringLiteral("    <V-VERSION> <CRLF>"));
    wl(QStringLiteral("    <SETG HERE ,") + startingRoomName + QLatin1Char('>'));
    wl(QStringLiteral("    <MOVE ,PLAYER ,HERE>"));
    wl(QStringLiteral("    <V-LOOK>"));
    wl(QStringLiteral("    <REPEAT ()"));
    wl(QStringLiteral("        <COND (<PARSER>"));
    wl(QStringLiteral("               <PERFORM ,PRSA ,PRSO ,PRSI>"));
    wl(QStringLiteral("               <COND (<NOT <GAME-VERB?>>"));
    wl(QStringLiteral("                      <APPLY <GETP ,HERE ,P?ACTION> ,M-END>"));
    wl(QStringLiteral("                      <CLOCKER>)>)>"));
    wl(QStringLiteral("        <SETG HERE <LOC ,WINNER>>>>"));
    wl();
    wl(QStringLiteral("<INSERT-FILE \"parser\">"));
    wl();

    if (!isNullOrWhitespace(history)) {
        wl(QStringLiteral("<SYNTAX ABOUT = V-ABOUT>"));
        wl();
        wl(QStringLiteral("<ROUTINE V-ABOUT ()"));
        wl(QStringLiteral("    <TELL ") + toZilString(history) + QStringLiteral(" CR>>"));
        wl();
    }

    wl(QStringLiteral("\"Objects\""));
}

void ZilExporter::exportContent(QString &out)
{
    auto wl = [&out](const QString &s = QString()) { out += s; out += QLatin1Char('\n'); };
    auto w = [&out](const QString &s) { out += s; };

    bool needConditionalFunction = false;
    bool wroteConditionalFunction = false;

    for (Location *location : m_locationsInExportOrder) {
        wl();
        wl(QStringLiteral("<ROOM ") + location->exportName);
        wl(QStringLiteral("    (DESC ") + toZilString(location->room->name) + QLatin1Char(')'));
        w(QStringLiteral("    (IN ROOMS)"));

        const QString primaryDesc = location->room->description;
        if (!isNullOrWhitespace(primaryDesc)) {
            wl();
            w(QStringLiteral("    (LDESC ") + toZilString(primaryDesc) + QLatin1Char(')'));
        }

        for (MappableDirection direction : allDirections()) {
            Exit *exit = location->bestExit(direction);
            if (exit != nullptr && exit->conditional) {
                wl();
                w(QStringLiteral("    (") + toZilPropertyName(direction)
                  + QStringLiteral(" PER TRIZBORT-CONDITIONAL-EXIT)"));
                needConditionalFunction = true;
            } else if (exit != nullptr) {
                wl();
                w(QStringLiteral("    (") + toZilPropertyName(direction) + QStringLiteral(" TO ")
                  + exit->target->exportName + QLatin1Char(')'));
                if (!Exit::isReciprocated(location, direction, exit->target))
                    continue;
                Exit *reciprocal = exit->target->bestExit(oppositeOf(direction));
                if (reciprocal != nullptr)
                    reciprocal->exported = true;
            }
        }

        if (!location->room->isDark) {
            wl();
            w(QStringLiteral("    (FLAGS LIGHTBIT)"));
        }

        wl(QStringLiteral(">"));
        wl();

        if (needConditionalFunction && !wroteConditionalFunction) {
            wl();
            wl(QStringLiteral("<ROUTINE TRIZBORT-CONDITIONAL-EXIT ()"));
            wl(QStringLiteral("    <TELL \"An export nymph appears on your keyboard. She says, "
                              "'You can't go that way, as that exit was marked as conditional, "
                              "you know, a dotted line, in Trizbort. Obviously in your game "
                              "you'll have a better rationale for this than, er, me.' She looks "
                              "embarrassed. 'Bye!'\" CR>"));
            wl(QStringLiteral("    <RFALSE>>"));
            wl();
            wroteConditionalFunction = true;
        }

        exportThings(out, location->things, nullptr);
    }
}

} // namespace trizbort
