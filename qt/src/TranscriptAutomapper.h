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

#ifndef TRIZBORT_TRANSCRIPTAUTOMAPPER_H
#define TRIZBORT_TRANSCRIPTAUTOMAPPER_H

#include <QRectF>
#include <QString>
#include <QStringList>

#include "MapDocument.h"

// Transcript-driven automapper, ported from Automap/Automap.cs (the parser) and
// UI/Controls/Canvas.Automap.cs (the layout). Reads a play-session transcript
// and builds rooms and connections into a Map, placing new rooms a grid step
// away in the movement direction and shifting the map aside to avoid overlaps.
namespace trizbort {

struct AutomapSettings {
    bool assumeTwoWayConnections = true;
    // When true (the non-interactive default), rooms sharing a name are taken to
    // be the same room; when false the controller is asked to disambiguate.
    bool assumeSameNameSameRoom = true;
    // When false the transcript is treated as terse (no room descriptions), so
    // rooms sharing a name are always taken to be the same room without asking
    // (Automap.cs: "must assume room with same name is same room" when not
    // verbose). When true, disambiguation applies as normal.
    bool verboseTranscript = true;
    // Live automap only: skip everything already in the transcript file and
    // start from its end, so only newly-played moves are mapped (C#
    // ContinueTranscript).
    bool continueTranscript = false;
    // Add dangling exit stubs for direction words found in a room's description.
    bool guessExits = false;
    double preferredDistanceBetweenRooms = 64.0;
    double gridSize = 32.0;
    // Customizable command words (the C# AutomapDialog options).
    QString objectCommand = QStringLiteral("tb see");
    QString regionCommand = QStringLiteral("tb region");
};

// Callbacks the automapper uses for interactive decisions and progress. The
// default (base-class) behaviour is non-interactive, matching a batch import;
// the GUI supplies a subclass that shows dialogs and gates single-stepping.
// This mirrors the C# IAutomapCanvas / Automap interaction points.
class AutomapController {
public:
    virtual ~AutomapController() = default;

    enum class SameDir { KeepExisting, KeepNew, KeepBoth };

    // A room name matched several existing rooms: return the chosen room id, or
    // -1 to create a new room. Default: reuse the first candidate.
    virtual int disambiguateRoom(const QString &name, const QList<int> &candidates)
    {
        Q_UNUSED(name);
        return candidates.isEmpty() ? -1 : candidates.first();
    }
    // Moving `dir` from a room, a different room already occupies that spot.
    // Default: keep both (place the new room and shift to make room).
    virtual SameDir sameDirection(int fromRoomId, int existingRoomId, const QString &dir)
    {
        Q_UNUSED(fromRoomId);
        Q_UNUSED(existingRoomId);
        Q_UNUSED(dir);
        return SameDir::KeepBoth;
    }
    // Gate invoked before each transcript command; return false to cancel.
    virtual bool step() { return true; }
    virtual void status(const QString &message) { Q_UNUSED(message); }
};

class TranscriptAutomapper {
public:
    explicit TranscriptAutomapper(const AutomapSettings &settings = {});

    // Apply a whole transcript to the map in one shot (batch). Returns the
    // number of rooms added. Uses the non-interactive default controller.
    int run(Map &map, const QStringList &lines);
    int run(Map &map, const QString &text);

    // Incremental, controller-driven use (live automap / stepping):
    //   begin() binds the map and controller and resets state;
    //   feed() appends transcript lines and processes complete commands
    //   (call with final=true for the last chunk); returns false if cancelled.
    void begin(Map &map, AutomapController *controller);
    bool feed(const QStringList &lines, bool final);

    int roomsAdded() const { return m_roomsAdded; }
    int connectionsAdded() const { return m_connectionsAdded; }

private:
    // Transcript parsing (Automap.cs).
    bool isPrompt(const QString &line, QString &command) const;
    bool extractRoomName(const QString &line, const QString &previousLine, QString &name) const;
    bool extractParagraph(const QStringList &lines, int lineIndex, QString &paragraph) const;
    void processTranscriptText(const QStringList &lines);
    void processPromptCommand(const QString &command);

    // Canvas operations on the map (Canvas.Automap.cs).
    int findRoom(const QString &name);
    int createRoomInDirection(int existingId, const QString &dir, const QString &roomName,
                              const QString &line);
    int createRoomTeleport(const QString &name);
    void connectRooms(int sourceId, const QString &dir, int targetId);
    bool anyRoomIntersects(const QRectF &bounds, int exceptId) const;
    void shiftMap(const QRectF &origin, double dx, double dy);
    // The id of a room already occupying the cell one step `dir` from fromId, or -1.
    int roomInDirection(int fromId, const QString &dir) const;
    void removeRoomById(int roomId);
    // Exit stubs: a dangling connection out of a room in a direction.
    void addExitStub(int roomId, const QString &dir);
    void removeExitStub(int roomId, const QString &dir);
    void guessExitsFromDescription(int roomId, const QString &description);
    // Process one buffered transcript line (prompt or accumulating room text).
    void processBuffer(bool final);

    AutomapSettings m_settings;
    Map *m_map = nullptr;
    AutomapController *m_controller = nullptr;

    int m_lastRoomId = -1;
    QString m_lastMoveDir;   // empty => none
    bool m_firstRoom = true;
    QString m_gameName;
    bool m_useDottedConnection = false;

    // Incremental parsing state.
    QStringList m_buffer;    // all transcript lines fed so far
    int m_pos = 0;           // next unprocessed line in m_buffer
    QStringList m_between;   // lines accumulated since the last prompt
    bool m_cancelled = false;

    int m_roomsAdded = 0;
    int m_connectionsAdded = 0;
};

} // namespace trizbort

#endif // TRIZBORT_TRANSCRIPTAUTOMAPPER_H
