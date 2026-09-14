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

#include <QAction>
#include <QCloseEvent>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QKeySequence>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QToolBar>

#include "ConnectionDialog.h"
#include "EditCommands.h"
#include "MapScene.h"
#include "MapView.h"
#include "RoomDialog.h"
#include "SettingsDialog.h"
#include "TrizbortReader.h"
#include "TrizbortWriter.h"
#include "export/CodeExporter.h"
#include "export/ExporterFactory.h"

namespace trizbort {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new MapView(this);
    m_view->setScene(m_scene);
    setCentralWidget(m_view);

    m_scene->setUndoStack(&m_undo);
    connect(m_scene, &MapScene::editRoomRequested, this, &MainWindow::editRoom);
    connect(m_scene, &MapScene::editConnectionRequested, this, &MainWindow::editConnection);
    connect(m_scene, &MapScene::selectionSummary, this,
            [this](const QString &text) { statusBar()->showMessage(text); });
    connect(&m_undo, &QUndoStack::cleanChanged, this,
            [this](bool clean) { setWindowModified(!clean); });

    createActions();

    // Start with a fresh, empty document.
    newFile();
    statusBar()->showMessage(tr("Ready. Insert adds a room; C toggles connect mode."));
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *toolBar = addToolBar(tr("Main"));

    auto *newAct = fileMenu->addAction(tr("&New"), QKeySequence::New, this, &MainWindow::newFile);
    auto *openAct = fileMenu->addAction(tr("&Open…"), QKeySequence::Open, this, &MainWindow::openFile);
    auto *saveAct = fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, [this] { save(); });
    fileMenu->addAction(tr("Save &As…"), QKeySequence::SaveAs, this, [this] { saveAs(); });
    fileMenu->addSeparator();

    QMenu *exportMenu = fileMenu->addMenu(tr("&Export"));
    for (const ExportFormat &fmt : exportFormats()) {
        const QString key = fmt.key;
        exportMenu->addAction(fmt.label, this, [this, key] { exportMap(key); });
    }
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Quit"), QKeySequence::Quit, this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QAction *undoAct = m_undo.createUndoAction(this, tr("&Undo"));
    undoAct->setShortcut(QKeySequence::Undo);
    QAction *redoAct = m_undo.createRedoAction(this, tr("&Redo"));
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    auto *addRoomAct =
        editMenu->addAction(tr("Add &Room"), QKeySequence(Qt::Key_Insert), this, &MainWindow::addRoom);
    auto *deleteAct = editMenu->addAction(tr("&Delete Selection"), QKeySequence::Delete, this,
                                          &MainWindow::deleteSelection);
    m_connectAction = editMenu->addAction(tr("&Connect Mode"));
    m_connectAction->setCheckable(true);
    m_connectAction->setShortcut(QKeySequence(Qt::Key_C));
    connect(m_connectAction, &QAction::toggled, this, &MainWindow::toggleConnectMode);
    editMenu->addSeparator();
    editMenu->addAction(tr("&Map Properties…"), this, &MainWindow::editMapProperties);
    editMenu->addAction(tr("Map &Settings…"), this, &MainWindow::editMapSettings);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(tr("Zoom &In"), QKeySequence::ZoomIn, m_view, &MapView::zoomIn);
    viewMenu->addAction(tr("Zoom &Out"), QKeySequence::ZoomOut, m_view, &MapView::zoomOut);
    viewMenu->addAction(tr("&Reset Zoom"), QKeySequence(Qt::CTRL | Qt::Key_0), m_view,
                        &MapView::resetZoom);
    viewMenu->addAction(tr("&Fit to Window"), QKeySequence(Qt::CTRL | Qt::Key_F), m_view,
                        &MapView::zoomToFit);

    toolBar->addAction(newAct);
    toolBar->addAction(openAct);
    toolBar->addAction(saveAct);
    toolBar->addSeparator();
    toolBar->addAction(undoAct);
    toolBar->addAction(redoAct);
    toolBar->addSeparator();
    toolBar->addAction(addRoomAct);
    toolBar->addAction(m_connectAction);
    toolBar->addAction(deleteAct);
}

void MainWindow::newFile()
{
    if (!maybeSave())
        return;
    m_map.clear();
    // A fresh document has the default palette and a single NoRegion region.
    m_map.regions.append(Region{kNoRegion, QColor(0, 0, 255), QColor(255, 255, 255), QString(), QString()});
    m_filePath.clear();
    m_scene->setDocument(&m_map);
    m_undo.clear();
    updateTitle();
}

void MainWindow::openFile()
{
    if (!maybeSave())
        return;
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Map"), QString(), tr("Trizbort maps (*.trizbort);;All files (*)"));
    if (path.isEmpty())
        return;
    loadFile(path);
}

bool MainWindow::loadFile(const QString &path)
{
    Map loaded;
    QString error;
    if (!TrizbortReader::load(path, loaded, &error)) {
        QMessageBox::warning(this, tr("Open Failed"), error);
        return false;
    }
    m_map = loaded;
    m_map.reindex();
    m_filePath = path;
    m_scene->setDocument(&m_map);
    m_undo.clear();
    updateTitle();
    m_view->zoomToFit();
    statusBar()->showMessage(tr("Opened %1").arg(QFileInfo(path).fileName()));
    return true;
}

bool MainWindow::save()
{
    if (m_filePath.isEmpty())
        return saveAs();
    return writeToPath(m_filePath);
}

bool MainWindow::saveAs()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Map"), m_filePath,
                                                tr("Trizbort maps (*.trizbort)"));
    if (path.isEmpty())
        return false;
    if (!path.endsWith(QLatin1String(".trizbort"), Qt::CaseInsensitive))
        path += QLatin1String(".trizbort");
    if (!writeToPath(path))
        return false;
    m_filePath = path;
    updateTitle();
    return true;
}

bool MainWindow::writeToPath(const QString &path)
{
    QString error;
    if (!TrizbortWriter::save(path, m_map, &error)) {
        QMessageBox::warning(this, tr("Save Failed"), error);
        return false;
    }
    m_undo.setClean();
    statusBar()->showMessage(tr("Saved %1").arg(QFileInfo(path).fileName()));
    return true;
}

void MainWindow::exportMap(const QString &format)
{
    const ExportFormat *fmt = nullptr;
    for (const ExportFormat &f : exportFormats()) {
        if (f.key == format) {
            fmt = &f;
            break;
        }
    }
    if (!fmt)
        return;

    const QString suggested =
        m_filePath.isEmpty() ? fmt->key : QFileInfo(m_filePath).completeBaseName();
    const QString filter = tr("%1 (*.%2);;All files (*)").arg(fmt->label, fmt->extension);
    QString path = QFileDialog::getSaveFileName(
        this, tr("Export as %1").arg(fmt->label), suggested + QLatin1Char('.') + fmt->extension,
        filter);
    if (path.isEmpty())
        return;

    auto exporter = makeExporter(format, m_map, m_filePath.isEmpty() ? path : m_filePath);
    if (!exporter)
        return;
    const QString text = exporter->exportToString();

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, tr("Export Failed"), tr("Could not write %1").arg(path));
        return;
    }
    file.write(text.toUtf8());
    statusBar()->showMessage(tr("Exported %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::addRoom()
{
    const QPointF center = m_view->mapToScene(m_view->viewport()->rect().center());
    const int id = m_scene->addRoomAt(center);
    if (id >= 0)
        statusBar()->showMessage(tr("Added room %1 (double-click to edit)").arg(id));
}

void MainWindow::deleteSelection()
{
    m_scene->deleteSelection();
}

void MainWindow::toggleConnectMode(bool on)
{
    m_scene->setConnectMode(on);
    statusBar()->showMessage(on ? tr("Connect mode: drag between two rooms to connect them.")
                                : tr("Select mode."));
}

void MainWindow::editRoom(int roomId)
{
    const Room *room = m_map.roomById(roomId);
    if (!room)
        return;
    const Room before = *room;
    RoomDialog dialog(m_map, before, this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    m_undo.push(new EditRoomCommand(m_scene, roomId, before, dialog.result()));
}

void MainWindow::editConnection(int connId)
{
    const int idx = m_map.connectionIndex(connId);
    if (idx < 0)
        return;
    const Connection before = m_map.connections.at(idx);
    ConnectionDialog dialog(before, this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    m_undo.push(new EditConnectionCommand(m_scene, connId, before, dialog.result()));
}

void MainWindow::editMapProperties()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Map Properties"));
    auto *form = new QFormLayout(&dialog);
    auto *title = new QLineEdit(m_map.title, &dialog);
    auto *author = new QLineEdit(m_map.author, &dialog);
    auto *description = new QPlainTextEdit(m_map.description, &dialog);
    description->setFixedHeight(64);
    auto *history = new QPlainTextEdit(m_map.history, &dialog);
    history->setFixedHeight(64);
    form->addRow(tr("&Title:"), title);
    form->addRow(tr("&Author:"), author);
    form->addRow(tr("&Description:"), description);
    form->addRow(tr("&History:"), history);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form->addRow(buttons);

    if (dialog.exec() != QDialog::Accepted)
        return;
    EditMapInfoCommand::Info before{m_map.title, m_map.author, m_map.description, m_map.history};
    EditMapInfoCommand::Info after{title->text(), author->text(), description->toPlainText(),
                                   history->toPlainText()};
    if (before.title == after.title && before.author == after.author
        && before.description == after.description && before.history == after.history)
        return;
    m_undo.push(new EditMapInfoCommand(m_scene, before, after));
}

void MainWindow::editMapSettings()
{
    SettingsDialog dialog(m_map.settings, m_map.regions, this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    m_undo.push(new EditSettingsCommand(m_scene, m_map.settings, m_map.regions,
                                        dialog.resultSettings(), dialog.resultRegions()));
}

void MainWindow::updateTitle()
{
    const QString name = m_filePath.isEmpty() ? tr("Untitled") : QFileInfo(m_filePath).fileName();
    setWindowTitle(tr("%1[*] — Trizbort (Qt)").arg(name));
    setWindowModified(!m_undo.isClean());
}

bool MainWindow::maybeSave()
{
    if (m_undo.isClean())
        return true;
    const auto choice = QMessageBox::warning(
        this, tr("Unsaved Changes"),
        tr("The map has unsaved changes. Save them?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (choice == QMessageBox::Save)
        return save();
    return choice == QMessageBox::Discard;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

} // namespace trizbort
