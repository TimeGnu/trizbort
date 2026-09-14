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

#ifndef TRIZBORT_ROOMDIALOG_H
#define TRIZBORT_ROOMDIALOG_H

#include <QDialog>

#include "MapDocument.h"

class QLineEdit;
class QPlainTextEdit;
class QComboBox;
class QCheckBox;
class QPushButton;

namespace trizbort {

// Modal editor for a room's properties. Constructed from a copy of the room;
// result() returns the edited room (id, seq and geometry preserved).
class RoomDialog : public QDialog {
    Q_OBJECT

public:
    RoomDialog(const Map &map, const Room &room, QWidget *parent = nullptr);

    Room result() const;

private:
    void pickFill();
    void pickBorder();
    void updateColorButtons();

    Room m_room;

    QLineEdit *m_name = nullptr;
    QLineEdit *m_subtitle = nullptr;
    QComboBox *m_region = nullptr;
    QPlainTextEdit *m_description = nullptr;
    QPlainTextEdit *m_objects = nullptr;
    QCheckBox *m_dark = nullptr;
    QCheckBox *m_start = nullptr;
    QCheckBox *m_end = nullptr;
    QComboBox *m_shape = nullptr;
    QComboBox *m_border = nullptr;
    QPushButton *m_fillButton = nullptr;
    QPushButton *m_borderColorButton = nullptr;

    QColor m_fill;    // invalid => default
    QColor m_borderColor;
};

} // namespace trizbort

#endif // TRIZBORT_ROOMDIALOG_H
