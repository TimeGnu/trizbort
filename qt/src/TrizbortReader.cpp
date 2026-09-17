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

#include "TrizbortReader.h"

#include <QFile>
#include <QObject>
#include <QXmlStreamReader>

namespace trizbort {

QColor parseTrizbortColor(const QString &text)
{
    const QString t = text.trimmed();
    if (t.isEmpty())
        return QColor(); // invalid => inherit default
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    return QColor::fromString(t);
#else
    QColor c;
    c.setNamedColor(t);
    return c;
#endif
}

namespace {

QString attr(const QXmlStreamAttributes &a, const char *name)
{
    return a.value(QLatin1String(name)).toString();
}

// Trizbort persists booleans as "yes"/"no" (XmlScribe); tolerate "true"/"1" too.
bool toBool(const QString &text, bool defaultValue = false)
{
    const QString t = text.trimmed().toLower();
    if (t.isEmpty())
        return defaultValue;
    return t == QLatin1String("yes") || t == QLatin1String("true") || t == QLatin1String("1");
}

double toDouble(const QString &text, double defaultValue = 0.0)
{
    bool ok = false;
    const double v = text.toDouble(&ok);
    return ok ? v : defaultValue;
}

void readObjects(QXmlStreamReader &xml, Room &r)
{
    const QXmlStreamAttributes a = xml.attributes();
    r.objectsPosition.at = attr(a, "at");
    r.objectsPosition.custom = toBool(attr(a, "custom"));
    r.objectsPosition.customRight = attr(a, "customRight").toInt();
    r.objectsPosition.customDown = attr(a, "customDown").toInt();

    // Trizbort encodes newlines in the objects text as '|' (and an escaped
    // literal pipe as '\|'); decode exactly as Room.cs:1162 does. Indentation
    // denotes containment and [..] encodes properties; the exporters parse that.
    QString text = xml.readElementText();
    text.replace(QLatin1String("|"), QLatin1String("\r\n"));
    text.replace(QLatin1String("\\\r\n"), QLatin1String("|"));
    r.objectsText = text;
}

// Files older than 1.5.8.3 stored roomFill="#FFFFFF" to mean "no fill"; such a
// room's colours are all ignored on load, exactly as Room.cs does for that
// version range.
bool isPreColorFixVersion(const QString &version)
{
    if (version.isEmpty())
        return false;
    const QStringList parts = version.split(QLatin1Char('.'));
    const int target[4] = {1, 5, 8, 3};
    for (int i = 0; i < 4; ++i) {
        const int v = i < parts.size() ? parts.at(i).toInt() : 0;
        if (v != target[i])
            return v < target[i];
    }
    return false; // exactly 1.5.8.3 => already fixed
}

Room readRoom(QXmlStreamReader &xml, bool legacyWhiteFillQuirk)
{
    const QXmlStreamAttributes a = xml.attributes();
    Room r;
    r.id = attr(a, "id").toInt();
    r.name = attr(a, "name");
    r.subtitle = attr(a, "subtitle");
    r.description = attr(a, "description");
    // Raw value, empty when the attribute is absent, matching Room.cs:1105
    // (element.Attribute("region").Text). The exporters treat "" and "NoRegion"
    // differently, so this must not be normalised on load.
    r.region = attr(a, "region");
    if (a.hasAttribute(QLatin1String("referenceRoom")))
        r.referenceRoom = attr(a, "referenceRoom").toInt();

    r.x = toDouble(attr(a, "x"));
    r.y = toDouble(attr(a, "y"));
    if (a.hasAttribute(QLatin1String("w")))
        r.w = toDouble(attr(a, "w"), r.w);
    if (a.hasAttribute(QLatin1String("h")))
        r.h = toDouble(attr(a, "h"), r.h);

    r.handDrawn = toBool(attr(a, "handDrawn"));
    r.allCornersEqual = toBool(attr(a, "allcornersequal"));
    r.ellipse = toBool(attr(a, "ellipse"));
    r.roundedCorners = toBool(attr(a, "roundedCorners"));
    r.octagonal = toBool(attr(a, "octagonal"));
    r.cornerTopLeft = toDouble(attr(a, "cornerTopLeft"));
    r.cornerTopRight = toDouble(attr(a, "cornerTopRight"));
    r.cornerBottomLeft = toDouble(attr(a, "cornerBottomLeft"));
    r.cornerBottomRight = toDouble(attr(a, "cornerBottomRight"));

    const QString bs = attr(a, "borderstyle");
    if (!bs.isEmpty())
        r.borderStyle = bs;

    r.isDark = toBool(attr(a, "isDark"));
    r.isStartRoom = toBool(attr(a, "isStartRoom"));
    r.isEndRoom = toBool(attr(a, "isEndRoom"));

    // For pre-1.5.8.3 files a white roomFill means "no colours": leave every
    // room colour at its inherited default (Room.cs:1145-1151).
    if (!(legacyWhiteFillQuirk &&
          attr(a, "roomFill") == QLatin1String("#FFFFFF"))) {
        r.fill = parseTrizbortColor(attr(a, "roomFill"));
        r.secondFill = parseTrizbortColor(attr(a, "secondFill"));
        if (a.hasAttribute(QLatin1String("secondFillLocation"))) {
            const QString loc = attr(a, "secondFillLocation");
            if (!loc.isEmpty())
                r.secondFillLocation = loc;
        }
        r.border = parseTrizbortColor(attr(a, "roomBorder"));
        r.largeText = parseTrizbortColor(attr(a, "roomLargeText"));
        r.subtitleColor = parseTrizbortColor(attr(a, "roomSubtitleColor"));
        r.smallText = parseTrizbortColor(attr(a, "roomSmallText"));
    }

    if (a.hasAttribute(QLatin1String("ZOrder")))
        r.zOrder = attr(a, "ZOrder").toInt();

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("objects"))
            readObjects(xml, r);
        else
            xml.skipCurrentElement();
    }
    return r;
}

Connection readLine(QXmlStreamReader &xml)
{
    const QXmlStreamAttributes a = xml.attributes();
    Connection c;
    c.id = attr(a, "id").toInt();
    c.style = (attr(a, "style") == QLatin1String("dashed")) ? ConnectionStyle::Dashed
                                                            : ConnectionStyle::Solid;
    c.flow = (attr(a, "flow") == QLatin1String("oneWay")) ? ConnectionFlow::OneWay
                                                          : ConnectionFlow::TwoWay;
    c.name = attr(a, "name");
    c.description = attr(a, "description");
    c.startText = attr(a, "startText");
    c.midText = attr(a, "midText");
    c.endText = attr(a, "endText");
    c.color = parseTrizbortColor(attr(a, "color"));
    if (toBool(attr(a, "door"))) {
        c.hasDoor = true;
        c.door.lockable = toBool(attr(a, "lockable"));
        c.door.openable = toBool(attr(a, "openable"));
        c.door.locked = toBool(attr(a, "locked"));
        c.door.open = toBool(attr(a, "open"));
    }

    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("dock")) {
            const QXmlStreamAttributes va = xml.attributes();
            Vertex v;
            v.index = attr(va, "index").toInt();
            v.docked = true;
            v.roomId = attr(va, "id").toInt();
            v.port = attr(va, "port");
            c.vertices.append(v);
            xml.skipCurrentElement();
        } else if (n == QLatin1String("point")) {
            const QXmlStreamAttributes va = xml.attributes();
            Vertex v;
            v.index = attr(va, "index").toInt();
            v.docked = false;
            v.point = QPointF(toDouble(attr(va, "x")), toDouble(attr(va, "y")));
            c.vertices.append(v);
            xml.skipCurrentElement();
        } else {
            xml.skipCurrentElement();
        }
    }
    return c;
}

void readInfo(QXmlStreamReader &xml, Map &out)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("title"))
            out.title = xml.readElementText();
        else if (n == QLatin1String("author"))
            out.author = xml.readElementText();
        else if (n == QLatin1String("description"))
            out.description = xml.readElementText();
        else if (n == QLatin1String("history"))
            out.history = xml.readElementText();
        else
            xml.skipCurrentElement();
    }
}

void readMap(QXmlStreamReader &xml, Map &out)
{
    int seq = 0;
    const bool legacyWhiteFill = isPreColorFixVersion(out.version);
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("room")) {
            Room r = readRoom(xml, legacyWhiteFill);
            r.seq = ++seq;
            out.rooms.append(r);
        } else if (n == QLatin1String("line")) {
            Connection c = readLine(xml);
            c.seq = ++seq;
            out.connections.append(c);
        } else {
            xml.skipCurrentElement();
        }
    }
}

// Maps a <colors> child element name to its palette slot (Colors.Names in C#).
// Names not in this table (e.g. legacy "fill"/"largeText") are ignored, exactly
// as Colors.FromName does.
int colorSlotFromName(const QStringView name)
{
    static const struct {
        QLatin1String tag;
        ColorSlot slot;
    } kSlots[] = {
        {QLatin1String("canvas"), ColorCanvas},
        {QLatin1String("border"), ColorBorder},
        {QLatin1String("line"), ColorLine},
        {QLatin1String("selectedLine"), ColorSelectedLine},
        {QLatin1String("hoverLine"), ColorHoverLine},
        {QLatin1String("subTitle"), ColorSubtitle},
        {QLatin1String("smallText"), ColorSmallText},
        {QLatin1String("lineText"), ColorLineText},
        {QLatin1String("grid"), ColorGrid},
        {QLatin1String("startRoom"), ColorStartRoom},
        {QLatin1String("endRoom"), ColorEndRoom},
    };
    for (const auto &s : kSlots) {
        if (name.compare(s.tag, Qt::CaseInsensitive) == 0)
            return s.slot;
    }
    return -1;
}

void readColors(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const int slot = colorSlotFromName(xml.name());
        const QString token = xml.readElementText();
        const QColor c = parseTrizbortColor(token);
        if (slot >= 0 && c.isValid()) {
            s.colors[slot] = c;
            s.colorTokens[slot] = token; // preserve exact serialized form
        }
    }
}

void readRegions(QXmlStreamReader &xml, Map &out)
{
    while (xml.readNextStartElement()) {
        Region reg;
        const QXmlStreamAttributes a = xml.attributes();
        QString name = attr(a, "Name");
        if (name.isEmpty())
            name = xml.name().toString(); // fall back to the element tag
        reg.name = name.replace(QLatin1String("____"), QLatin1String(" ")); // ClearRegionNameObfuscation
        const QString tc = attr(a, "TextColor");
        reg.textColor = tc.isEmpty() ? QColor(0, 0, 255) : parseTrizbortColor(tc); // default Blue
        reg.textColorToken = tc;
        const QString fillToken = xml.readElementText();               // consumes element
        const QColor fill = parseTrizbortColor(fillToken);
        reg.fillColor = fill.isValid() ? fill : QColor(255, 255, 255); // default White
        reg.fillColorToken = fillToken;
        out.regions.append(reg);
    }
}

void readFontSpec(QXmlStreamReader &xml, FontSpec &f)
{
    const QXmlStreamAttributes a = xml.attributes();
    f.size = toDouble(attr(a, "size"), f.size);
    f.bold = toBool(attr(a, "bold"));
    f.italic = toBool(attr(a, "italic"));
    f.underline = toBool(attr(a, "underline"));
    f.strikeout = toBool(attr(a, "strikeout"));
    const QString family = xml.readElementText();
    if (!family.isEmpty())
        f.family = family;
}

void readFonts(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("room"))
            readFontSpec(xml, s.roomFont);
        else if (n == QLatin1String("object"))
            readFontSpec(xml, s.objectFont);
        else if (n == QLatin1String("subTitle"))
            readFontSpec(xml, s.subtitleFont);
        else if (n == QLatin1String("line"))
            readFontSpec(xml, s.lineFont);
        else
            xml.skipCurrentElement();
    }
}

void readGrid(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("snapTo"))
            s.snapToGrid = toBool(xml.readElementText(), s.snapToGrid);
        else if (n == QLatin1String("visible"))
            s.gridVisible = toBool(xml.readElementText(), s.gridVisible);
        else if (n == QLatin1String("showOrigin"))
            s.showOrigin = toBool(xml.readElementText(), s.showOrigin);
        else if (n == QLatin1String("size"))
            s.gridSize = toDouble(xml.readElementText(), s.gridSize);
        else
            xml.skipCurrentElement();
    }
}

void readLines(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("width"))
            s.lineWidth = toDouble(xml.readElementText(), s.lineWidth);
        else if (n == QLatin1String("arrowSize"))
            s.arrowSize = toDouble(xml.readElementText(), s.arrowSize);
        else if (n == QLatin1String("textOffset"))
            s.textOffset = toDouble(xml.readElementText(), s.textOffset);
        else
            xml.skipCurrentElement();
    }
}

void readRooms(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("darknessStripeSize"))
            s.darknessStripeSize = toDouble(xml.readElementText(), s.darknessStripeSize);
        else if (n == QLatin1String("objectListOffset"))
            s.objectListOffset = toDouble(xml.readElementText(), s.objectListOffset);
        else if (n == QLatin1String("connectionStalkLength"))
            s.connectionStalkLength = toDouble(xml.readElementText(), s.connectionStalkLength);
        else if (n == QLatin1String("preferredDistanceBetweenRooms"))
            s.preferredDistanceBetweenRooms =
                toDouble(xml.readElementText(), s.preferredDistanceBetweenRooms);
        else if (n == QLatin1String("defaultRoomName")) {
            const QString name = xml.readElementText();
            s.defaultRoomName = name.isEmpty() ? QStringLiteral("Cave") : name;
        } else if (n == QLatin1String("defaultRoomShape"))
            s.defaultRoomShape = xml.readElementText().toInt();
        else
            xml.skipCurrentElement();
    }
}

void readUi(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("handleSize"))
            s.handleSize = toDouble(xml.readElementText(), s.handleSize);
        else if (n == QLatin1String("snapToElementSize"))
            s.snapToElementSize = toDouble(xml.readElementText(), s.snapToElementSize);
        else
            xml.skipCurrentElement();
    }
}

void readMargins(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("documentSpecific"))
            s.documentSpecificMargins = toBool(xml.readElementText(), s.documentSpecificMargins);
        else if (n == QLatin1String("horizontal"))
            s.horizontalMargin = toDouble(xml.readElementText(), s.horizontalMargin);
        else if (n == QLatin1String("vertical"))
            s.verticalMargin = toDouble(xml.readElementText(), s.verticalMargin);
        else if (n == QLatin1String("wrapDashes"))
            s.wrapTextAtDashes = toBool(xml.readElementText(), s.wrapTextAtDashes);
        else
            xml.skipCurrentElement();
    }
}

void readKeypad(QXmlStreamReader &xml, MapSettings &s)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("creationModifier")) {
            const QString v = xml.readElementText();
            if (!v.isEmpty())
                s.keypadCreationModifier = v;
        } else if (n == QLatin1String("unexploredModifier")) {
            const QString v = xml.readElementText();
            if (!v.isEmpty())
                s.keypadUnexploredModifier = v;
        } else
            xml.skipCurrentElement();
    }
}

void readSettings(QXmlStreamReader &xml, Map &out)
{
    out.settings.loaded = true;
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("colors"))
            readColors(xml, out.settings);
        else if (n == QLatin1String("regions"))
            readRegions(xml, out);
        else if (n == QLatin1String("fonts"))
            readFonts(xml, out.settings);
        else if (n == QLatin1String("grid"))
            readGrid(xml, out.settings);
        else if (n == QLatin1String("lines"))
            readLines(xml, out.settings);
        else if (n == QLatin1String("rooms"))
            readRooms(xml, out.settings);
        else if (n == QLatin1String("ui"))
            readUi(xml, out.settings);
        else if (n == QLatin1String("margins"))
            readMargins(xml, out.settings);
        else if (n == QLatin1String("keypadNavigation"))
            readKeypad(xml, out.settings);
        else
            xml.skipCurrentElement();
    }
}

// Parse a document from an already-opened stream reader into out. `source` is a
// human-readable label used in error messages.
bool parseDocument(QXmlStreamReader &xml, Map &out, const QString &source, QString *errorMessage)
{
    out.clear();

    // A document has exactly one root element: read it once, then descend.
    if (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("trizbort")) {
            out.version = attr(xml.attributes(), "version");
            while (xml.readNextStartElement()) {
                const auto c = xml.name();
                if (c == QLatin1String("info"))
                    readInfo(xml, out);
                else if (c == QLatin1String("map"))
                    readMap(xml, out);
                else if (c == QLatin1String("settings"))
                    readSettings(xml, out);
                else
                    xml.skipCurrentElement();
            }
        } else {
            xml.raiseError(
                QObject::tr("Not a Trizbort file (root element is <%1>)").arg(xml.name().toString()));
        }
    }

    if (xml.hasError()) {
        if (errorMessage)
            *errorMessage = QObject::tr("Parse error in %1 (line %2): %3")
                                .arg(source)
                                .arg(xml.lineNumber())
                                .arg(xml.errorString());
        out.clear();
        return false;
    }

    // If the file carried no <regions>, keep the default NoRegion, matching
    // Settings.Load's behaviour of seeding a single default region.
    if (out.regions.isEmpty())
        out.regions.append(Region{kNoRegion, QColor(0, 0, 255), QColor(255, 255, 255)});

    out.reindex();
    return true;
}

} // namespace

bool TrizbortReader::load(const QString &path, Map &out, QString *errorMessage)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (errorMessage)
            *errorMessage = QObject::tr("Cannot open %1: %2").arg(path, file.errorString());
        return false;
    }
    QXmlStreamReader xml(&file);
    return parseDocument(xml, out, path, errorMessage);
}

bool TrizbortReader::loadFromString(const QString &xmlText, Map &out, QString *errorMessage)
{
    QXmlStreamReader xml(xmlText);
    return parseDocument(xml, out, QObject::tr("clipboard"), errorMessage);
}

} // namespace trizbort
