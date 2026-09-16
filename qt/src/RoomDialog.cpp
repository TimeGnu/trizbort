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

#include "RoomDialog.h"

#include <algorithm>

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

#include "ColorButton.h"

namespace trizbort {

namespace {
// Border tokens exactly as stored in the .trizbort file (BorderDashStyle names).
const char *const kBorderStyles[] = {"Solid", "Dash", "DashDot", "DashDotDot", "Dot", "None"};
// Second-fill layouts, matching the C# SecondFillLocation values.
const char *const kSecondFillLocations[] = {"Bottom", "BottomRight", "BottomLeft", "Left",
                                            "Right",  "TopRight",    "TopLeft",    "Top"};
} // namespace

RoomDialog::RoomDialog(const Map &map, const Room &room, QWidget *parent)
    : QDialog(parent)
    , m_room(room)
{
    setWindowTitle(tr("Room Properties"));

    auto *tabs = new QTabWidget(this);

    // --- General tab ---
    auto *general = new QWidget(tabs);
    auto *form = new QFormLayout(general);

    m_name = new QLineEdit(room.name, general);
    form->addRow(tr("&Name:"), m_name);

    m_subtitle = new QLineEdit(room.subtitle, general);
    form->addRow(tr("&Subtitle:"), m_subtitle);

    m_region = new QComboBox(general);
    m_region->setEditable(true);
    m_region->addItem(kNoRegion);
    for (const Region &r : map.regions) {
        if (r.name != kNoRegion)
            m_region->addItem(r.name);
    }
    {
        const QString cur = room.region.isEmpty() ? kNoRegion : room.region;
        int idx = m_region->findText(cur);
        if (idx < 0) {
            m_region->addItem(cur);
            idx = m_region->findText(cur);
        }
        m_region->setCurrentIndex(idx);
    }
    form->addRow(tr("&Region:"), m_region);

    m_reference = new QComboBox(general);
    m_reference->addItem(tr("(none)"));
    for (const Room &r : map.rooms) {
        if (r.id == room.id)
            continue;
        m_reference->addItem(r.name.isEmpty() ? tr("Room %1").arg(r.id) : r.name);
        m_roomIds.append(r.id);
        if (r.id == room.referenceRoom)
            m_reference->setCurrentIndex(m_reference->count() - 1);
    }
    form->addRow(tr("Re&ferences room:"), m_reference);

    m_description = new QPlainTextEdit(room.description, general);
    m_description->setTabChangesFocus(true);
    m_description->setFixedHeight(64);
    form->addRow(tr("&Description:"), m_description);

    m_objects = new QPlainTextEdit(general);
    m_objects->setTabChangesFocus(true);
    m_objects->setFixedHeight(64);
    QString objects = room.objectsText;
    objects.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    m_objects->setPlainText(objects);
    form->addRow(tr("&Objects:"), m_objects);

    m_objectsPosition = new QComboBox(general);
    const struct {
        const char *label;
        const char *token;
    } kPositions[] = {{"South", "s"},     {"South-East", "se"}, {"East", "e"},
                      {"North-East", "ne"}, {"North", "n"},     {"North-West", "nw"},
                      {"West", "w"},        {"South-West", "sw"}};
    for (const auto &p : kPositions)
        m_objectsPosition->addItem(tr(p.label), QString::fromLatin1(p.token));
    {
        const QString cur = room.objectsPosition.at.isEmpty() ? QStringLiteral("s")
                                                              : room.objectsPosition.at.toLower();
        int idx = m_objectsPosition->findData(cur);
        if (idx < 0) {
            // Preserve an in-between position token the combo doesn't list.
            m_objectsPosition->addItem(cur.toUpper(), cur);
            idx = m_objectsPosition->count() - 1;
        }
        m_objectsPosition->setCurrentIndex(idx);
    }
    form->addRow(tr("Objects &position:"), m_objectsPosition);

    m_dark = new QCheckBox(tr("Dark"), general);
    m_dark->setChecked(room.isDark);
    form->addRow(QString(), m_dark);
    m_start = new QCheckBox(tr("Start room"), general);
    m_start->setChecked(room.isStartRoom);
    form->addRow(QString(), m_start);
    m_end = new QCheckBox(tr("End room"), general);
    m_end->setChecked(room.isEndRoom);
    form->addRow(QString(), m_end);

    tabs->addTab(general, tr("&General"));

    // --- Appearance tab ---
    auto *appearance = new QWidget(tabs);
    auto *aform = new QFormLayout(appearance);

    m_shape = new QComboBox(appearance);
    m_shape->addItems({tr("Square"), tr("Rounded"), tr("Ellipse"), tr("Octagonal")});
    int shapeIndex = 0;
    if (room.ellipse)
        shapeIndex = 2;
    else if (room.octagonal)
        shapeIndex = 3;
    else if (room.roundedCorners)
        shapeIndex = 1;
    m_shape->setCurrentIndex(shapeIndex);
    aform->addRow(tr("S&hape:"), m_shape);

    m_cornerRadius = new QSpinBox(appearance);
    m_cornerRadius->setRange(0, 30);
    m_cornerRadius->setValue(qRound(std::max({room.cornerTopLeft, room.cornerTopRight,
                                              room.cornerBottomLeft, room.cornerBottomRight})));
    aform->addRow(tr("Corner &radius:"), m_cornerRadius);

    m_border = new QComboBox(appearance);
    for (const char *style : kBorderStyles)
        m_border->addItem(QString::fromLatin1(style));
    {
        int idx = m_border->findText(room.borderStyle);
        m_border->setCurrentIndex(idx < 0 ? 0 : idx);
    }
    aform->addRow(tr("&Border style:"), m_border);

    m_fillButton = new ColorButton(appearance);
    m_fillButton->setTitle(tr("Room Fill Colour"));
    m_fillButton->setColor(room.fill);
    aform->addRow(tr("&Fill colour:"), m_fillButton);

    m_secondFillButton = new ColorButton(appearance);
    m_secondFillButton->setTitle(tr("Second Fill Colour"));
    m_secondFillButton->setColor(room.secondFill);
    aform->addRow(tr("Second &fill:"), m_secondFillButton);

    m_secondFillLocation = new QComboBox(appearance);
    for (const char *loc : kSecondFillLocations)
        m_secondFillLocation->addItem(QString::fromLatin1(loc));
    {
        int idx = m_secondFillLocation->findText(room.secondFillLocation);
        m_secondFillLocation->setCurrentIndex(idx < 0 ? 0 : idx);
    }
    aform->addRow(tr("Second fill &location:"), m_secondFillLocation);

    m_borderColorButton = new ColorButton(appearance);
    m_borderColorButton->setTitle(tr("Border Colour"));
    m_borderColorButton->setColor(room.border);
    aform->addRow(tr("Border co&lour:"), m_borderColorButton);

    m_nameColorButton = new ColorButton(appearance);
    m_nameColorButton->setTitle(tr("Name Text Colour"));
    m_nameColorButton->setColor(room.largeText);
    aform->addRow(tr("Nam&e colour:"), m_nameColorButton);

    m_subtitleColorButton = new ColorButton(appearance);
    m_subtitleColorButton->setTitle(tr("Subtitle Colour"));
    m_subtitleColorButton->setColor(room.subtitleColor);
    aform->addRow(tr("S&ubtitle colour:"), m_subtitleColorButton);

    m_objectColorButton = new ColorButton(appearance);
    m_objectColorButton->setTitle(tr("Object Text Colour"));
    m_objectColorButton->setColor(room.smallText);
    aform->addRow(tr("Ob&ject colour:"), m_objectColorButton);

    tabs->addTab(appearance, tr("&Appearance"));

    auto *outer = new QVBoxLayout(this);
    outer->addWidget(tabs);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    outer->addWidget(buttons);

    m_name->setFocus();
    m_name->selectAll();
}

Room RoomDialog::result() const
{
    Room r = m_room;
    r.name = m_name->text();
    r.subtitle = m_subtitle->text();
    r.region = m_region->currentText();
    r.description = m_description->toPlainText();

    const int refIdx = m_reference->currentIndex();
    r.referenceRoom = (refIdx > 0 && refIdx - 1 < m_roomIds.size()) ? m_roomIds.at(refIdx - 1) : -1;

    QString objects = m_objects->toPlainText();
    objects.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    r.objectsText = objects;
    r.objectsPosition.at = m_objectsPosition->currentData().toString();

    r.isDark = m_dark->isChecked();
    r.isStartRoom = m_start->isChecked();
    r.isEndRoom = m_end->isChecked();

    r.ellipse = r.octagonal = r.roundedCorners = false;
    switch (m_shape->currentIndex()) {
    case 1: r.roundedCorners = true; break;
    case 2: r.ellipse = true; break;
    case 3: r.octagonal = true; break;
    default: break;
    }

    const double radius = m_cornerRadius->value();
    r.allCornersEqual = true;
    r.cornerTopLeft = r.cornerTopRight = r.cornerBottomLeft = r.cornerBottomRight = radius;

    r.borderStyle = m_border->currentText();
    r.fill = m_fillButton->color();
    r.border = m_borderColorButton->color();
    r.secondFill = m_secondFillButton->color();
    r.secondFillLocation = m_secondFillLocation->currentText();
    r.largeText = m_nameColorButton->color();
    r.subtitleColor = m_subtitleColorButton->color();
    r.smallText = m_objectColorButton->color();
    return r;
}

} // namespace trizbort
