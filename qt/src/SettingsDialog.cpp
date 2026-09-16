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

#include "SettingsDialog.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFontDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QTabWidget>
#include <QVBoxLayout>

#include "FontUtil.h"

namespace trizbort {

namespace {
struct SlotLabel {
    ColorSlot slot;
    const char *label;
};
const SlotLabel kSlotLabels[] = {
    {ColorCanvas, "Canvas"},         {ColorBorder, "Room border"},
    {ColorLine, "Connection line"},  {ColorSelectedLine, "Selected line"},
    {ColorHoverLine, "Hover line"},  {ColorSubtitle, "Subtitle text"},
    {ColorSmallText, "Object text"}, {ColorLineText, "Connection text"},
    {ColorGrid, "Grid"},             {ColorStartRoom, "Start room"},
    {ColorEndRoom, "End room"},
};

QDoubleSpinBox *makeSpin(double value, double max = 4096.0)
{
    auto *spin = new QDoubleSpinBox;
    spin->setRange(0, max);
    spin->setDecimals(2);
    spin->setValue(value);
    return spin;
}
} // namespace

SettingsDialog::SettingsDialog(const MapSettings &settings, const QList<Region> &regions,
                               QWidget *parent)
    : QDialog(parent)
    , m_settings(settings)
    , m_regions(regions)
{
    setWindowTitle(tr("Map Settings"));
    resize(460, 480);

    m_roomFont = m_settings.roomFont;
    m_subtitleFont = m_settings.subtitleFont;
    m_objectFont = m_settings.objectFont;
    m_lineFont = m_settings.lineFont;

    auto *layout = new QVBoxLayout(this);
    auto *tabs = new QTabWidget(this);
    tabs->addTab(buildColorsTab(), tr("&Colours"));
    tabs->addTab(buildGridLinesTab(), tr("&Grid && Lines"));
    tabs->addTab(buildRoomsTab(), tr("&Rooms"));
    tabs->addTab(buildFontsTab(), tr("&Fonts"));
    tabs->addTab(buildAdvancedTab(), tr("Ad&vanced"));
    tabs->addTab(buildRegionsTab(), tr("Re&gions"));
    layout->addWidget(tabs);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void SettingsDialog::setButtonColor(QPushButton *button, const QColor &color)
{
    const QColor c = color.isValid() ? color : QColor(Qt::white);
    button->setProperty("swatch", c);
    button->setText(c.name().toUpper());
    const QString fg = (c.lightnessF() < 0.5) ? QStringLiteral("white") : QStringLiteral("black");
    button->setStyleSheet(
        QStringLiteral("background-color: %1; color: %2;").arg(c.name(), fg));
}

QColor SettingsDialog::buttonColor(const QPushButton *button) const
{
    return button->property("swatch").value<QColor>();
}

QWidget *SettingsDialog::buildColorsTab()
{
    auto *widget = new QWidget;
    auto *form = new QFormLayout(widget);
    for (const SlotLabel &sl : kSlotLabels) {
        auto *button = new QPushButton(widget);
        setButtonColor(button, m_settings.colors[sl.slot]);
        connect(button, &QPushButton::clicked, this, [this, button] {
            const QColor chosen = QColorDialog::getColor(buttonColor(button), this,
                                                         tr("Choose colour"));
            if (chosen.isValid())
                setButtonColor(button, chosen);
        });
        m_colorButtons[sl.slot] = button;
        form->addRow(tr(sl.label), button);
    }
    return widget;
}

QWidget *SettingsDialog::buildGridLinesTab()
{
    auto *widget = new QWidget;
    auto *form = new QFormLayout(widget);

    m_gridSize = makeSpin(m_settings.gridSize, 512);
    form->addRow(tr("Grid size:"), m_gridSize);
    m_gridVisible = new QCheckBox(tr("Show grid"), widget);
    m_gridVisible->setChecked(m_settings.gridVisible);
    form->addRow(QString(), m_gridVisible);
    m_snapToGrid = new QCheckBox(tr("Snap to grid"), widget);
    m_snapToGrid->setChecked(m_settings.snapToGrid);
    form->addRow(QString(), m_snapToGrid);
    m_showOrigin = new QCheckBox(tr("Show origin"), widget);
    m_showOrigin->setChecked(m_settings.showOrigin);
    form->addRow(QString(), m_showOrigin);

    m_lineWidth = makeSpin(m_settings.lineWidth, 64);
    form->addRow(tr("Line width:"), m_lineWidth);
    m_arrowSize = makeSpin(m_settings.arrowSize, 128);
    form->addRow(tr("Arrow size:"), m_arrowSize);
    m_textOffset = makeSpin(m_settings.textOffset, 128);
    form->addRow(tr("Text offset:"), m_textOffset);
    return widget;
}

QWidget *SettingsDialog::buildRoomsTab()
{
    auto *widget = new QWidget;
    auto *form = new QFormLayout(widget);

    m_defaultRoomName = new QLineEdit(m_settings.defaultRoomName, widget);
    form->addRow(tr("Default room name:"), m_defaultRoomName);

    m_defaultRoomShape = new QComboBox(widget);
    m_defaultRoomShape->addItems({tr("Square"), tr("Rounded"), tr("Ellipse"), tr("Octagonal")});
    m_defaultRoomShape->setCurrentIndex(
        qBound(0, m_settings.defaultRoomShape, m_defaultRoomShape->count() - 1));
    form->addRow(tr("Default room shape:"), m_defaultRoomShape);

    m_darknessStripe = makeSpin(m_settings.darknessStripeSize, 256);
    form->addRow(tr("Darkness stripe size:"), m_darknessStripe);
    m_objectListOffset = makeSpin(m_settings.objectListOffset, 256);
    form->addRow(tr("Object list offset:"), m_objectListOffset);
    m_stalkLength = makeSpin(m_settings.connectionStalkLength, 512);
    form->addRow(tr("Connection stalk length:"), m_stalkLength);
    m_preferredDistance = makeSpin(m_settings.preferredDistanceBetweenRooms, 2048);
    form->addRow(tr("Preferred distance between rooms:"), m_preferredDistance);
    return widget;
}

void SettingsDialog::setupFontButton(QPushButton *button, FontSpec *spec)
{
    auto refresh = [button, spec] {
        const double size = spec->size > 0.0 ? spec->size : 12.0;
        const QString family = spec->family.isEmpty() ? tr("(default)") : spec->family;
        button->setText(tr("%1, %2 pt").arg(family).arg(size));
        button->setFont(qfontFromSpec(*spec, 10.0));
    };
    refresh();
    connect(button, &QPushButton::clicked, this, [this, spec, refresh] {
        bool ok = false;
        const QFont chosen = QFontDialog::getFont(&ok, qfontFromSpec(*spec, 12.0), this,
                                                  tr("Choose Font"));
        if (!ok)
            return;
        spec->family = chosen.family();
        spec->size = chosen.pointSizeF() > 0 ? chosen.pointSizeF() : chosen.pixelSize();
        spec->bold = chosen.bold();
        spec->italic = chosen.italic();
        spec->underline = chosen.underline();
        spec->strikeout = chosen.strikeOut();
        refresh();
    });
}

QWidget *SettingsDialog::buildFontsTab()
{
    auto *widget = new QWidget;
    auto *form = new QFormLayout(widget);
    m_roomFontButton = new QPushButton(widget);
    setupFontButton(m_roomFontButton, &m_roomFont);
    form->addRow(tr("Room name font:"), m_roomFontButton);
    m_subtitleFontButton = new QPushButton(widget);
    setupFontButton(m_subtitleFontButton, &m_subtitleFont);
    form->addRow(tr("Subtitle font:"), m_subtitleFontButton);
    m_objectFontButton = new QPushButton(widget);
    setupFontButton(m_objectFontButton, &m_objectFont);
    form->addRow(tr("Object list font:"), m_objectFontButton);
    m_lineFontButton = new QPushButton(widget);
    setupFontButton(m_lineFontButton, &m_lineFont);
    form->addRow(tr("Connection text font:"), m_lineFontButton);
    return widget;
}

QWidget *SettingsDialog::buildAdvancedTab()
{
    auto *widget = new QWidget;
    auto *form = new QFormLayout(widget);

    m_handleSize = makeSpin(m_settings.handleSize, 64);
    form->addRow(tr("Handle size:"), m_handleSize);
    m_snapToElement = makeSpin(m_settings.snapToElementSize, 256);
    form->addRow(tr("Snap-to-element distance:"), m_snapToElement);

    m_docMargins = new QCheckBox(tr("Use document-specific margins"), widget);
    m_docMargins->setChecked(m_settings.documentSpecificMargins);
    form->addRow(QString(), m_docMargins);
    m_hMargin = makeSpin(m_settings.horizontalMargin, 4096);
    form->addRow(tr("Horizontal margin:"), m_hMargin);
    m_vMargin = makeSpin(m_settings.verticalMargin, 4096);
    form->addRow(tr("Vertical margin:"), m_vMargin);

    m_wrapDashes = new QCheckBox(tr("Wrap text at dashes"), widget);
    m_wrapDashes->setChecked(m_settings.wrapTextAtDashes);
    form->addRow(QString(), m_wrapDashes);
    return widget;
}

QWidget *SettingsDialog::buildRegionsTab()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);

    m_regionTable = new QTableWidget(0, 3, widget);
    m_regionTable->setHorizontalHeaderLabels({tr("Name"), tr("Text"), tr("Fill")});
    m_regionTable->horizontalHeader()->setStretchLastSection(false);
    m_regionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    auto addRow = [this](const Region &r, const QString &originalName) {
        const int row = m_regionTable->rowCount();
        m_regionTable->insertRow(row);
        auto *nameItem = new QTableWidgetItem(r.name);
        // Remember the name this row started with, so a rename can be detected
        // and propagated to the rooms that use it.
        nameItem->setData(Qt::UserRole, originalName);
        m_regionTable->setItem(row, 0, nameItem);
        for (int col = 1; col <= 2; ++col) {
            auto *item = new QTableWidgetItem;
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            const QColor c = (col == 1) ? r.textColor : r.fillColor;
            item->setBackground(c.isValid() ? c : QColor(Qt::white));
            item->setData(Qt::UserRole, c);
            m_regionTable->setItem(row, col, item);
        }
    };
    for (const Region &r : m_regions)
        addRow(r, r.name);

    connect(m_regionTable, &QTableWidget::cellDoubleClicked, this, [this](int row, int col) {
        if (col == 0)
            return;
        QTableWidgetItem *item = m_regionTable->item(row, col);
        const QColor chosen =
            QColorDialog::getColor(item->data(Qt::UserRole).value<QColor>(), this,
                                   tr("Choose colour"));
        if (chosen.isValid()) {
            item->setBackground(chosen);
            item->setData(Qt::UserRole, chosen);
        }
    });

    layout->addWidget(m_regionTable);

    auto *buttonRow = new QHBoxLayout;
    auto *addButton = new QPushButton(tr("Add"), widget);
    auto *removeButton = new QPushButton(tr("Remove"), widget);
    buttonRow->addWidget(addButton);
    buttonRow->addWidget(removeButton);
    buttonRow->addStretch(1);
    layout->addLayout(buttonRow);

    connect(addButton, &QPushButton::clicked, this, [this, addRow] {
        addRow(Region{tr("New Region"), QColor(0, 0, 255), QColor(255, 255, 255), QString(),
                      QString()},
               QString());
    });
    connect(removeButton, &QPushButton::clicked, this, [this] {
        const int row = m_regionTable->currentRow();
        if (row >= 0) {
            const QString name = m_regionTable->item(row, 0)->text();
            if (name != kNoRegion) // never remove the default region
                m_regionTable->removeRow(row);
        }
    });
    return widget;
}

MapSettings SettingsDialog::resultSettings() const
{
    MapSettings s = m_settings; // keep tokens/fonts/margins we didn't expose
    for (int i = 0; i < ColorCount; ++i) {
        if (m_colorButtons[i])
            s.colors[i] = buttonColor(m_colorButtons[i]);
    }
    s.gridSize = m_gridSize->value();
    s.gridVisible = m_gridVisible->isChecked();
    s.snapToGrid = m_snapToGrid->isChecked();
    s.showOrigin = m_showOrigin->isChecked();
    s.lineWidth = m_lineWidth->value();
    s.arrowSize = m_arrowSize->value();
    s.textOffset = m_textOffset->value();

    s.defaultRoomName = m_defaultRoomName->text().isEmpty() ? QStringLiteral("Cave")
                                                            : m_defaultRoomName->text();
    s.defaultRoomShape = m_defaultRoomShape->currentIndex();
    s.darknessStripeSize = m_darknessStripe->value();
    s.objectListOffset = m_objectListOffset->value();
    s.connectionStalkLength = m_stalkLength->value();
    s.preferredDistanceBetweenRooms = m_preferredDistance->value();

    s.roomFont = m_roomFont;
    s.subtitleFont = m_subtitleFont;
    s.objectFont = m_objectFont;
    s.lineFont = m_lineFont;

    if (m_handleSize)
        s.handleSize = m_handleSize->value();
    if (m_snapToElement)
        s.snapToElementSize = m_snapToElement->value();
    if (m_docMargins)
        s.documentSpecificMargins = m_docMargins->isChecked();
    if (m_hMargin)
        s.horizontalMargin = m_hMargin->value();
    if (m_vMargin)
        s.verticalMargin = m_vMargin->value();
    if (m_wrapDashes)
        s.wrapTextAtDashes = m_wrapDashes->isChecked();
    return s;
}

void SettingsDialog::accept()
{
    // Validate region names: non-empty, unique, and free of the '_' and ':'
    // characters the exporters reserve.
    QSet<QString> seen;
    for (int row = 0; row < m_regionTable->rowCount(); ++row) {
        const QString name = m_regionTable->item(row, 0)->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, tr("Invalid Region"), tr("Region names cannot be empty."));
            return;
        }
        if (name.contains(QLatin1Char('_')) || name.contains(QLatin1Char(':'))) {
            QMessageBox::warning(this, tr("Invalid Region"),
                                 tr("Region name '%1' may not contain '_' or ':'.").arg(name));
            return;
        }
        const QString key = name.toLower();
        if (seen.contains(key)) {
            QMessageBox::warning(this, tr("Invalid Region"),
                                 tr("Region name '%1' is used more than once.").arg(name));
            return;
        }
        seen.insert(key);
    }
    QDialog::accept();
}

QHash<QString, QString> SettingsDialog::regionRenames() const
{
    QHash<QString, QString> renames;
    for (int row = 0; row < m_regionTable->rowCount(); ++row) {
        const QString original = m_regionTable->item(row, 0)->data(Qt::UserRole).toString();
        const QString current = m_regionTable->item(row, 0)->text().trimmed();
        if (!original.isEmpty() && original != current)
            renames.insert(original, current);
    }
    return renames;
}

QStringList SettingsDialog::removedRegionNames() const
{
    QSet<QString> present;
    for (int row = 0; row < m_regionTable->rowCount(); ++row) {
        const QString original = m_regionTable->item(row, 0)->data(Qt::UserRole).toString();
        if (!original.isEmpty())
            present.insert(original);
    }
    QStringList removed;
    for (const Region &r : m_regions) {
        if (r.name != kNoRegion && !present.contains(r.name))
            removed << r.name;
    }
    return removed;
}

QList<Region> SettingsDialog::resultRegions() const
{
    QList<Region> regions;
    bool hasNoRegion = false;
    for (int row = 0; row < m_regionTable->rowCount(); ++row) {
        Region r;
        r.name = m_regionTable->item(row, 0)->text().trimmed();
        if (r.name.isEmpty())
            continue;
        if (r.name == kNoRegion)
            hasNoRegion = true;
        r.textColor = m_regionTable->item(row, 1)->data(Qt::UserRole).value<QColor>();
        r.fillColor = m_regionTable->item(row, 2)->data(Qt::UserRole).value<QColor>();
        // Preserve the exact serialized token for a region left unchanged, so
        // its colour is not rewritten from hex to a named colour and back.
        for (const Region &old : m_regions) {
            if (old.name == r.name) {
                if (old.textColor == r.textColor)
                    r.textColorToken = old.textColorToken;
                if (old.fillColor == r.fillColor)
                    r.fillColorToken = old.fillColorToken;
                break;
            }
        }
        regions.append(r);
    }
    if (!hasNoRegion)
        regions.prepend(Region{kNoRegion, QColor(0, 0, 255), QColor(255, 255, 255), QString(),
                               QString()});
    return regions;
}

} // namespace trizbort
