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

#ifndef TRIZBORT_MAINWINDOW_H
#define TRIZBORT_MAINWINDOW_H

#include <QMainWindow>

#include "MapDocument.h"

namespace trizbort {

class MapScene;
class MapView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    bool loadFile(const QString &path);

private:
    void openFile();
    void fitMap();

    MapScene *m_scene = nullptr;
    MapView *m_view = nullptr;
    Map m_map;
};

} // namespace trizbort

#endif // TRIZBORT_MAINWINDOW_H
