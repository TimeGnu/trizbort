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

#ifndef TRIZBORT_EXPORT_CODEEXPORTER_H
#define TRIZBORT_EXPORT_CODEEXPORTER_H

#include <optional>

#include <QHash>
#include <QList>
#include <QSet>
#include <QString>

#include "../MapDocument.h"
#include "ExportModel.h"

// Base class for the code exporters, ported from Export/CodeExporter.cs.
// Builds the intermediate model (regions, locations, exits, things) from a Map,
// then a subclass serializes it to a target language.
namespace trizbort {

class CodeExporter {
public:
    CodeExporter(const Map &map, const QString &sourcePath);
    virtual ~CodeExporter();

    // Runs the pipeline and returns the full exported source.
    QString exportToString();

protected:
    virtual QStringList reservedWords() const = 0;
    virtual void exportHeader(QString &out, const QString &title, const QString &author,
                              const QString &description, const QString &history) = 0;
    virtual void exportContent(QString &out) = 0;
    virtual QString getExportName(const Room &room, std::optional<int> suffix) = 0;
    virtual QString getExportName(const QString &displayName, std::optional<int> suffix) = 0;

    static QString deaccent(const QString &s);

    const Map &m_map;
    QString m_sourcePath;
    QList<Location *> m_locationsInExportOrder;
    QList<ExportRegion *> m_regionsInExportOrder;

private:
    void prepareContent();
    void findRegions();
    void findRooms();
    void findExits();
    void pickBestExits();
    void findThings();

    const Room *sourceRoom(const Connection &c, CompassPoint &cp) const;
    const Room *targetRoom(const Connection &c, CompassPoint &cp) const;

    QHash<int, Location *> m_roomIdToLocation;

    // Owned heap objects, freed in the destructor.
    QList<Location *> m_ownedLocations;
    QList<Exit *> m_ownedExits;
    QList<Thing *> m_ownedThings;
    QList<ExportRegion *> m_ownedRegions;
};

} // namespace trizbort

#endif // TRIZBORT_EXPORT_CODEEXPORTER_H
