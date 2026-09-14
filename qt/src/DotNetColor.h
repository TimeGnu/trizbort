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

#ifndef TRIZBORT_DOTNETCOLOR_H
#define TRIZBORT_DOTNETCOLOR_H

#include <QColor>
#include <QString>

namespace trizbort {

// System.Drawing.ColorTranslator.ToHtml: a known colour by name, else #RRGGBB.
// Used for the settings palette and region colours, which Trizbort serialises
// this way. (Room/connection colours use SaveColor/ToHex instead — always hex.)
QString dotNetColorToHtml(const QColor &c);

} // namespace trizbort

#endif // TRIZBORT_DOTNETCOLOR_H
