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

#ifndef TRIZBORT_COLORBUTTON_H
#define TRIZBORT_COLORBUTTON_H

#include <QColor>
#include <QColorDialog>
#include <QContextMenuEvent>
#include <QIcon>
#include <QMenu>
#include <QPixmap>
#include <QPushButton>

namespace trizbort {

// A push button that edits an optional colour. Left-click opens a colour
// picker; when clearing is allowed, right-click offers "clear" to fall back to
// the default (an invalid QColor). Header-only and free of new signals, so it
// needs no moc.
class ColorButton : public QPushButton {
public:
    explicit ColorButton(QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        connect(this, &QPushButton::clicked, this, [this] { choose(); });
        refresh();
    }

    void setColor(const QColor &c)
    {
        m_color = c;
        refresh();
    }
    QColor color() const { return m_color; }

    void setAllowClear(bool allow)
    {
        m_allowClear = allow;
        refresh();
    }
    void setTitle(const QString &title) { m_title = title; }

protected:
    void contextMenuEvent(QContextMenuEvent *event) override
    {
        if (!m_allowClear) {
            QPushButton::contextMenuEvent(event);
            return;
        }
        QMenu menu(this);
        QAction *setAct = menu.addAction(tr("Set Colour…"));
        QAction *clearAct = menu.addAction(tr("Clear (use default)"));
        clearAct->setEnabled(m_color.isValid());
        QAction *chosen = menu.exec(event->globalPos());
        if (chosen == setAct)
            choose();
        else if (chosen == clearAct)
            setColor(QColor());
    }

private:
    void choose()
    {
        const QColor start = m_color.isValid() ? m_color : QColor(Qt::white);
        const QColor c = QColorDialog::getColor(
            start, this, m_title.isEmpty() ? tr("Choose Colour") : m_title);
        if (c.isValid())
            setColor(c);
    }

    void refresh()
    {
        if (m_color.isValid()) {
            QPixmap pm(16, 16);
            pm.fill(m_color);
            setIcon(QIcon(pm));
            setText(m_color.name().toUpper());
        } else {
            setIcon(QIcon());
            setText(m_allowClear ? tr("None (default)") : tr("Default (click to set)"));
        }
        if (m_allowClear)
            setToolTip(tr("Left-click to set a colour; right-click to clear."));
    }

    QColor m_color;
    QString m_title;
    bool m_allowClear = true;
};

} // namespace trizbort

#endif // TRIZBORT_COLORBUTTON_H
