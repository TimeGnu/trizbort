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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright (c) 2010-2022 by Genstein and Jason Lautzenheiser
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */

#include "MainWindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>

#include "MapScene.h"
#include "MapView.h"
#include "TrizbortReader.h"

namespace trizbort {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new MapView(this);
    m_view->setScene(m_scene);
    setCentralWidget(m_view);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addSeparator();

    QAction *quitAction = fileMenu->addAction(tr("&Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    setWindowTitle(tr("Trizbort (Qt)"));
    statusBar()->showMessage(tr("Open a .trizbort map to begin."));
}

void MainWindow::openFile()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Trizbort Map"), QString(),
        tr("Trizbort maps (*.trizbort);;All files (*)"));
    if (!path.isEmpty())
        loadFile(path);
}

bool MainWindow::loadFile(const QString &path)
{
    QString error;
    Map map;
    if (!TrizbortReader::load(path, map, &error)) {
        QMessageBox::warning(this, tr("Trizbort (Qt)"),
                             tr("Could not load the map:\n%1").arg(error));
        return false;
    }

    m_map = map;
    m_scene->setMap(m_map);
    fitMap();

    setWindowTitle(tr("%1 - Trizbort (Qt)").arg(QFileInfo(path).fileName()));
    statusBar()->showMessage(tr("%1: %2 rooms, %3 connections")
                                 .arg(QFileInfo(path).fileName())
                                 .arg(m_map.rooms.size())
                                 .arg(m_map.connections.size()));
    return true;
}

void MainWindow::fitMap()
{
    const QRectF bounds = m_scene->itemsBoundingRect().adjusted(-40, -40, 40, 40);
    if (bounds.isEmpty())
        return;
    m_view->setSceneRect(bounds);
    m_view->fitInView(bounds, Qt::KeepAspectRatio);
}

} // namespace trizbort
