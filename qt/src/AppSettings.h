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

#ifndef TRIZBORT_APPSETTINGS_H
#define TRIZBORT_APPSETTINGS_H

#include <QString>

namespace trizbort {

// Application-wide preferences (the C# ApplicationSettingsController.AppSettings),
// shared as a single instance so rendering code — tooltips, hand-drawn defaults,
// export margins, port granularity — can read them without threading a pointer
// through every object. MainWindow loads and saves these via QSettings; the
// defaults here match the C# ones so headless tools behave like a fresh install.
struct AppSettings {
    // Tooltips.
    bool showObjectsInTooltips = true;
    bool showDescriptionsInTooltips = true;
    bool limitRoomDescriptionChars = false;
    int roomDescriptionChars = 50;
    bool limitConnectionDescriptionChars = false;
    int connectionDescriptionChars = 50;

    // Rendering.
    bool handDrawnGlobal = false;      // seeds new rooms' hand-drawn flag
    int portAdjustDetail = 16;         // compass points offered when docking: 16/8/4
    QString defaultFontName = QStringLiteral("Arial");

    // Canvas interaction.
    bool infiniteScrollBounds = false; // scroll far beyond the content bounds

    // Export.
    bool saveTadsToAdv3Lite = true;    // TADS export target: adv3Lite vs adv3

    // General export margins, used when a document has no document-specific
    // margins of its own.
    bool specifyGenMargins = false;
    double genHorizontalMargin = 0.0;
    double genVerticalMargin = 0.0;

    bool showTooltips() const { return showObjectsInTooltips || showDescriptionsInTooltips; }

    static AppSettings &instance()
    {
        static AppSettings s;
        return s;
    }
};

} // namespace trizbort

#endif // TRIZBORT_APPSETTINGS_H
