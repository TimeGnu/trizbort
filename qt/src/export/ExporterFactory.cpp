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

#include "ExporterFactory.h"

#include "AdventuronExporter.h"
#include "AlanExporter.h"
#include "HugoExporter.h"
#include "Inform6Exporter.h"
#include "Inform7Exporter.h"
#include "QuestExporter.h"
#include "TadsExporter.h"
#include "ZilExporter.h"

namespace trizbort {

const QList<ExportFormat> &exportFormats()
{
    static const QList<ExportFormat> formats = {
        {QStringLiteral("zil"), QStringLiteral("ZIL (Infocom / ZILF)"), QStringLiteral("zil")},
        {QStringLiteral("inform7"), QStringLiteral("Inform 7"), QStringLiteral("ni")},
        {QStringLiteral("inform6"), QStringLiteral("Inform 6"), QStringLiteral("inf")},
        {QStringLiteral("tads"), QStringLiteral("TADS 3"), QStringLiteral("t")},
        {QStringLiteral("hugo"), QStringLiteral("Hugo"), QStringLiteral("hug")},
        {QStringLiteral("alan"), QStringLiteral("Alan"), QStringLiteral("i")},
        {QStringLiteral("adventuron"), QStringLiteral("Adventuron"), QStringLiteral("adv")},
        {QStringLiteral("quest"), QStringLiteral("Quest 5"), QStringLiteral("aslx")},
    };
    return formats;
}

std::unique_ptr<CodeExporter> makeExporter(const QString &fmt, const Map &map, const QString &path)
{
    if (fmt == QLatin1String("zil"))
        return std::make_unique<ZilExporter>(map, path);
    if (fmt == QLatin1String("adventuron"))
        return std::make_unique<AdventuronExporter>(map, path);
    if (fmt == QLatin1String("hugo"))
        return std::make_unique<HugoExporter>(map, path);
    if (fmt == QLatin1String("alan"))
        return std::make_unique<AlanExporter>(map, path);
    if (fmt == QLatin1String("tads"))
        return std::make_unique<TadsExporter>(map, path);
    if (fmt == QLatin1String("inform6"))
        return std::make_unique<Inform6Exporter>(map, path);
    if (fmt == QLatin1String("inform7"))
        return std::make_unique<Inform7Exporter>(map, path);
    if (fmt == QLatin1String("quest"))
        return std::make_unique<QuestExporter>(map, path);
    return nullptr;
}

} // namespace trizbort
