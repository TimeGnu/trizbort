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

#include "ConnectionDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "ColorButton.h"

namespace trizbort {

ConnectionDialog::ConnectionDialog(const Connection &connection, QWidget *parent)
    : QDialog(parent)
    , m_conn(connection)
{
    setWindowTitle(tr("Connection Properties"));

    auto *form = new QFormLayout(this);

    m_flow = new QComboBox(this);
    m_flow->addItems({tr("Two-way"), tr("One-way")});
    m_flow->setCurrentIndex(connection.flow == ConnectionFlow::OneWay ? 1 : 0);
    form->addRow(tr("&Flow:"), m_flow);

    m_style = new QComboBox(this);
    m_style->addItems({tr("Solid"), tr("Dashed")});
    m_style->setCurrentIndex(connection.style == ConnectionStyle::Dashed ? 1 : 0);
    form->addRow(tr("&Style:"), m_style);

    m_colorButton = new ColorButton(this);
    m_colorButton->setTitle(tr("Connection Colour"));
    m_colorButton->setColor(connection.color);
    form->addRow(tr("&Colour:"), m_colorButton);

    m_name = new QLineEdit(connection.name, this);
    form->addRow(tr("&Name:"), m_name);

    m_description = new QLineEdit(connection.description, this);
    form->addRow(tr("&Description:"), m_description);

    m_startText = new QLineEdit(connection.startText, this);
    form->addRow(tr("Start &text:"), m_startText);

    m_midText = new QLineEdit(connection.midText, this);
    form->addRow(tr("&Mid text:"), m_midText);

    m_endText = new QLineEdit(connection.endText, this);
    form->addRow(tr("&End text:"), m_endText);

    // Quick up/down/in/out labels: set the start/end text to a direction pair
    // (the C# connection labels), which is what those connections render as.
    auto *quick = new QWidget(this);
    auto *quickLayout = new QHBoxLayout(quick);
    quickLayout->setContentsMargins(0, 0, 0, 0);
    const struct {
        const char *label;
        const char *start;
        const char *end;
    } kQuick[] = {{"Up/Down", "up", "down"},
                  {"Down/Up", "down", "up"},
                  {"In/Out", "in", "out"},
                  {"Out/In", "out", "in"}};
    for (const auto &q : kQuick) {
        auto *btn = new QPushButton(tr(q.label), quick);
        const QString s = QString::fromLatin1(q.start);
        const QString e = QString::fromLatin1(q.end);
        connect(btn, &QPushButton::clicked, this, [this, s, e] {
            m_startText->setText(s);
            m_endText->setText(e);
        });
        quickLayout->addWidget(btn);
    }
    form->addRow(tr("Quick &label:"), quick);

    m_door = new QCheckBox(tr("Has door"), this);
    m_door->setChecked(connection.hasDoor);
    form->addRow(QString(), m_door);

    m_lockable = new QCheckBox(tr("Lockable"), this);
    m_lockable->setChecked(connection.door.lockable);
    form->addRow(QString(), m_lockable);

    m_openable = new QCheckBox(tr("Openable"), this);
    m_openable->setChecked(connection.door.openable);
    form->addRow(QString(), m_openable);

    m_locked = new QCheckBox(tr("Locked"), this);
    m_locked->setChecked(connection.door.locked);
    form->addRow(QString(), m_locked);

    m_open = new QCheckBox(tr("Open"), this);
    m_open->setChecked(connection.door.open);
    form->addRow(QString(), m_open);

    auto updateDoorEnabled = [this] {
        const bool on = m_door->isChecked();
        m_lockable->setEnabled(on);
        m_openable->setEnabled(on);
        m_locked->setEnabled(on);
        m_open->setEnabled(on);
    };
    connect(m_door, &QCheckBox::toggled, this, updateDoorEnabled);
    updateDoorEnabled();

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    form->addRow(buttons);
}

Connection ConnectionDialog::result() const
{
    Connection c = m_conn;
    c.flow = m_flow->currentIndex() == 1 ? ConnectionFlow::OneWay : ConnectionFlow::TwoWay;
    c.style = m_style->currentIndex() == 1 ? ConnectionStyle::Dashed : ConnectionStyle::Solid;
    c.name = m_name->text();
    c.description = m_description->text();
    c.color = m_colorButton->color();
    c.startText = m_startText->text();
    c.midText = m_midText->text();
    c.endText = m_endText->text();
    c.hasDoor = m_door->isChecked();
    c.door.lockable = m_lockable->isChecked();
    c.door.openable = m_openable->isChecked();
    c.door.locked = m_locked->isChecked();
    c.door.open = m_open->isChecked();
    return c;
}

} // namespace trizbort
