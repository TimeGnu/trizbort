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

Room readRoom(QXmlStreamReader &xml)
{
    const QXmlStreamAttributes a = xml.attributes();
    Room r;
    r.id = attr(a, "id").toInt();
    r.name = attr(a, "name");
    r.subtitle = attr(a, "subtitle");
    r.description = attr(a, "description");
    r.region = attr(a, "region");
    r.x = attr(a, "x").toDouble();
    r.y = attr(a, "y").toDouble();
    if (a.hasAttribute(QLatin1String("w")))
        r.w = attr(a, "w").toDouble();
    if (a.hasAttribute(QLatin1String("h")))
        r.h = attr(a, "h").toDouble();
    const QString bs = attr(a, "borderstyle");
    if (!bs.isEmpty())
        r.borderStyle = bs;
    r.fill = parseTrizbortColor(attr(a, "roomFill"));
    r.secondFill = parseTrizbortColor(attr(a, "secondFill"));
    r.border = parseTrizbortColor(attr(a, "roomBorder"));
    r.largeText = parseTrizbortColor(attr(a, "roomLargeText"));
    r.smallText = parseTrizbortColor(attr(a, "roomSmallText"));
    const QString dark = attr(a, "isDark").toLower();
    r.isDark = (dark == QLatin1String("yes") || dark == QLatin1String("true"));
    const QString start = attr(a, "isStartRoom").toLower();
    r.isStartRoom = (start == QLatin1String("yes") || start == QLatin1String("true"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("objects")) {
            // Trizbort encodes newlines in the objects text as '|' (and an
            // escaped literal pipe as '\|'); decode exactly as Room.cs:1162
            // does. Indentation denotes containment and [..] encodes
            // properties; the exporters parse that themselves.
            QString text = xml.readElementText();
            text.replace(QLatin1String("|"), QLatin1String("\r\n"));
            text.replace(QLatin1String("\\\r\n"), QLatin1String("|"));
            r.objectsText = text;
        } else {
            xml.skipCurrentElement();
        }
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
            v.point = QPointF(attr(va, "x").toDouble(), attr(va, "y").toDouble());
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
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("room"))
            out.rooms.append(readRoom(xml));
        else if (n == QLatin1String("line"))
            out.connections.append(readLine(xml));
        else
            xml.skipCurrentElement();
    }
}

void readColors(QXmlStreamReader &xml, Map &out)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("canvas"))
            out.canvasColor = parseTrizbortColor(xml.readElementText());
        else if (n == QLatin1String("line"))
            out.lineColor = parseTrizbortColor(xml.readElementText());
        else if (n == QLatin1String("border"))
            out.borderColor = parseTrizbortColor(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
}

void readRegions(QXmlStreamReader &xml, Map &out)
{
    while (xml.readNextStartElement()) {
        Region reg;
        const QXmlStreamAttributes a = xml.attributes();
        reg.name = attr(a, "Name");
        if (reg.name.isEmpty())
            reg.name = xml.name().toString(); // fall back to the element tag
        reg.textColor = parseTrizbortColor(attr(a, "TextColor"));
        reg.fillColor = parseTrizbortColor(xml.readElementText()); // consumes element
        out.regions.append(reg);
    }
}

void readSettings(QXmlStreamReader &xml, Map &out)
{
    while (xml.readNextStartElement()) {
        const auto n = xml.name();
        if (n == QLatin1String("colors"))
            readColors(xml, out);
        else if (n == QLatin1String("regions"))
            readRegions(xml, out);
        else
            xml.skipCurrentElement();
    }
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

    out.clear();
    QXmlStreamReader xml(&file);

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
                                .arg(path)
                                .arg(xml.lineNumber())
                                .arg(xml.errorString());
        out.clear();
        return false;
    }

    out.reindex();
    return true;
}

} // namespace trizbort
