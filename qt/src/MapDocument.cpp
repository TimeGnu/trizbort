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

#include "MapDocument.h"

namespace trizbort {

void Map::clear()
{
    version.clear();
    title.clear();
    author.clear();
    description.clear();
    history.clear();
    rooms.clear();
    connections.clear();
    regions.clear();
    canvasColor = QColor();
    lineColor = QColor();
    borderColor = QColor();
    m_roomIndex.clear();
}

void Map::reindex()
{
    m_roomIndex.clear();
    for (int i = 0; i < rooms.size(); ++i)
        m_roomIndex.insert(rooms.at(i).id, i);
}

const Room *Map::roomById(int id) const
{
    const auto it = m_roomIndex.constFind(id);
    if (it == m_roomIndex.constEnd())
        return nullptr;
    return &rooms.at(it.value());
}

QColor Map::regionFill(const QString &name) const
{
    if (name.isEmpty() || name == QLatin1String("NoRegion"))
        return QColor();
    for (const Region &r : regions) {
        if (r.name == name)
            return r.fillColor;
    }
    return QColor();
}

} // namespace trizbort
