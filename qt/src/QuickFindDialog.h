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

#ifndef TRIZBORT_QUICKFINDDIALOG_H
#define TRIZBORT_QUICKFINDDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>

#include "MapDocument.h"

namespace trizbort {

// Incremental find over the map: type to filter rooms by name, subtitle,
// description or objects; the chosen room's id is returned via selectedRoomId().
class QuickFindDialog : public QDialog {
public:
    explicit QuickFindDialog(const Map &map, QWidget *parent = nullptr)
        : QDialog(parent)
        , m_map(map)
    {
        setWindowTitle(tr("Find Room"));
        resize(360, 400);
        auto *layout = new QVBoxLayout(this);

        m_query = new QLineEdit(this);
        m_query->setPlaceholderText(tr("Search name, subtitle, description or objects…"));
        layout->addWidget(m_query);

        m_list = new QListWidget(this);
        layout->addWidget(m_list);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        layout->addWidget(buttons);

        connect(m_query, &QLineEdit::textChanged, this, [this] { refilter(); });
        connect(m_list, &QListWidget::itemActivated, this, [this] { accept(); });
        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

        refilter();
        m_query->setFocus();
    }

    int selectedRoomId() const
    {
        QListWidgetItem *item = m_list->currentItem();
        return item ? item->data(Qt::UserRole).toInt() : -1;
    }

private:
    void refilter()
    {
        const QString q = m_query->text().trimmed();
        m_list->clear();
        for (const Room &r : m_map.rooms) {
            if (!q.isEmpty()) {
                const bool match =
                    r.name.contains(q, Qt::CaseInsensitive) ||
                    r.subtitle.contains(q, Qt::CaseInsensitive) ||
                    r.description.contains(q, Qt::CaseInsensitive) ||
                    r.objectsText.contains(q, Qt::CaseInsensitive);
                if (!match)
                    continue;
            }
            const QString label = r.name.isEmpty() ? tr("(unnamed room %1)").arg(r.id) : r.name;
            auto *item = new QListWidgetItem(
                r.subtitle.isEmpty() ? label : QStringLiteral("%1 — %2").arg(label, r.subtitle),
                m_list);
            item->setData(Qt::UserRole, r.id);
        }
        if (m_list->count() > 0)
            m_list->setCurrentRow(0);
    }

    const Map &m_map;
    QLineEdit *m_query = nullptr;
    QListWidget *m_list = nullptr;
};

} // namespace trizbort

#endif // TRIZBORT_QUICKFINDDIALOG_H
