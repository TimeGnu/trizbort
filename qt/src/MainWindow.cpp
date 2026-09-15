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

#include <algorithm>

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QCloseEvent>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QFormLayout>
#include <QInputDialog>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QHash>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QSet>
#include <QSettings>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QToolBar>

#include "ConnectionDialog.h"
#include "EditCommands.h"
#include "MapRender.h"
#include "MapScene.h"
#include "MapStatisticsDialog.h"
#include "MapView.h"
#include "MinimapView.h"
#include "QuickFindDialog.h"
#include "RoomDialog.h"
#include "SettingsDialog.h"
#include "TranscriptAutomapper.h"
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

    // Minimap overview in a dockable panel (hidden until the user shows it).
    m_minimapDock = new QDockWidget(tr("Mini Map"), this);
    m_minimapDock->setWidget(new MinimapView(m_scene, m_view, m_minimapDock));
    m_minimapDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_minimapDock);
    m_minimapDock->hide();

    m_scene->setUndoStack(&m_undo);
    connect(m_scene, &MapScene::editRoomRequested, this, &MainWindow::editRoom);
    connect(m_scene, &MapScene::editConnectionRequested, this, &MainWindow::editConnection);
    connect(m_scene, &MapScene::selectionSummary, this,
            [this](const QString &text) { statusBar()->showMessage(text); });
    connect(&m_undo, &QUndoStack::cleanChanged, this,
            [this](bool clean) { setWindowModified(!clean); });

    createActions();

    // A permanent zoom-percentage indicator in the status bar, kept current as
    // the view is zoomed (a scale change alters the scrollbar range).
    m_zoomLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_zoomLabel);
    auto updateZoom = [this] {
        m_zoomLabel->setText(tr("%1%").arg(qRound(m_view->transform().m11() * 100.0)));
    };
    connect(m_view->horizontalScrollBar(), &QScrollBar::rangeChanged, this, updateZoom);
    connect(m_view->verticalScrollBar(), &QScrollBar::rangeChanged, this, updateZoom);
    updateZoom();

    // Restore preferences (window geometry, recent files, toggles).
    loadPreferences();
    m_view->setInvertWheelZoom(m_invertWheel);
    rebuildRecentMenu();

    // Start with a fresh, empty document, then optionally reopen the last map.
    newFile();
    if (m_loadLastOnStart && !m_recentFiles.isEmpty() && QFile::exists(m_recentFiles.first()))
        loadFile(m_recentFiles.first());
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
    m_recentMenu = fileMenu->addMenu(tr("Recent &Maps"));
    fileMenu->addSeparator();

    fileMenu->addAction(tr("Bac&kup"), QKeySequence(Qt::CTRL | Qt::Key_B), this,
                        &MainWindow::backupMap);
    fileMenu->addAction(tr("S&mart Save"), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S), this,
                        &MainWindow::smartSave);
    fileMenu->addSeparator();

    QMenu *exportMenu = fileMenu->addMenu(tr("&Export"));
    for (const ExportFormat &fmt : exportFormats()) {
        const QString key = fmt.key;
        exportMenu->addAction(fmt.label, this, [this, key] { exportMap(key); });
    }
    exportMenu->addSeparator();
    exportMenu->addAction(tr("PDF…"), this, &MainWindow::exportPdf);
    exportMenu->addAction(tr("Image (PNG/JPEG/BMP)…"), this, &MainWindow::exportImage);

    QMenu *clipboardMenu = fileMenu->addMenu(tr("Export to &Clipboard"));
    for (const ExportFormat &fmt : exportFormats()) {
        const QString key = fmt.key;
        clipboardMenu->addAction(fmt.label, this, [this, key] { exportToClipboard(key); });
    }

    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Import Transcript…"), this, &MainWindow::importTranscript);
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
    editMenu->addAction(tr("Cu&t"), QKeySequence::Cut, this, [this] {
        copySelection();
        deleteSelection();
    });
    editMenu->addAction(tr("&Copy"), QKeySequence::Copy, this, &MainWindow::copySelection);
    editMenu->addAction(tr("&Paste"), QKeySequence::Paste, this, &MainWindow::paste);
    editMenu->addAction(tr("Copy Colo&ur"), QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C), this,
                        &MainWindow::copyColor);
    editMenu->addAction(tr("Paste Colou&r"), QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_V), this,
                        &MainWindow::pasteColor);
    editMenu->addSeparator();
    editMenu->addAction(tr("Select &All"), QKeySequence::SelectAll, this,
                        [this] { m_scene->selectAll(); });
    editMenu->addAction(tr("Select &None"), QKeySequence(Qt::Key_Escape), this,
                        [this] { m_scene->clearSelection(); });
    editMenu->addAction(tr("&Find Room…"), QKeySequence::Find, this, [this] {
        QuickFindDialog dlg(m_map, this);
        if (dlg.exec() != QDialog::Accepted)
            return;
        const int id = dlg.selectedRoomId();
        if (id < 0)
            return;
        m_scene->selectRoomItem(id);
        if (const Room *r = m_map.roomById(id))
            m_view->centerOn(r->x + r->w / 2.0, r->y + r->h / 2.0);
    });
    QMenu *selectMenu = editMenu->addMenu(tr("Se&lect"));
    selectMenu->addAction(tr("Unconnected Rooms"), this, [this] { selectSpecial(0); });
    selectMenu->addAction(tr("Rooms With Objects"), this, [this] { selectSpecial(1); });
    selectMenu->addAction(tr("Rooms Without Objects"), this, [this] { selectSpecial(2); });
    selectMenu->addSeparator();
    selectMenu->addAction(tr("All Connections"), this, [this] { selectSpecial(3); });
    selectMenu->addAction(tr("Dangling Connections"), this, [this] { selectSpecial(4); });
    selectMenu->addAction(tr("Self-Looping Connections"), this, [this] { selectSpecial(5); });

    // Automap: create a connected room in a compass direction from the selection.
    QMenu *automapMenu = editMenu->addMenu(tr("Add Connec&ted Room"));
    struct Dir {
        const char *label;
        const char *dir;
        QKeySequence shortcut;
    };
    const Dir dirs[] = {
        {"North", "n", QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Up)},
        {"South", "s", QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Down)},
        {"East", "e", QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Right)},
        {"West", "w", QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_Left)},
        {"North-East", "ne", QKeySequence()},
        {"North-West", "nw", QKeySequence()},
        {"South-East", "se", QKeySequence()},
        {"South-West", "sw", QKeySequence()},
    };
    for (const Dir &d : dirs) {
        const QString dir = QString::fromLatin1(d.dir);
        QAction *act = automapMenu->addAction(tr(d.label), this, [this, dir] { addConnectedRoom(dir); });
        if (!d.shortcut.isEmpty())
            act->setShortcut(d.shortcut);
    }
    editMenu->addSeparator();
    editMenu->addAction(tr("&Map Properties…"), this, &MainWindow::editMapProperties);
    editMenu->addAction(tr("Map &Settings…"), this, &MainWindow::editMapSettings);

    // --- Rooms menu ---
    QMenu *roomsMenu = menuBar()->addMenu(tr("&Rooms"));
    roomsMenu->addAction(tr("Re&name…"), QKeySequence(Qt::Key_F2), this,
                         &MainWindow::renameSelectedRoom);
    roomsMenu->addAction(tr("Change &Region…"), QKeySequence(Qt::SHIFT | Qt::Key_F2), this,
                         &MainWindow::changeSelectedRegion);
    roomsMenu->addSeparator();
    roomsMenu->addAction(tr("Toggle &Darkness"), QKeySequence(Qt::Key_K), this, [this] {
        applyToSelectedRooms(tr("Toggle Darkness"), [](Room &r) { r.isDark = !r.isDark; });
    });
    roomsMenu->addAction(tr("Force Dar&k"), QKeySequence(Qt::CTRL | Qt::Key_K), this, [this] {
        applyToSelectedRooms(tr("Force Dark"), [](Room &r) { r.isDark = true; });
    });
    roomsMenu->addAction(tr("Force &Lighted"), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_K), this,
                         [this] {
                             applyToSelectedRooms(tr("Force Lighted"),
                                                  [](Room &r) { r.isDark = false; });
                         });
    roomsMenu->addSeparator();
    roomsMenu->addAction(tr("Set as &Start Room"), QKeySequence(Qt::CTRL | Qt::Key_F5), this,
                         [this] { setStartOrEndRoom(true); });
    roomsMenu->addAction(tr("Set as &End Room"), QKeySequence(Qt::SHIFT | Qt::Key_F5), this,
                         [this] { setStartOrEndRoom(false); });
    roomsMenu->addSeparator();
    QMenu *shapeMenu = roomsMenu->addMenu(tr("S&hape"));
    shapeMenu->addAction(tr("S&quare"), QKeySequence(Qt::CTRL | Qt::Key_H), this,
                         [this] { setSelectedRoomShape(0); });
    shapeMenu->addAction(tr("&Rounded"), QKeySequence(Qt::CTRL | Qt::Key_R), this,
                         [this] { setSelectedRoomShape(1); });
    shapeMenu->addAction(tr("&Ellipse"), QKeySequence(Qt::CTRL | Qt::Key_E), this,
                         [this] { setSelectedRoomShape(2); });
    shapeMenu->addAction(tr("&Octagonal"), QKeySequence(Qt::CTRL | Qt::Key_8), this,
                         [this] { setSelectedRoomShape(3); });
    roomsMenu->addSeparator();
    roomsMenu->addAction(tr("&Join Rooms"), QKeySequence(Qt::Key_J), this,
                         &MainWindow::joinSelectedRooms);
    QMenu *swapMenu = roomsMenu->addMenu(tr("S&wap (two rooms)"));
    swapMenu->addAction(tr("Swap &Objects"), QKeySequence(Qt::Key_W), this,
                        [this] { swapSelectedRooms(0); });
    swapMenu->addAction(tr("Swap &Names"), QKeySequence(Qt::CTRL | Qt::Key_W), this,
                        [this] { swapSelectedRooms(1); });
    swapMenu->addAction(tr("Swap &Formats"), QKeySequence(Qt::SHIFT | Qt::Key_W), this,
                        [this] { swapSelectedRooms(2); });
    swapMenu->addAction(tr("Swap &Regions"), QKeySequence(Qt::ALT | Qt::Key_W), this,
                        [this] { swapSelectedRooms(3); });

    // --- Validation menu ---
    QMenu *validationMenu = menuBar()->addMenu(tr("&Validation"));
    auto *vUnique = validationMenu->addAction(tr("Rooms Must Have a &Unique Name"));
    auto *vDesc = validationMenu->addAction(tr("Rooms Must Have a &Description"));
    auto *vSub = validationMenu->addAction(tr("Rooms Must Have a &Subtitle"));
    auto *vDang = validationMenu->addAction(tr("Rooms Must Not Have a Dan&gling Connection"));
    for (QAction *a : {vUnique, vDesc, vSub, vDang})
        a->setCheckable(true);
    auto applyValidation = [this, vUnique, vDesc, vSub, vDang] {
        MapScene::ValidationFlags f;
        f.uniqueNames = vUnique->isChecked();
        f.description = vDesc->isChecked();
        f.subtitle = vSub->isChecked();
        f.noDangling = vDang->isChecked();
        m_scene->setValidation(f);
    };
    for (QAction *a : {vUnique, vDesc, vSub, vDang})
        connect(a, &QAction::triggered, this, applyValidation);

    // --- Tools menu ---
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(tr("Map St&atistics…"), this, [this] {
        MapStatisticsDialog dlg(m_map, this);
        dlg.exec();
    });
    toolsMenu->addSeparator();
    toolsMenu->addAction(tr("&Application Settings…"), this, &MainWindow::showAppSettings);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(tr("Zoom &In"), QKeySequence::ZoomIn, m_view, &MapView::zoomIn);
    viewMenu->addAction(tr("Zoom &Out"), QKeySequence::ZoomOut, m_view, &MapView::zoomOut);
    viewMenu->addAction(tr("Zoom In &1%"), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Equal), this,
                        [this] { m_view->microZoom(true); });
    viewMenu->addAction(tr("Zoom Out 1&%"), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Minus), this,
                        [this] { m_view->microZoom(false); });
    QMenu *zoomMenu = viewMenu->addMenu(tr("&Zoom To"));
    zoomMenu->addAction(tr("50%"), this, [this] { m_view->setZoomPercent(50); });
    zoomMenu->addAction(tr("100%"), this, [this] { m_view->setZoomPercent(100); });
    zoomMenu->addAction(tr("200%"), this, [this] { m_view->setZoomPercent(200); });
    viewMenu->addAction(tr("&Reset Zoom"), QKeySequence(Qt::CTRL | Qt::Key_0), m_view,
                        &MapView::resetZoom);
    viewMenu->addAction(tr("Reset &Origin"), QKeySequence(Qt::Key_Home), m_view,
                        &MapView::resetOrigin);
    viewMenu->addAction(tr("&Fit to Window"), QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F), m_view,
                        &MapView::zoomToFit);
    viewMenu->addSeparator();

    m_gridAction = viewMenu->addAction(tr("Show &Grid"));
    QAction *gridAct = m_gridAction;
    gridAct->setCheckable(true);
    gridAct->setChecked(m_map.settings.gridVisible);
    connect(gridAct, &QAction::triggered, this, [this](bool on) {
        MapSettings ns = m_map.settings;
        ns.gridVisible = on;
        m_undo.push(new EditSettingsCommand(m_scene, m_map.settings, m_map.regions, ns,
                                            m_map.regions));
    });
    m_snapAction = viewMenu->addAction(tr("&Snap to Grid"));
    QAction *snapAct = m_snapAction;
    snapAct->setCheckable(true);
    snapAct->setChecked(m_map.settings.snapToGrid);
    connect(snapAct, &QAction::triggered, this, [this](bool on) {
        MapSettings ns = m_map.settings;
        ns.snapToGrid = on;
        m_undo.push(new EditSettingsCommand(m_scene, m_map.settings, m_map.regions, ns,
                                            m_map.regions));
    });
    viewMenu->addSeparator();
    if (m_minimapDock) {
        QAction *mm = m_minimapDock->toggleViewAction();
        mm->setText(tr("Mini &Map"));
        viewMenu->addAction(mm);
    }

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
    setWatchedFile(path);
    addRecentFile(path);
    statusBar()->showMessage(tr("Opened %1").arg(QFileInfo(path).fileName()));

    // Warn if the map was written by a newer format than this build produces.
    auto parts = [](const QString &v) {
        QList<int> p;
        for (const QString &s : v.split(QLatin1Char('.')))
            p << s.toInt();
        while (p.size() < 4)
            p << 0;
        return p;
    };
    if (!m_map.version.isEmpty()) {
        const QList<int> loadedV = parts(m_map.version);
        const QList<int> curV = parts(QStringLiteral("1.8.0.0"));
        for (int i = 0; i < 4; ++i) {
            if (loadedV[i] > curV[i]) {
                QMessageBox::information(
                    this, tr("Newer File Format"),
                    tr("This map was created by a newer version of Trizbort. Some information "
                       "may be lost if you edit and save it."));
                break;
            }
            if (loadedV[i] < curV[i])
                break;
        }
    }
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
    m_lastSaveMs = QDateTime::currentMSecsSinceEpoch();
    setWatchedFile(path);
    addRecentFile(path);
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

void MainWindow::exportImage()
{
    const QString suggested =
        (m_filePath.isEmpty() ? QStringLiteral("map") : QFileInfo(m_filePath).completeBaseName())
        + QStringLiteral(".png");
    QString path = QFileDialog::getSaveFileName(
        this, tr("Export Image"), suggested,
        tr("PNG image (*.png);;JPEG image (*.jpg *.jpeg);;Bitmap (*.bmp);;All files (*)"));
    if (path.isEmpty())
        return;
    // Default to PNG if the user gave no recognised image extension.
    const QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix != QLatin1String("png") && suffix != QLatin1String("jpg") &&
        suffix != QLatin1String("jpeg") && suffix != QLatin1String("bmp"))
        path += QStringLiteral(".png");
    QString error;
    if (!renderMapToImage(m_map, path, &error))
        QMessageBox::warning(this, tr("Export Failed"), error);
    else
        statusBar()->showMessage(tr("Exported %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::exportPdf()
{
    const QString suggested =
        (m_filePath.isEmpty() ? QStringLiteral("map") : QFileInfo(m_filePath).completeBaseName())
        + QStringLiteral(".pdf");
    QString path = QFileDialog::getSaveFileName(this, tr("Export PDF"), suggested,
                                                tr("PDF document (*.pdf);;All files (*)"));
    if (path.isEmpty())
        return;
    QString error;
    if (!renderMapToPdf(m_map, path, &error))
        QMessageBox::warning(this, tr("Export Failed"), error);
    else
        statusBar()->showMessage(tr("Exported %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::importTranscript()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Import Transcript"), QString(),
        tr("Transcripts (*.txt *.log *.scr);;All files (*)"));
    if (path.isEmpty())
        return;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Import Failed"), tr("Could not read %1").arg(path));
        return;
    }
    const QString text = QString::fromUtf8(file.readAll());

    ReplaceContentCommand::Content before{m_map.rooms, m_map.connections, m_map.regions};

    Map working = m_map;
    AutomapSettings settings;
    settings.preferredDistanceBetweenRooms = m_map.settings.preferredDistanceBetweenRooms;
    settings.gridSize = m_map.settings.gridSize;
    TranscriptAutomapper mapper(settings);
    const int added = mapper.run(working, text);
    if (added == 0) {
        QMessageBox::information(this, tr("Import Transcript"),
                                tr("No rooms were found in the transcript."));
        return;
    }
    ReplaceContentCommand::Content after{working.rooms, working.connections, working.regions};
    m_undo.push(new ReplaceContentCommand(m_scene, before, after, tr("Import Transcript")));
    m_view->zoomToFit();
    statusBar()->showMessage(tr("Imported %1: added %2 rooms, %3 connections")
                                 .arg(QFileInfo(path).fileName())
                                 .arg(added)
                                 .arg(mapper.connectionsAdded()));
}

void MainWindow::addRoom()
{
    const QPointF center = m_view->mapToScene(m_view->viewport()->rect().center());
    const int id = m_scene->addRoomAt(center);
    if (id >= 0)
        statusBar()->showMessage(tr("Added room %1 (double-click to edit)").arg(id));
}

void MainWindow::addConnectedRoom(const QString &direction)
{
    const int fromId = m_scene->selectedRoomId();
    if (fromId < 0) {
        statusBar()->showMessage(tr("Select a room first, then add a connected room."));
        return;
    }
    auto *cmd = new AddConnectedRoomCommand(m_scene, fromId, direction);
    if (!cmd->valid()) {
        delete cmd;
        return;
    }
    const int newId = cmd->newRoomId();
    m_undo.push(cmd);
    statusBar()->showMessage(tr("Added room %1 (%2)").arg(newId).arg(direction.toUpper()));
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

void MainWindow::applyToSelectedRooms(const QString &label, const std::function<void(Room &)> &fn)
{
    const QList<int> ids = m_scene->selectedRoomIds();
    if (ids.isEmpty()) {
        statusBar()->showMessage(tr("Select one or more rooms first."));
        return;
    }
    m_undo.beginMacro(label);
    for (int id : ids) {
        const Room *r = m_map.roomById(id);
        if (!r)
            continue;
        Room before = *r;
        Room after = before;
        fn(after);
        m_undo.push(new EditRoomCommand(m_scene, id, before, after));
    }
    m_undo.endMacro();
}

void MainWindow::setStartOrEndRoom(bool start)
{
    const QList<int> sel = m_scene->selectedRoomIds();
    if (sel.isEmpty()) {
        statusBar()->showMessage(tr("Select a room first."));
        return;
    }
    const int target = sel.first(); // start/end room is unique
    QList<int> changed;
    for (const Room &room : m_map.rooms) {
        const bool cur = start ? room.isStartRoom : room.isEndRoom;
        if (cur != (room.id == target))
            changed.append(room.id);
    }
    if (changed.isEmpty())
        return;
    m_undo.beginMacro(start ? tr("Set Start Room") : tr("Set End Room"));
    for (int id : changed) {
        const Room *r = m_map.roomById(id);
        if (!r)
            continue;
        Room before = *r;
        Room after = before;
        (start ? after.isStartRoom : after.isEndRoom) = (id == target);
        m_undo.push(new EditRoomCommand(m_scene, id, before, after));
    }
    m_undo.endMacro();
}

void MainWindow::setSelectedRoomShape(int shape)
{
    applyToSelectedRooms(tr("Set Room Shape"), [shape](Room &r) {
        r.ellipse = r.octagonal = r.roundedCorners = false;
        if (shape == 1)
            r.roundedCorners = true;
        else if (shape == 2)
            r.ellipse = true;
        else if (shape == 3)
            r.octagonal = true;
    });
}

void MainWindow::joinSelectedRooms()
{
    const QList<int> sel = m_scene->selectedRoomIds();
    if (sel.size() != 2) {
        statusBar()->showMessage(tr("Select exactly two rooms to join."));
        return;
    }
    const Room *a = m_map.roomById(sel.at(0));
    const Room *b = m_map.roomById(sel.at(1));
    if (!a || !b)
        return;
    // Skip if a connection already docks both of these rooms.
    for (const Connection &c : m_map.connections) {
        bool touchesA = false;
        bool touchesB = false;
        for (const Vertex &v : c.vertices) {
            if (v.docked && v.roomId == a->id)
                touchesA = true;
            if (v.docked && v.roomId == b->id)
                touchesB = true;
        }
        if (touchesA && touchesB) {
            statusBar()->showMessage(tr("Those rooms are already connected."));
            return;
        }
    }
    m_undo.push(new AddConnectionCommand(m_scene, a->id, Map::portFacing(*a, *b), b->id,
                                         Map::portFacing(*b, *a)));
}

void MainWindow::swapSelectedRooms(int mode)
{
    const QList<int> sel = m_scene->selectedRoomIds();
    if (sel.size() != 2) {
        statusBar()->showMessage(tr("Select exactly two rooms to swap."));
        return;
    }
    const Room *ra = m_map.roomById(sel.at(0));
    const Room *rb = m_map.roomById(sel.at(1));
    if (!ra || !rb)
        return;
    const Room a = *ra;
    const Room b = *rb;
    Room na = a;
    Room nb = b;

    auto copyFormat = [](Room &dst, const Room &src) {
        dst.fill = src.fill;
        dst.secondFill = src.secondFill;
        dst.secondFillLocation = src.secondFillLocation;
        dst.border = src.border;
        dst.largeText = src.largeText;
        dst.subtitleColor = src.subtitleColor;
        dst.smallText = src.smallText;
        dst.borderStyle = src.borderStyle;
        dst.ellipse = src.ellipse;
        dst.octagonal = src.octagonal;
        dst.roundedCorners = src.roundedCorners;
        dst.allCornersEqual = src.allCornersEqual;
        dst.cornerTopLeft = src.cornerTopLeft;
        dst.cornerTopRight = src.cornerTopRight;
        dst.cornerBottomLeft = src.cornerBottomLeft;
        dst.cornerBottomRight = src.cornerBottomRight;
        dst.isDark = src.isDark;
        dst.handDrawn = src.handDrawn;
    };

    switch (mode) {
    case 0: // objects
        na.objectsText = b.objectsText;
        na.objectsPosition = b.objectsPosition;
        nb.objectsText = a.objectsText;
        nb.objectsPosition = a.objectsPosition;
        break;
    case 1: // names (and subtitles)
        na.name = b.name;
        na.subtitle = b.subtitle;
        nb.name = a.name;
        nb.subtitle = a.subtitle;
        break;
    case 2: // formats / fills
        copyFormat(na, b);
        copyFormat(nb, a);
        break;
    case 3: // regions
        na.region = b.region;
        nb.region = a.region;
        break;
    default:
        return;
    }
    m_undo.beginMacro(tr("Swap Rooms"));
    m_undo.push(new EditRoomCommand(m_scene, a.id, a, na));
    m_undo.push(new EditRoomCommand(m_scene, b.id, b, nb));
    m_undo.endMacro();
}

void MainWindow::renameSelectedRoom()
{
    const int id = m_scene->selectedRoomId();
    const Room *r = m_map.roomById(id);
    if (!r) {
        statusBar()->showMessage(tr("Select a room to rename."));
        return;
    }
    bool ok = false;
    const QString name =
        QInputDialog::getText(this, tr("Rename Room"), tr("Name:"), QLineEdit::Normal, r->name, &ok);
    if (!ok)
        return;
    Room before = *r;
    Room after = before;
    after.name = name;
    m_undo.push(new EditRoomCommand(m_scene, id, before, after));
}

void MainWindow::changeSelectedRegion()
{
    if (m_scene->selectedRoomIds().isEmpty()) {
        statusBar()->showMessage(tr("Select one or more rooms first."));
        return;
    }
    QStringList regions;
    for (const Region &rg : m_map.regions)
        regions << rg.name;
    if (regions.isEmpty())
        regions << kNoRegion;
    bool ok = false;
    const QString region =
        QInputDialog::getItem(this, tr("Change Region"), tr("Region:"), regions, 0, false, &ok);
    if (!ok)
        return;
    applyToSelectedRooms(tr("Change Region"), [region](Room &r) { r.region = region; });
}

namespace {
const char *const kTrizbortMime = "application/x-trizbort";
}

void MainWindow::copySelection()
{
    const QList<int> roomIds = m_scene->selectedRoomIds();
    const QList<int> connIds = m_scene->selectedConnectionIds();
    if (roomIds.isEmpty() && connIds.isEmpty()) {
        statusBar()->showMessage(tr("Nothing selected to copy."));
        return;
    }
    const QSet<int> roomSet(roomIds.begin(), roomIds.end());
    const QSet<int> connSet(connIds.begin(), connIds.end());

    Map temp;
    QSet<QString> regionsUsed;
    for (int id : roomIds) {
        if (const Room *r = m_map.roomById(id)) {
            temp.rooms.append(*r);
            regionsUsed.insert(r->region);
        }
    }
    // Copy connections explicitly selected, plus those whose docked endpoints are
    // all within the copied room set (a connection internal to the selection).
    for (const Connection &c : m_map.connections) {
        bool anyDock = false;
        bool allInside = true;
        for (const Vertex &v : c.vertices) {
            if (v.docked) {
                anyDock = true;
                if (!roomSet.contains(v.roomId))
                    allInside = false;
            }
        }
        if (connSet.contains(c.id) || (anyDock && allInside))
            temp.connections.append(c);
    }
    for (const Region &rg : m_map.regions) {
        if (regionsUsed.contains(rg.name))
            temp.regions.append(rg);
    }

    const QString xml = TrizbortWriter::toString(temp);
    auto *mime = new QMimeData;
    mime->setData(QString::fromLatin1(kTrizbortMime), xml.toUtf8());
    mime->setText(xml);
    QApplication::clipboard()->setMimeData(mime);
    statusBar()->showMessage(
        tr("Copied %1 rooms, %2 connections.").arg(temp.rooms.size()).arg(temp.connections.size()));
}

void MainWindow::paste()
{
    const QMimeData *mime = QApplication::clipboard()->mimeData();
    QString xml;
    if (mime && mime->hasFormat(QString::fromLatin1(kTrizbortMime)))
        xml = QString::fromUtf8(mime->data(QString::fromLatin1(kTrizbortMime)));
    else if (mime && mime->hasText())
        xml = mime->text();
    if (xml.isEmpty()) {
        statusBar()->showMessage(tr("Nothing to paste."));
        return;
    }
    Map temp;
    if (!TrizbortReader::loadFromString(xml, temp, nullptr) ||
        (temp.rooms.isEmpty() && temp.connections.isEmpty())) {
        statusBar()->showMessage(tr("The clipboard does not contain map elements."));
        return;
    }

    ReplaceContentCommand::Content oldContent{m_map.rooms, m_map.connections, m_map.regions};
    ReplaceContentCommand::Content newContent = oldContent;

    int maxRoomId = 0;
    int maxConnId = 0;
    int maxSeq = 0;
    for (const Room &r : m_map.rooms) {
        maxRoomId = std::max(maxRoomId, r.id);
        maxSeq = std::max(maxSeq, r.seq);
    }
    for (const Connection &c : m_map.connections) {
        maxConnId = std::max(maxConnId, c.id);
        maxSeq = std::max(maxSeq, c.seq);
    }

    const double off = m_map.settings.gridSize > 1.0 ? m_map.settings.gridSize : 32.0;
    QHash<int, int> roomIdMap;
    QList<int> pastedRoomIds;
    for (Room r : temp.rooms) {
        const int newId = ++maxRoomId;
        roomIdMap.insert(r.id, newId);
        r.id = newId;
        r.seq = ++maxSeq;
        r.x += off;
        r.y += off;
        newContent.rooms.append(r);
        pastedRoomIds.append(newId);
    }
    for (Connection c : temp.connections) {
        bool ok = true;
        for (Vertex &v : c.vertices) {
            if (v.docked) {
                if (roomIdMap.contains(v.roomId))
                    v.roomId = roomIdMap.value(v.roomId);
                else
                    ok = false; // endpoint outside the paste set
            } else {
                v.point += QPointF(off, off);
            }
        }
        if (!ok)
            continue;
        c.id = ++maxConnId;
        c.seq = ++maxSeq;
        newContent.connections.append(c);
    }
    for (const Region &rg : temp.regions) {
        bool exists = false;
        for (const Region &e : newContent.regions) {
            if (e.name == rg.name) {
                exists = true;
                break;
            }
        }
        if (!exists)
            newContent.regions.append(rg);
    }

    m_undo.push(new ReplaceContentCommand(m_scene, oldContent, newContent, tr("Paste")));
    m_scene->selectRoomsByIds(pastedRoomIds);
    statusBar()->showMessage(tr("Pasted %1 rooms.").arg(pastedRoomIds.size()));
}

void MainWindow::copyColor()
{
    const int id = m_scene->selectedRoomId();
    const Room *r = m_map.roomById(id);
    if (!r) {
        statusBar()->showMessage(tr("Select a room to copy its colours."));
        return;
    }
    m_copiedColors = *r;
    m_hasCopiedColors = true;
    statusBar()->showMessage(tr("Copied colours. Select rooms and use Paste Colour (Ctrl+Alt+V)."));
}

void MainWindow::pasteColor()
{
    if (!m_hasCopiedColors) {
        statusBar()->showMessage(tr("Copy a room's colours first (Ctrl+Alt+C)."));
        return;
    }
    const Room src = m_copiedColors;
    applyToSelectedRooms(tr("Paste Colour"), [src](Room &r) {
        r.fill = src.fill;
        r.secondFill = src.secondFill;
        r.secondFillLocation = src.secondFillLocation;
        r.border = src.border;
        r.largeText = src.largeText;
        r.subtitleColor = src.subtitleColor;
        r.smallText = src.smallText;
    });
}

void MainWindow::selectSpecial(int kind)
{
    if (kind <= 2) {
        QSet<int> connectedRooms;
        for (const Connection &c : m_map.connections)
            for (const Vertex &v : c.vertices)
                if (v.docked)
                    connectedRooms.insert(v.roomId);
        QList<int> ids;
        for (const Room &r : m_map.rooms) {
            const bool hasObjects = !r.objectsText.trimmed().isEmpty();
            bool sel = false;
            if (kind == 0)
                sel = !connectedRooms.contains(r.id);
            else if (kind == 1)
                sel = hasObjects;
            else
                sel = !hasObjects;
            if (sel)
                ids.append(r.id);
        }
        m_scene->selectRoomsByIds(ids);
        statusBar()->showMessage(tr("Selected %1 rooms.").arg(ids.size()));
        return;
    }
    QList<int> ids;
    for (const Connection &c : m_map.connections) {
        bool sel = false;
        if (kind == 3) {
            sel = true;
        } else if (kind == 4) {
            for (const Vertex &v : c.vertices)
                if (!v.docked) {
                    sel = true;
                    break;
                }
        } else {
            QSet<int> rooms;
            int dockCount = 0;
            for (const Vertex &v : c.vertices) {
                if (v.docked) {
                    rooms.insert(v.roomId);
                    ++dockCount;
                }
            }
            sel = (dockCount >= 2 && rooms.size() == 1);
        }
        if (sel)
            ids.append(c.id);
    }
    m_scene->selectConnectionsByIds(ids);
    statusBar()->showMessage(tr("Selected %1 connections.").arg(ids.size()));
}

void MainWindow::exportToClipboard(const QString &format)
{
    auto exporter =
        makeExporter(format, m_map, m_filePath.isEmpty() ? QStringLiteral("map") : m_filePath);
    if (!exporter)
        return;
    QApplication::clipboard()->setText(exporter->exportToString());
    statusBar()->showMessage(tr("Copied the %1 export to the clipboard.").arg(format));
}

void MainWindow::backupMap()
{
    if (m_filePath.isEmpty()) {
        statusBar()->showMessage(tr("Save the map before backing it up."));
        return;
    }
    const QFileInfo fi(m_filePath);
    for (int n = 1; n <= 9999; ++n) {
        const QString candidate = QStringLiteral("%1/%2 (%3).%4")
                                      .arg(fi.absolutePath(), fi.completeBaseName())
                                      .arg(n)
                                      .arg(fi.suffix());
        if (QFile::exists(candidate))
            continue;
        if (QFile::copy(m_filePath, candidate))
            statusBar()->showMessage(tr("Backed up to %1").arg(QFileInfo(candidate).fileName()));
        else
            QMessageBox::warning(this, tr("Backup Failed"), tr("Could not write %1").arg(candidate));
        return;
    }
}

void MainWindow::smartSave()
{
    if (m_filePath.isEmpty()) {
        if (!saveAs())
            return;
    } else if (!save()) {
        return;
    }
    const QFileInfo fi(m_filePath);
    const QString base = fi.absolutePath() + QLatin1Char('/') + fi.completeBaseName();
    QString error;
    bool ok = renderMapToPdf(m_map, base + QStringLiteral(".pdf"), &error);
    ok = renderMapToImage(m_map, base + QStringLiteral(".png"), &error) && ok;
    statusBar()->showMessage(ok ? tr("Smart-saved the map, a PDF and a PNG.")
                                : tr("Smart save error: %1").arg(error));
}

void MainWindow::setWatchedFile(const QString &path)
{
    if (!m_watcher) {
        m_watcher = new QFileSystemWatcher(this);
        connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &p) {
            // Ignore the change our own save just made.
            if (QDateTime::currentMSecsSinceEpoch() - m_lastSaveMs < 1500) {
                if (m_watcher && QFile::exists(p) && !m_watcher->files().contains(p))
                    m_watcher->addPath(p);
                return;
            }
            if (m_watcher && QFile::exists(p) && !m_watcher->files().contains(p))
                m_watcher->addPath(p);
            reloadFromDisk();
        });
    }
    if (!m_watcher->files().isEmpty())
        m_watcher->removePaths(m_watcher->files());
    if (!path.isEmpty() && QFile::exists(path))
        m_watcher->addPath(path);
}

void MainWindow::reloadFromDisk()
{
    if (m_filePath.isEmpty() || !QFile::exists(m_filePath))
        return;
    if (!m_undo.isClean()) {
        statusBar()->showMessage(
            tr("The file changed on disk; unsaved changes were kept (not reloaded)."));
        return;
    }
    const QString path = m_filePath;
    loadFile(path);
    statusBar()->showMessage(
        tr("Reloaded %1 after an external change.").arg(QFileInfo(path).fileName()));
}

void MainWindow::loadPreferences()
{
    QSettings s;
    m_recentFiles = s.value(QStringLiteral("recentFiles")).toStringList();
    m_loadLastOnStart = s.value(QStringLiteral("loadLastOnStart"), false).toBool();
    m_showFullPath = s.value(QStringLiteral("showFullPathInTitle"), false).toBool();
    m_invertWheel = s.value(QStringLiteral("invertWheelZoom"), false).toBool();
    const QByteArray geometry = s.value(QStringLiteral("windowGeometry")).toByteArray();
    if (!geometry.isEmpty())
        restoreGeometry(geometry);
}

void MainWindow::savePreferences()
{
    QSettings s;
    s.setValue(QStringLiteral("recentFiles"), m_recentFiles);
    s.setValue(QStringLiteral("loadLastOnStart"), m_loadLastOnStart);
    s.setValue(QStringLiteral("showFullPathInTitle"), m_showFullPath);
    s.setValue(QStringLiteral("invertWheelZoom"), m_invertWheel);
    s.setValue(QStringLiteral("windowGeometry"), saveGeometry());
}

void MainWindow::addRecentFile(const QString &path)
{
    if (path.isEmpty())
        return;
    const QString abs = QFileInfo(path).absoluteFilePath();
    m_recentFiles.removeAll(abs);
    m_recentFiles.prepend(abs);
    while (m_recentFiles.size() > 8)
        m_recentFiles.removeLast();
    rebuildRecentMenu();
    savePreferences();
}

void MainWindow::rebuildRecentMenu()
{
    if (!m_recentMenu)
        return;
    m_recentMenu->clear();
    // Prune entries whose files have gone away.
    for (int i = m_recentFiles.size() - 1; i >= 0; --i)
        if (!QFile::exists(m_recentFiles.at(i)))
            m_recentFiles.removeAt(i);
    if (m_recentFiles.isEmpty()) {
        QAction *none = m_recentMenu->addAction(tr("(no recent maps)"));
        none->setEnabled(false);
        return;
    }
    int n = 1;
    for (const QString &path : m_recentFiles) {
        const QString label = QStringLiteral("&%1  %2").arg(n++).arg(QFileInfo(path).fileName());
        m_recentMenu->addAction(label, this, [this, path] {
            if (maybeSave())
                loadFile(path);
        });
    }
    m_recentMenu->addSeparator();
    m_recentMenu->addAction(tr("&Clear List"), this, [this] {
        m_recentFiles.clear();
        rebuildRecentMenu();
        savePreferences();
    });
}

void MainWindow::showAppSettings()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Application Settings"));
    auto *form = new QFormLayout(&dialog);

    auto *invert = new QCheckBox(tr("Invert mouse-wheel zoom direction"), &dialog);
    invert->setChecked(m_invertWheel);
    form->addRow(invert);
    auto *loadLast = new QCheckBox(tr("Open the last map on start-up"), &dialog);
    loadLast->setChecked(m_loadLastOnStart);
    form->addRow(loadLast);
    auto *fullPath = new QCheckBox(tr("Show the full path in the title bar"), &dialog);
    fullPath->setChecked(m_showFullPath);
    form->addRow(fullPath);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form->addRow(buttons);

    if (dialog.exec() != QDialog::Accepted)
        return;
    m_invertWheel = invert->isChecked();
    m_loadLastOnStart = loadLast->isChecked();
    m_showFullPath = fullPath->isChecked();
    m_view->setInvertWheelZoom(m_invertWheel);
    updateTitle();
    savePreferences();
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

    const QHash<QString, QString> renames = dialog.regionRenames();
    const QStringList removedList = dialog.removedRegionNames();
    const QSet<QString> removed(removedList.begin(), removedList.end());

    m_undo.beginMacro(tr("Map Settings"));
    m_undo.push(new EditSettingsCommand(m_scene, m_map.settings, m_map.regions,
                                        dialog.resultSettings(), dialog.resultRegions()));

    // Propagate a region rename to the rooms that used it, and reassign rooms
    // whose region was deleted back to the default region.
    if (!renames.isEmpty() || !removed.isEmpty()) {
        QList<int> ids;
        for (const Room &room : m_map.rooms)
            ids.append(room.id);
        for (int id : ids) {
            const Room *r = m_map.roomById(id);
            if (!r)
                continue;
            QString newRegion = r->region;
            if (renames.contains(r->region))
                newRegion = renames.value(r->region);
            else if (removed.contains(r->region))
                newRegion = kNoRegion;
            if (newRegion != r->region) {
                Room before = *r;
                Room after = before;
                after.region = newRegion;
                m_undo.push(new EditRoomCommand(m_scene, id, before, after));
            }
        }
    }
    m_undo.endMacro();
}

void MainWindow::updateTitle()
{
    QString name;
    if (m_filePath.isEmpty())
        name = tr("Untitled");
    else
        name = m_showFullPath ? m_filePath : QFileInfo(m_filePath).fileName();
    setWindowTitle(tr("%1[*] — Trizbort (Qt)").arg(name));
    setWindowModified(!m_undo.isClean());
    // Keep the grid/snap toggles in sync with the current document's settings.
    if (m_gridAction)
        m_gridAction->setChecked(m_map.settings.gridVisible);
    if (m_snapAction)
        m_snapAction->setChecked(m_map.settings.snapToGrid);
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
    if (maybeSave()) {
        savePreferences();
        event->accept();
    } else {
        event->ignore();
    }
}

} // namespace trizbort
