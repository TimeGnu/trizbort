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

#ifndef TRIZBORT_SETTINGSDIALOG_H
#define TRIZBORT_SETTINGSDIALOG_H

#include <QDialog>

#include "MapDocument.h"

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QPushButton;
class QTableWidget;

namespace trizbort {

// Editor for the document's appearance settings (palette, grid, lines, room
// defaults) and its region list. result*() return the edited values.
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(const MapSettings &settings, const QList<Region> &regions,
                   QWidget *parent = nullptr);

    MapSettings resultSettings() const;
    QList<Region> resultRegions() const;
    // Original region name -> new name, for rows whose name changed; and the
    // names of original regions the user removed. Used to fix up rooms.
    QHash<QString, QString> regionRenames() const;
    QStringList removedRegionNames() const;

protected:
    void accept() override; // validates region names first

private:
    QWidget *buildColorsTab();
    QWidget *buildGridLinesTab();
    QWidget *buildRoomsTab();
    QWidget *buildFontsTab();
    QWidget *buildAdvancedTab();
    QWidget *buildRegionsTab();
    void setButtonColor(QPushButton *button, const QColor &color);
    QColor buttonColor(const QPushButton *button) const;
    void setupFontButton(QPushButton *button, FontSpec *spec);

    MapSettings m_settings;
    QList<Region> m_regions;

    QPushButton *m_colorButtons[ColorCount] = {};

    QDoubleSpinBox *m_gridSize = nullptr;
    QCheckBox *m_gridVisible = nullptr;
    QCheckBox *m_snapToGrid = nullptr;
    QCheckBox *m_showOrigin = nullptr;
    QDoubleSpinBox *m_lineWidth = nullptr;
    QDoubleSpinBox *m_arrowSize = nullptr;
    QDoubleSpinBox *m_textOffset = nullptr;

    QLineEdit *m_defaultRoomName = nullptr;
    QComboBox *m_defaultRoomShape = nullptr;
    QDoubleSpinBox *m_darknessStripe = nullptr;
    QDoubleSpinBox *m_objectListOffset = nullptr;
    QDoubleSpinBox *m_stalkLength = nullptr;
    QDoubleSpinBox *m_preferredDistance = nullptr;

    // Fonts (edited copies, applied in resultSettings).
    FontSpec m_roomFont;
    FontSpec m_subtitleFont;
    FontSpec m_objectFont;
    FontSpec m_lineFont;
    QPushButton *m_roomFontButton = nullptr;
    QPushButton *m_subtitleFontButton = nullptr;
    QPushButton *m_objectFontButton = nullptr;
    QPushButton *m_lineFontButton = nullptr;

    // Advanced.
    QDoubleSpinBox *m_handleSize = nullptr;
    QDoubleSpinBox *m_snapToElement = nullptr;
    QCheckBox *m_docMargins = nullptr;
    QDoubleSpinBox *m_hMargin = nullptr;
    QDoubleSpinBox *m_vMargin = nullptr;
    QCheckBox *m_wrapDashes = nullptr;

    QTableWidget *m_regionTable = nullptr;
};

} // namespace trizbort

#endif // TRIZBORT_SETTINGSDIALOG_H
