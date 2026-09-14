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

using CommandLine;

namespace Trizbort.Domain.Application;

public class CommandLineOptions
{
  [Value(0)]
  public string Executable { get; set; }

  [Value(1)]
  public string FileName { get; set; }

  [Option('a', "loadlastproject", HelpText = "Load the last opened project.")]
  public bool LoadLastProject { get; set; }

  [Option('m',"automap", HelpText = "Start automap with given transcript.")]
  public string Transcript { get; set; }

  [Option('q',"quicksave", HelpText="Quick save the map to the current Trizbort file.")]
  public string QuickSave { get; set; }

  [Option('s', "smartsave", HelpText = "SmartSave the loaded file")]
  public bool SmartSave { get; set; }

  [Option('n', "name", HelpText = "Name the current map.")]
  public string Name { get; set; }

  [Option('x', "exit", HelpText = "Exit Trizbort.")]
  public bool Exit { get; set; }

  [Option("inform6", HelpText = "Export to I6.")]
  public string I6 { get; set; }

  [Option("inform7", HelpText = "Export to I7.")]
  public string I7 { get; set; }

  [Option("tads", HelpText = "Export to Tads.")]
  public string Tads { get; set; }

  [Option("alan", HelpText = "Export to Alan.")]
  public string Alan { get; set; }

  [Option("hugo", HelpText = "Export to Hugo.")]
  public string Hugo { get; set; }

  [Option("zil", HelpText = "Export to Zil.")]
  public string Zil { get; set; }

  [Option("quest", HelpText = "Export to Quest.")]
  public string Quest { get; set; }

  [Option("quest rooms", HelpText = "Export to Quest section.")]
  public string QuestRooms { get; set; }

  [Option("adventuron", HelpText = "Export to Adventuron.")]
  public string Adventuron { get; set; }
}