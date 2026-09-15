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

#ifndef TRIZBORT_EXPORT_INFORM6EXPORTER_H
#define TRIZBORT_EXPORT_INFORM6EXPORTER_H

#include "CodeExporter.h"

// Inform 6 exporter, ported from Inform6Exporter.cs.
namespace trizbort {

class Inform6Exporter : public CodeExporter {
public:
    using CodeExporter::CodeExporter;

protected:
    QStringList reservedWords() const override;
    void exportHeader(QString &out, const QString &title, const QString &author,
                      const QString &description, const QString &history) override;
    void exportContent(QString &out) override;
    QString getExportName(const Room &room, std::optional<int> suffix) override;
    QString getExportName(const QString &displayName, std::optional<int> suffix) override;

private:
    void writeLocation(QString &out, Location *location);
    void writeDoor(QString &out, Location *location, MappableDirection direction, Exit *exit);
};

} // namespace trizbort

#endif // TRIZBORT_EXPORT_INFORM6EXPORTER_H
