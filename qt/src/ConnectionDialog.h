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

#ifndef TRIZBORT_CONNECTIONDIALOG_H
#define TRIZBORT_CONNECTIONDIALOG_H

#include <QDialog>

#include "MapDocument.h"

class QLineEdit;
class QComboBox;
class QCheckBox;

namespace trizbort {

class ColorButton;

// Modal editor for a connection's properties. result() returns the edited
// connection (id, seq and vertices preserved).
class ConnectionDialog : public QDialog {
    Q_OBJECT

public:
    ConnectionDialog(const Connection &connection, QWidget *parent = nullptr);

    Connection result() const;

private:
    Connection m_conn;

    QComboBox *m_flow = nullptr;
    QComboBox *m_style = nullptr;
    QLineEdit *m_name = nullptr;
    QLineEdit *m_description = nullptr;
    QLineEdit *m_startText = nullptr;
    QLineEdit *m_midText = nullptr;
    QLineEdit *m_endText = nullptr;
    ColorButton *m_colorButton = nullptr;
    QCheckBox *m_door = nullptr;
    QCheckBox *m_lockable = nullptr;
    QCheckBox *m_openable = nullptr;
    QCheckBox *m_locked = nullptr;
    QCheckBox *m_open = nullptr;
};

} // namespace trizbort

#endif // TRIZBORT_CONNECTIONDIALOG_H
