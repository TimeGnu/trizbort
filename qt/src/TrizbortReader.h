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

#ifndef TRIZBORT_TRIZBORTREADER_H
#define TRIZBORT_TRIZBORTREADER_H

#include <QString>

#include "MapDocument.h"

namespace trizbort {

// Loads the .trizbort XML format documented in
// qt/docs/trizbort-file-format.md into a Map. Returns false and sets
// *errorMessage on failure.
class TrizbortReader {
public:
    static bool load(const QString &path, Map &out, QString *errorMessage = nullptr);
};

// Parses a Trizbort color string ("#RRGGBB", a named color, or ""). An empty
// or unrecognized string yields an invalid QColor, meaning "inherit default".
QColor parseTrizbortColor(const QString &text);

} // namespace trizbort

#endif // TRIZBORT_TRIZBORTREADER_H
