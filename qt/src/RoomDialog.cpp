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

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

namespace trizbort {

namespace {
const char *const kBorderStyles[] = {"Solid", "Dashed", "None", "Dot"};
}

RoomDialog::RoomDialog(const Map &map, const Room &room, QWidget *parent)
    : QDialog(parent)
    , m_room(room)
    , m_fill(room.fill)
    , m_borderColor(room.border)
{
    setWindowTitle(tr("Room Properties"));

    auto *form = new QFormLayout(this);

    m_name = new QLineEdit(room.name, this);
    form->addRow(tr("&Name:"), m_name);

    m_subtitle = new QLineEdit(room.subtitle, this);
    form->addRow(tr("&Subtitle:"), m_subtitle);

    m_region = new QComboBox(this);
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

    m_description = new QPlainTextEdit(room.description, this);
    m_description->setTabChangesFocus(true);
    m_description->setFixedHeight(64);
    form->addRow(tr("&Description:"), m_description);

    m_objects = new QPlainTextEdit(this);
    m_objects->setTabChangesFocus(true);
    m_objects->setFixedHeight(64);
    // The model stores objects one per line; show them with plain newlines.
    QString objects = room.objectsText;
    objects.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    m_objects->setPlainText(objects);
    form->addRow(tr("&Objects:"), m_objects);

    m_shape = new QComboBox(this);
    m_shape->addItems({tr("Square"), tr("Rounded"), tr("Ellipse"), tr("Octagonal")});
    int shapeIndex = 0;
    if (room.ellipse)
        shapeIndex = 2;
    else if (room.octagonal)
        shapeIndex = 3;
    else if (room.roundedCorners)
        shapeIndex = 1;
    m_shape->setCurrentIndex(shapeIndex);
    form->addRow(tr("S&hape:"), m_shape);

    m_border = new QComboBox(this);
    for (const char *style : kBorderStyles)
        m_border->addItem(QString::fromLatin1(style));
    {
        int idx = m_border->findText(room.borderStyle);
        m_border->setCurrentIndex(idx < 0 ? 0 : idx);
    }
    form->addRow(tr("&Border:"), m_border);

    m_dark = new QCheckBox(tr("Dark"), this);
    m_dark->setChecked(room.isDark);
    form->addRow(QString(), m_dark);

    m_start = new QCheckBox(tr("Start room"), this);
    m_start->setChecked(room.isStartRoom);
    form->addRow(QString(), m_start);

    m_end = new QCheckBox(tr("End room"), this);
    m_end->setChecked(room.isEndRoom);
    form->addRow(QString(), m_end);

    m_fillButton = new QPushButton(this);
    connect(m_fillButton, &QPushButton::clicked, this, &RoomDialog::pickFill);
    form->addRow(tr("&Fill colour:"), m_fillButton);

    m_borderColorButton = new QPushButton(this);
    connect(m_borderColorButton, &QPushButton::clicked, this, &RoomDialog::pickBorder);
    form->addRow(tr("Border co&lour:"), m_borderColorButton);

    updateColorButtons();

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    form->addRow(buttons);

    m_name->setFocus();
    m_name->selectAll();
}

void RoomDialog::updateColorButtons()
{
    m_fillButton->setText(m_fill.isValid() ? m_fill.name().toUpper() : tr("Default (click to set)"));
    m_borderColorButton->setText(m_borderColor.isValid() ? m_borderColor.name().toUpper()
                                                         : tr("Default (click to set)"));
}

void RoomDialog::pickFill()
{
    const QColor start = m_fill.isValid() ? m_fill : QColor(Qt::white);
    const QColor chosen = QColorDialog::getColor(start, this, tr("Room Fill Colour"));
    if (chosen.isValid()) {
        m_fill = chosen;
        updateColorButtons();
    }
}

void RoomDialog::pickBorder()
{
    const QColor start = m_borderColor.isValid() ? m_borderColor : QColor(Qt::black);
    const QColor chosen = QColorDialog::getColor(start, this, tr("Room Border Colour"));
    if (chosen.isValid()) {
        m_borderColor = chosen;
        updateColorButtons();
    }
}

Room RoomDialog::result() const
{
    Room r = m_room;
    r.name = m_name->text();
    r.subtitle = m_subtitle->text();
    r.region = m_region->currentText();
    r.description = m_description->toPlainText();

    QString objects = m_objects->toPlainText();
    objects.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    r.objectsText = objects;

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
    r.borderStyle = m_border->currentText();
    r.fill = m_fill;
    r.border = m_borderColor;
    return r;
}

} // namespace trizbort
