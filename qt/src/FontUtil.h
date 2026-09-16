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
 */

#ifndef TRIZBORT_FONTUTIL_H
#define TRIZBORT_FONTUTIL_H

#include <QFont>

#include "AppSettings.h"
#include "MapDocument.h"

namespace trizbort {

// Build a QFont from a stored FontSpec, applying the family, point size and
// style flags. When the spec carries no family or a non-positive size (an
// unspecified font), the given fallback point size and the default UI family
// are used instead. Sizes are treated as points, matching the C# GDI fonts the
// .trizbort format stores.
inline QFont qfontFromSpec(const FontSpec &spec, double fallbackPointSize)
{
    QFont font;
    if (!spec.family.isEmpty())
        font.setFamily(spec.family);
    else if (!AppSettings::instance().defaultFontName.isEmpty())
        font.setFamily(AppSettings::instance().defaultFontName); // the app-wide default family
    font.setPointSizeF(spec.size > 0.0 ? spec.size : fallbackPointSize);
    font.setBold(spec.bold);
    font.setItalic(spec.italic);
    font.setUnderline(spec.underline);
    font.setStrikeOut(spec.strikeout);
    return font;
}

// Honour the map's "wrap text at dashes" setting when laying out canvas text,
// mirroring TextBlock.cs. Qt's word wrap only guarantees breaks at whitespace,
// so this rewrites the break behaviour around hyphen-minus while keeping the
// exact glyph: with wrapping on, a zero-width space (U+200B) after each hyphen
// adds a break opportunity there; with it off, a word joiner (U+2060) removes
// one. Like the C# original, this only applies to single-line text (text with
// explicit newlines is drawn as authored).
inline QString applyDashWrapping(const QString &text, bool wrapAtDashes)
{
    if (!text.contains(QLatin1Char('-')) || text.contains(QLatin1Char('\n')))
        return text;
    QString out = text;
    out.replace(QLatin1Char('-'),
                wrapAtDashes ? QStringLiteral("-​") : QStringLiteral("-⁠"));
    return out;
}

} // namespace trizbort

#endif // TRIZBORT_FONTUTIL_H
