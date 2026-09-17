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

#ifndef TRIZBORT_MAINWINDOW_H
#define TRIZBORT_MAINWINDOW_H

#include <functional>

#include <QMainWindow>
#include <QUndoStack>

#include "MapDocument.h"

class QAction;
class QDockWidget;
class QFileSystemWatcher;
class QLabel;
class QMenu;
class QTimer;

namespace trizbort {

class MapScene;
class MapView;
class TranscriptAutomapper;
class GuiAutomapController;

class MainWindow : public QMainWindow {
    Q_OBJECT

    // Headless self-test seam (main.cpp): drives the keyboard handlers against a
    // real window and inspects the model. Keeps the handlers out of the public API.
    friend int runKeyboardSelftest();

public:
    explicit MainWindow(QWidget *parent = nullptr);

    bool loadFile(const QString &path);

protected:
    void closeEvent(QCloseEvent *event) override;
    // Keyboard editing/navigation on the canvas (the C# Canvas key handlers):
    // arrow-key nudge/scroll, Ctrl+arrow select-or-add, Shift+arrow follow a
    // connection, Ctrl+Alt+arrow resize, and numeric-keypad navigation.
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void newFile();
    void openFile();
    void openFromUrl();
    bool save();
    bool saveAs();
    void exportMap(const QString &format);
    void exportImage();
    void exportPdf();
    void importTranscript();
    void startLiveAutomap();
    void stopLiveAutomap();
    void abandonLiveAutomap(); // tear down without recording an undo step
    void automapTick();
    void addRoom();
    void addConnectedRoom(const QString &direction);

    // Keyboard-editing helpers (see eventFilter). Directions are compass tokens
    // ("n","s","e","w","ne","nw","se","sw").
    bool handleCanvasKey(QKeyEvent *event);
    // Follow a connection docked at the selected room toward the given compass
    // direction (exact, then ±45°) and return the room at the far end, or -1.
    int roomThroughConnection(int roomId, const QString &dir) const;
    void nudgeSelection(double dx, double dy);              // move selected elements
    void keyboardResizeRooms(const QString &dir);           // Ctrl+Alt+arrow
    // Select the room reached through a connection in `dir`; when none exists,
    // optionally add a connected room, or add an unexplored dangling exit.
    void navigateOrAdd(const QString &dir, bool allowCreate, bool unexploredStub);
    void addUnexploredExit(int roomId, const QString &dir);
    void addConnectedRoomLabeled(const QString &placementDir, const QString &startLabel,
                                 const QString &endLabel, const QString &displayName);
    void deleteSelection();
    void toggleConnectMode(bool on);
    void editMapProperties();
    void editMapSettings();
    void editRoom(int roomId);
    void editConnection(int connId);

private:
    // Apply an edit to every selected room as one undoable step.
    void applyToSelectedRooms(const QString &label, const std::function<void(Room &)> &fn);
    void applyToSelectedConnections(const QString &label,
                                    const std::function<void(Connection &)> &fn);
    void reverseSelectedConnections();
    void rotateSelectedConnectors(bool source);
    void insertRoomOnConnection();
    void setStartOrEndRoom(bool start);
    void setSelectedRoomShape(int shape); // 0 square,1 rounded,2 ellipse,3 octagonal
    void joinSelectedRooms();
    void swapSelectedRooms(int mode);     // 0 objects,1 names,2 formats,3 regions
    void renameSelectedRoom();
    void changeSelectedRegion();

    // Clipboard.
    void copySelection();
    void paste();
    void copyColor();
    void pasteColor();

    // Export / file helpers.
    void exportToClipboard(const QString &format);
    void backupMap();
    void smartSave();
    void setWatchedFile(const QString &path);
    void reloadFromDisk();

    // Application preferences and recent-files (persisted via QSettings).
    void loadPreferences();
    void savePreferences();
    void addRecentFile(const QString &path);
    void rebuildRecentMenu();
    void showAppSettings();

    // Selection commands. kind: 0 unconnected rooms, 1 rooms w/ objects,
    // 2 rooms w/o objects, 3 all connections, 4 dangling connections,
    // 5 self-looping connections.
    void selectSpecial(int kind);

    void createActions();
    void updateTitle();
    bool maybeSave();                 // returns false to cancel the pending action
    bool writeToPath(const QString &path);

    Map m_map;
    QString m_filePath;
    QUndoStack m_undo;
    QFileSystemWatcher *m_watcher = nullptr;
    qint64 m_lastSaveMs = 0; // ignore watcher events right after our own save

    // Live automap: an incremental, controller-driven engine fed the transcript
    // as it grows on disk.
    QTimer *m_automapTimer = nullptr;
    QString m_automapPath;
    qint64 m_automapSize = -1;
    int m_automapFedLines = 0;        // lines already fed to the engine
    Map m_automapPreMap;              // map content captured when automap started
    QAction *m_automapStopAction = nullptr;
    QAction *m_automapStepAction = nullptr;
    QAction *m_automapRunAction = nullptr;
    bool m_automapping = false;
    bool m_automapBusy = false;       // guard against re-entrant ticks while stepping
    TranscriptAutomapper *m_automapper = nullptr;
    GuiAutomapController *m_automapController = nullptr;

    MapScene *m_scene = nullptr;
    MapView *m_view = nullptr;
    QAction *m_connectAction = nullptr;
    QAction *m_gridAction = nullptr;
    QAction *m_snapAction = nullptr;
    QDockWidget *m_minimapDock = nullptr;
    QLabel *m_zoomLabel = nullptr;
    QMenu *m_recentMenu = nullptr;
    QStringList m_recentFiles;
    bool m_loadLastOnStart = false;
    bool m_showFullPath = false;
    bool m_invertWheel = false;

    // Smart Save: which artifacts to write alongside the project (the C#
    // SaveToPDF / SaveToImage app settings), and the image format to use.
    bool m_smartSavePdf = true;
    bool m_smartSaveImage = true;
    QString m_smartSaveImageFormat = QStringLiteral("png");
    // When true, image/PDF exports render at 100% (1 unit per pixel); when false
    // they render at the current view zoom (the C# SaveAt100 app setting).
    bool m_saveAt100 = true;
};

} // namespace trizbort

#endif // TRIZBORT_MAINWINDOW_H
