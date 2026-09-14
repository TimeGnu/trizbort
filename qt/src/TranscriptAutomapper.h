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
    // Non-interactive import assumes rooms sharing a name are the same room,
    // which avoids the disambiguation prompts the C# UI shows.
    bool assumeSameNameSameRoom = true;
    double preferredDistanceBetweenRooms = 64.0;
    double gridSize = 32.0;
};

class TranscriptAutomapper {
public:
    explicit TranscriptAutomapper(const AutomapSettings &settings = {});

    // Apply the transcript to the map, mutating it in place. Returns the number
    // of rooms added.
    int run(Map &map, const QStringList &lines);
    int run(Map &map, const QString &text);

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
    int findRoom(const QString &name) const;
    int createRoomInDirection(int existingId, const QString &dir, const QString &roomName,
                              const QString &line);
    int createRoomTeleport(const QString &name);
    void connectRooms(int sourceId, const QString &dir, int targetId);
    bool anyRoomIntersects(const QRectF &bounds, int exceptId) const;
    void shiftMap(const QRectF &origin, double dx, double dy);

    AutomapSettings m_settings;
    Map *m_map = nullptr;

    int m_lastRoomId = -1;
    QString m_lastMoveDir;   // empty => none
    bool m_firstRoom = true;
    QString m_gameName;
    bool m_useDottedConnection = false;

    int m_roomsAdded = 0;
    int m_connectionsAdded = 0;
};

} // namespace trizbort

#endif // TRIZBORT_TRANSCRIPTAUTOMAPPER_H
