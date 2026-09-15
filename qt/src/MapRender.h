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

#ifndef TRIZBORT_MAPRENDER_H
#define TRIZBORT_MAPRENDER_H

#include <QString>

#include "MapDocument.h"

// Renders a map to a raster image or a PDF, using the same drawing the editor
// canvas uses (minus the grid). Used by the File > Export menu and the CLI.
namespace trizbort {

bool renderMapToImage(const Map &map, const QString &path, QString *errorMessage = nullptr);
bool renderMapToPdf(const Map &map, const QString &path, QString *errorMessage = nullptr);

} // namespace trizbort

#endif // TRIZBORT_MAPRENDER_H
