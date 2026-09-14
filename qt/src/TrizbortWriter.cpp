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

#include "TrizbortWriter.h"

#include <charconv>

#include <QFile>
#include <QObject>
#include <algorithm>

#include "DotNetColor.h"
#include "TrizbortReader.h"

namespace trizbort {

namespace {

// Settings/region colours are serialized by ColorTranslator.ToHtml, which is
// provenance-sensitive in .NET (a colour loaded as "#F5F5F5" stays hex; one
// created from a KnownColor stays named). We can't recover provenance from the
// colour alone, so we keep the exact token each colour was read from and reuse
// it as long as the colour is unchanged; an edited colour falls back to ToHtml.
QString colorWithToken(const QColor &c, const QString &token)
{
    if (!token.isEmpty() && parseTrizbortColor(token) == c)
        return token;
    return dotNetColorToHtml(c);
}

// XmlScribe writes booleans as "yes"/"no".
QString yesNo(bool b) { return b ? QStringLiteral("yes") : QStringLiteral("no"); }

// Float attribute/element formatting: XmlScribe uses float.ToString with
// InvariantCulture, i.e. the shortest round-tripping decimal. Values are stored
// as double but originate as single-precision floats, so cast before printing.
QString fmtFloat(double value)
{
    char buf[64];
    const std::to_chars_result res =
        std::to_chars(buf, buf + sizeof(buf), static_cast<float>(value));
    return QString::fromLatin1(buf, static_cast<int>(res.ptr - buf));
}

// Colors.SaveColor: empty for the default (invalid/transparent), else the
// uppercase #RRGGBB hex (Color.ToHex). Used for room and connection colours.
QString saveColorHex(const QColor &c)
{
    if (!c.isValid())
        return QString();
    const unsigned int rgb =
        ((c.red() & 0xFF) << 16) | ((c.green() & 0xFF) << 8) | (c.blue() & 0xFF);
    return QStringLiteral("#%1").arg(rgb, 6, 16, QLatin1Char('0')).toUpper();
}

QString escAttr(const QString &s)
{
    QString r;
    r.reserve(s.size() + 8);
    for (const QChar c : s) {
        switch (c.unicode()) {
        case '&': r += QLatin1String("&amp;"); break;
        case '<': r += QLatin1String("&lt;"); break;
        case '>': r += QLatin1String("&gt;"); break;
        case '"': r += QLatin1String("&quot;"); break;
        // Attribute values normalize whitespace, so newlines/tabs must be escaped
        // to survive a round trip (matching what .NET's XmlWriter emits).
        case '\n': r += QLatin1String("&#xA;"); break;
        case '\r': r += QLatin1String("&#xD;"); break;
        case '\t': r += QLatin1String("&#x9;"); break;
        default: r += c; break;
        }
    }
    return r;
}

QString escText(const QString &s)
{
    QString r;
    r.reserve(s.size() + 8);
    for (const QChar c : s) {
        switch (c.unicode()) {
        case '&': r += QLatin1String("&amp;"); break;
        case '<': r += QLatin1String("&lt;"); break;
        case '>': r += QLatin1String("&gt;"); break;
        case '\r': r += QLatin1String("&#xD;"); break;
        default: r += c; break;
        }
    }
    return r;
}

QString indent(int depth) { return QString(depth, QLatin1Char('\t')); }

// One attribute, only when the value is non-empty-worthy per the caller.
void addAttr(QString &out, const QString &name, const QString &value)
{
    out += QLatin1Char(' ');
    out += name;
    out += QLatin1String("=\"");
    out += escAttr(value);
    out += QLatin1Char('"');
}

// XmlConvert.EncodeName, used (after " "->"____") for a region's element tag.
// Non-XML-name characters become _xHHHH_; a leading non-name-start char too.
QString encodeXmlName(const QString &in)
{
    QString s = in;
    s.replace(QLatin1Char(' '), QLatin1String("____"));
    auto isNameChar = [](QChar c, bool first) {
        const ushort u = c.unicode();
        const bool letter = (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z');
        const bool digit = (u >= '0' && u <= '9');
        if (first)
            return letter || u == '_';
        return letter || digit || u == '_' || u == '-' || u == '.';
    };
    QString r;
    for (int i = 0; i < s.size(); ++i) {
        const QChar c = s.at(i);
        if (isNameChar(c, i == 0)) {
            r += c;
        } else {
            r += QStringLiteral("_x%1_").arg(c.unicode(), 4, 16, QLatin1Char('0')).toUpper();
        }
    }
    if (r.isEmpty())
        r = QStringLiteral("_");
    return r;
}

void writeObjects(QString &out, const Room &r, int depth)
{
    const QString &at = r.objectsPosition.at;
    const bool defaultPosition = at.isEmpty() || at == QLatin1String("s"); // South is default
    if (r.objectsText.isEmpty() && defaultPosition)
        return;

    QString openTag = indent(depth) + QLatin1String("<objects");
    if (!defaultPosition)
        addAttr(openTag, QStringLiteral("at"), at);
    if (r.objectsPosition.custom) {
        addAttr(openTag, QStringLiteral("custom"), yesNo(true));
        addAttr(openTag, QStringLiteral("customRight"), QString::number(r.objectsPosition.customRight));
        addAttr(openTag, QStringLiteral("customDown"), QString::number(r.objectsPosition.customDown));
    }

    if (r.objectsText.isEmpty()) {
        out += openTag + QLatin1String(" />\n");
        return;
    }

    // Encode newlines as '|' (escaping literal pipes), matching Room.Save.
    QString encoded = r.objectsText;
    encoded.replace(QLatin1String("\r"), QString());
    encoded.replace(QLatin1String("|"), QLatin1String("\\|"));
    encoded.replace(QLatin1String("\n"), QLatin1String("|"));

    out += openTag + QLatin1Char('>') + escText(encoded) + QLatin1String("</objects>\n");
}

void writeRoom(QString &out, const Room &r, int depth)
{
    QString tag = indent(depth) + QLatin1String("<room");
    addAttr(tag, QStringLiteral("id"), QString::number(r.id));
    addAttr(tag, QStringLiteral("name"), r.name);
    addAttr(tag, QStringLiteral("subtitle"), r.subtitle);
    addAttr(tag, QStringLiteral("x"), fmtFloat(r.x));
    addAttr(tag, QStringLiteral("y"), fmtFloat(r.y));
    addAttr(tag, QStringLiteral("w"), fmtFloat(r.w));
    addAttr(tag, QStringLiteral("h"), fmtFloat(r.h));
    // Preserve the loaded region string exactly (empty stays empty) so the round
    // trip is lossless; new rooms default to NoRegion in the model.
    addAttr(tag, QStringLiteral("region"), r.region);
    if (r.referenceRoom >= 0)
        addAttr(tag, QStringLiteral("referenceRoom"), QString::number(r.referenceRoom));

    addAttr(tag, QStringLiteral("handDrawn"), yesNo(r.handDrawn));
    addAttr(tag, QStringLiteral("allcornersequal"), yesNo(r.allCornersEqual));
    addAttr(tag, QStringLiteral("ellipse"), yesNo(r.ellipse));
    addAttr(tag, QStringLiteral("roundedCorners"), yesNo(r.roundedCorners));
    addAttr(tag, QStringLiteral("octagonal"), yesNo(r.octagonal));
    addAttr(tag, QStringLiteral("cornerTopLeft"), fmtFloat(r.cornerTopLeft));
    addAttr(tag, QStringLiteral("cornerTopRight"), fmtFloat(r.cornerTopRight));
    addAttr(tag, QStringLiteral("cornerBottomLeft"), fmtFloat(r.cornerBottomLeft));
    addAttr(tag, QStringLiteral("cornerBottomRight"), fmtFloat(r.cornerBottomRight));

    addAttr(tag, QStringLiteral("borderstyle"), r.borderStyle);
    if (r.isDark)
        addAttr(tag, QStringLiteral("isDark"), yesNo(true));
    if (r.isStartRoom)
        addAttr(tag, QStringLiteral("isStartRoom"), yesNo(true));
    if (r.isEndRoom)
        addAttr(tag, QStringLiteral("isEndRoom"), yesNo(true));

    addAttr(tag, QStringLiteral("description"), r.description);
    addAttr(tag, QStringLiteral("roomFill"), saveColorHex(r.fill));
    addAttr(tag, QStringLiteral("secondFill"), saveColorHex(r.secondFill));
    addAttr(tag, QStringLiteral("secondFillLocation"), r.secondFillLocation);
    addAttr(tag, QStringLiteral("roomBorder"), saveColorHex(r.border));
    addAttr(tag, QStringLiteral("roomLargeText"), saveColorHex(r.largeText));
    addAttr(tag, QStringLiteral("roomSubtitleColor"), saveColorHex(r.subtitleColor));
    addAttr(tag, QStringLiteral("roomSmallText"), saveColorHex(r.smallText));
    addAttr(tag, QStringLiteral("ZOrder"), QString::number(r.zOrder));

    // Determine whether an <objects> child will be written.
    const QString &at = r.objectsPosition.at;
    const bool defaultPosition = at.isEmpty() || at == QLatin1String("s");
    const bool hasObjects = !(r.objectsText.isEmpty() && defaultPosition);

    if (!hasObjects) {
        out += tag + QLatin1String(" />\n");
        return;
    }
    out += tag + QLatin1String(">\n");
    writeObjects(out, r, depth + 1);
    out += indent(depth) + QLatin1String("</room>\n");
}

void writeLine(QString &out, const Connection &c, int depth)
{
    QString tag = indent(depth) + QLatin1String("<line");
    addAttr(tag, QStringLiteral("id"), QString::number(c.id));
    addAttr(tag, QStringLiteral("name"), c.name);
    addAttr(tag, QStringLiteral("description"), c.description);
    if (c.hasDoor) {
        addAttr(tag, QStringLiteral("door"), yesNo(true));
        addAttr(tag, QStringLiteral("lockable"), yesNo(c.door.lockable));
        addAttr(tag, QStringLiteral("openable"), yesNo(c.door.openable));
        addAttr(tag, QStringLiteral("locked"), yesNo(c.door.locked));
        addAttr(tag, QStringLiteral("open"), yesNo(c.door.open));
    }
    if (c.color.isValid())
        addAttr(tag, QStringLiteral("color"), saveColorHex(c.color));
    if (c.style != ConnectionStyle::Solid)
        addAttr(tag, QStringLiteral("style"), QStringLiteral("dashed"));
    if (c.flow != ConnectionFlow::TwoWay)
        addAttr(tag, QStringLiteral("flow"), QStringLiteral("oneWay"));
    if (!c.startText.isEmpty())
        addAttr(tag, QStringLiteral("startText"), c.startText);
    if (!c.midText.isEmpty())
        addAttr(tag, QStringLiteral("midText"), c.midText);
    if (!c.endText.isEmpty())
        addAttr(tag, QStringLiteral("endText"), c.endText);

    if (c.vertices.isEmpty()) {
        out += tag + QLatin1String(" />\n");
        return;
    }
    out += tag + QLatin1String(">\n");
    int index = 0;
    for (const Vertex &v : c.vertices) {
        QString vtag = indent(depth + 1);
        if (v.docked) {
            vtag += QLatin1String("<dock");
            addAttr(vtag, QStringLiteral("index"), QString::number(index));
            addAttr(vtag, QStringLiteral("id"), QString::number(v.roomId));
            addAttr(vtag, QStringLiteral("port"), v.port);
        } else {
            vtag += QLatin1String("<point");
            addAttr(vtag, QStringLiteral("index"), QString::number(index));
            addAttr(vtag, QStringLiteral("x"), fmtFloat(v.point.x()));
            addAttr(vtag, QStringLiteral("y"), fmtFloat(v.point.y()));
        }
        out += vtag + QLatin1String(" />\n");
        ++index;
    }
    out += indent(depth) + QLatin1String("</line>\n");
}

void writeElementText(QString &out, int depth, const QString &tag, const QString &text)
{
    out += indent(depth) + QLatin1Char('<') + tag + QLatin1Char('>') + escText(text)
        + QLatin1String("</") + tag + QLatin1String(">\n");
}

void writeFont(QString &out, int depth, const QString &tag, const FontSpec &f)
{
    QString open = indent(depth) + QLatin1Char('<') + tag;
    addAttr(open, QStringLiteral("size"), fmtFloat(f.size));
    if (f.bold)
        addAttr(open, QStringLiteral("bold"), yesNo(true));
    if (f.italic)
        addAttr(open, QStringLiteral("italic"), yesNo(true));
    if (f.underline)
        addAttr(open, QStringLiteral("underline"), yesNo(true));
    if (f.strikeout)
        addAttr(open, QStringLiteral("strikeout"), yesNo(true));
    out += open + QLatin1Char('>') + escText(f.family) + QLatin1String("</") + tag
        + QLatin1String(">\n");
}

void writeSettings(QString &out, const Map &map, int depth)
{
    const MapSettings &s = map.settings;
    out += indent(depth) + QLatin1String("<settings>\n");

    // colors
    static const char *const kColorNames[ColorCount] = {
        "canvas", "border", "line",     "selectedLine", "hoverLine", "subTitle",
        "smallText", "lineText", "grid", "startRoom",    "endRoom"};
    out += indent(depth + 1) + QLatin1String("<colors>\n");
    for (int i = 0; i < ColorCount; ++i)
        writeElementText(out, depth + 2, QString::fromLatin1(kColorNames[i]),
                         colorWithToken(s.colors[i], s.colorTokens[i]));
    out += indent(depth + 1) + QLatin1String("</colors>\n");

    // regions, ordered by name (Regions.OrderBy(RegionName))
    QList<Region> regions = map.regions;
    std::stable_sort(regions.begin(), regions.end(),
                     [](const Region &a, const Region &b) { return a.name < b.name; });
    out += indent(depth + 1) + QLatin1String("<regions>\n");
    for (const Region &r : regions) {
        const QString tag = encodeXmlName(r.name);
        QString open = indent(depth + 2) + QLatin1Char('<') + tag;
        addAttr(open, QStringLiteral("Name"), r.name);
        addAttr(open, QStringLiteral("TextColor"), colorWithToken(r.textColor, r.textColorToken));
        out += open + QLatin1Char('>') + escText(colorWithToken(r.fillColor, r.fillColorToken))
            + QLatin1String("</") + tag + QLatin1String(">\n");
    }
    out += indent(depth + 1) + QLatin1String("</regions>\n");

    // fonts
    out += indent(depth + 1) + QLatin1String("<fonts>\n");
    writeFont(out, depth + 2, QStringLiteral("room"), s.roomFont);
    writeFont(out, depth + 2, QStringLiteral("object"), s.objectFont);
    writeFont(out, depth + 2, QStringLiteral("subTitle"), s.subtitleFont);
    writeFont(out, depth + 2, QStringLiteral("line"), s.lineFont);
    out += indent(depth + 1) + QLatin1String("</fonts>\n");

    // grid
    out += indent(depth + 1) + QLatin1String("<grid>\n");
    writeElementText(out, depth + 2, QStringLiteral("snapTo"), yesNo(s.snapToGrid));
    writeElementText(out, depth + 2, QStringLiteral("visible"), yesNo(s.gridVisible));
    writeElementText(out, depth + 2, QStringLiteral("showOrigin"), yesNo(s.showOrigin));
    writeElementText(out, depth + 2, QStringLiteral("size"), fmtFloat(s.gridSize));
    out += indent(depth + 1) + QLatin1String("</grid>\n");

    // lines
    out += indent(depth + 1) + QLatin1String("<lines>\n");
    writeElementText(out, depth + 2, QStringLiteral("width"), fmtFloat(s.lineWidth));
    writeElementText(out, depth + 2, QStringLiteral("arrowSize"), fmtFloat(s.arrowSize));
    writeElementText(out, depth + 2, QStringLiteral("textOffset"), fmtFloat(s.textOffset));
    out += indent(depth + 1) + QLatin1String("</lines>\n");

    // rooms
    out += indent(depth + 1) + QLatin1String("<rooms>\n");
    writeElementText(out, depth + 2, QStringLiteral("darknessStripeSize"),
                     fmtFloat(s.darknessStripeSize));
    writeElementText(out, depth + 2, QStringLiteral("objectListOffset"),
                     fmtFloat(s.objectListOffset));
    writeElementText(out, depth + 2, QStringLiteral("connectionStalkLength"),
                     fmtFloat(s.connectionStalkLength));
    writeElementText(out, depth + 2, QStringLiteral("preferredDistanceBetweenRooms"),
                     fmtFloat(s.preferredDistanceBetweenRooms));
    writeElementText(out, depth + 2, QStringLiteral("defaultRoomName"), s.defaultRoomName);
    writeElementText(out, depth + 2, QStringLiteral("defaultRoomShape"),
                     QString::number(s.defaultRoomShape));
    out += indent(depth + 1) + QLatin1String("</rooms>\n");

    // ui
    out += indent(depth + 1) + QLatin1String("<ui>\n");
    writeElementText(out, depth + 2, QStringLiteral("handleSize"), fmtFloat(s.handleSize));
    writeElementText(out, depth + 2, QStringLiteral("snapToElementSize"),
                     fmtFloat(s.snapToElementSize));
    out += indent(depth + 1) + QLatin1String("</ui>\n");

    // margins
    out += indent(depth + 1) + QLatin1String("<margins>\n");
    writeElementText(out, depth + 2, QStringLiteral("documentSpecific"),
                     yesNo(s.documentSpecificMargins));
    writeElementText(out, depth + 2, QStringLiteral("horizontal"), fmtFloat(s.horizontalMargin));
    writeElementText(out, depth + 2, QStringLiteral("vertical"), fmtFloat(s.verticalMargin));
    writeElementText(out, depth + 2, QStringLiteral("wrapDashes"), yesNo(s.wrapTextAtDashes));
    out += indent(depth + 1) + QLatin1String("</margins>\n");

    // keypadNavigation
    out += indent(depth + 1) + QLatin1String("<keypadNavigation>\n");
    writeElementText(out, depth + 2, QStringLiteral("creationModifier"),
                     s.keypadCreationModifier);
    writeElementText(out, depth + 2, QStringLiteral("unexploredModifier"),
                     s.keypadUnexploredModifier);
    out += indent(depth + 1) + QLatin1String("</keypadNavigation>\n");

    out += indent(depth) + QLatin1String("</settings>\n");
}

} // namespace

QString TrizbortWriter::toString(const Map &map)
{
    QString out;
    out += QLatin1String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

    const QString version = map.version.isEmpty() ? QStringLiteral("1.8.0.0") : map.version;
    out += QLatin1String("<trizbort");
    addAttr(out, QStringLiteral("version"), version);
    out += QLatin1String(">\n");

    // <info>
    const bool anyInfo = !map.title.isEmpty() || !map.author.isEmpty()
        || !map.description.isEmpty() || !map.history.isEmpty();
    if (!anyInfo) {
        out += indent(1) + QLatin1String("<info />\n");
    } else {
        out += indent(1) + QLatin1String("<info>\n");
        if (!map.title.isEmpty())
            writeElementText(out, 2, QStringLiteral("title"), map.title);
        if (!map.author.isEmpty())
            writeElementText(out, 2, QStringLiteral("author"), map.author);
        if (!map.description.isEmpty())
            writeElementText(out, 2, QStringLiteral("description"), map.description);
        if (!map.history.isEmpty())
            writeElementText(out, 2, QStringLiteral("history"), map.history);
        out += indent(1) + QLatin1String("</info>\n");
    }

    // <map>: rooms and lines are emitted in their original interleaved order
    // (by seq), so a load/save round trip preserves the element ordering.
    if (map.rooms.isEmpty() && map.connections.isEmpty()) {
        out += indent(1) + QLatin1String("<map />\n");
    } else {
        out += indent(1) + QLatin1String("<map>\n");
        int ri = 0;
        int ci = 0;
        while (ri < map.rooms.size() || ci < map.connections.size()) {
            const bool takeRoom =
                ci >= map.connections.size()
                || (ri < map.rooms.size() && map.rooms.at(ri).seq <= map.connections.at(ci).seq);
            if (takeRoom)
                writeRoom(out, map.rooms.at(ri++), 2);
            else
                writeLine(out, map.connections.at(ci++), 2);
        }
        out += indent(1) + QLatin1String("</map>\n");
    }

    // <settings>
    writeSettings(out, map, 1);

    // No trailing newline after the root element, matching Trizbort's output.
    out += QLatin1String("</trizbort>");
    return out;
}

bool TrizbortWriter::save(const QString &path, const Map &map, QString *errorMessage)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (errorMessage)
            *errorMessage = QObject::tr("Cannot write %1: %2").arg(path, file.errorString());
        return false;
    }
    // A UTF-8 BOM, matching Trizbort's files (and the on-disk samples). Line
    // endings stay LF, as the repository stores them.
    QByteArray bytes = QByteArray::fromRawData("\xEF\xBB\xBF", 3) + toString(map).toUtf8();
    if (file.write(bytes) != bytes.size()) {
        if (errorMessage)
            *errorMessage = QObject::tr("Error writing %1: %2").arg(path, file.errorString());
        return false;
    }
    return true;
}

} // namespace trizbort
