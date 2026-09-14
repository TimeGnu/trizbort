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

#ifndef TRIZBORT_EXPORT_EXPORTERFACTORY_H
#define TRIZBORT_EXPORT_EXPORTERFACTORY_H

#include <memory>

#include <QList>
#include <QString>

#include "../MapDocument.h"
#include "CodeExporter.h"

namespace trizbort {

// One selectable export target: the CLI/format key, a menu label, and the file
// extension its output uses.
struct ExportFormat {
    QString key;      // "zil", "inform7", ...
    QString label;    // "ZIL (Infocom)"
    QString extension; // "zil", "ni", ...
};

// The eight code-generation targets, in menu order.
const QList<ExportFormat> &exportFormats();

// Construct an exporter for the given format key, or nullptr if unknown.
std::unique_ptr<CodeExporter> makeExporter(const QString &fmt, const Map &map,
                                           const QString &path);

} // namespace trizbort

#endif // TRIZBORT_EXPORT_EXPORTERFACTORY_H
