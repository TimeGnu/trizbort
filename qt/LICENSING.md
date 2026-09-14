# Licensing policy for the Qt/C++ Trizbort

This directory (`qt/`) is a new, from-scratch reimplementation of Trizbort in
C++/Qt. It is licensed **GNU GPL, version 3 or later**. The full license text
is in [`COPYING`](COPYING).

The rest of the repository (the original C#/.NET Trizbort) remains under its
original **MIT** license (see the top-level `LICENSE.txt`). Nothing here
changes that.

There are two kinds of files in this directory, with two different headers.

**Default for this project:** because the entire `qt/` reimplementation was
developed by studying the MIT-licensed C# Trizbort, all `qt/` **source files
carry the dual header** (section 2 below) as a safe, generous default —
over-attribution costs nothing, under-attribution is the real risk. The
GPL-only header (section 1) is reserved for files with no derivation from the
original, such as build scripts (`CMakeLists.txt`) and this repository's own
tooling. Files that are *direct translations* of the C# code (the exporters and
export pipeline) require the dual header regardless.

## 1. Original files (written from scratch)

Files that are our own original work — no code copied or translated from the
MIT-licensed C# sources — are **wholly GPLv3-or-later** and carry only our
copyright. No incorporation notice is needed, because nothing is being adapted.

```c
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
```

## 2. Ported files (adapted from the MIT C# code)

Translating code from one language to another creates a **derivative work**.
So any file that ports logic from the original C# Trizbort (for example, a C++
reimplementation of one of the exporters, written by reading the C# source)
must keep the incorporation notice below the GPL notice, reproducing the
**actual MIT license text and copyright line** of the original.

> Note: Trizbort's real license is **MIT** and its copyright line is
> `Copyright (c) 2010-2022 by Genstein and Jason Lautzenheiser`. Use the exact
> text below — not an ISC-style "with or without fee" notice.

```c
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
```

If a ported file incorporates work from additional contributors whose copyright
is distinct from the line above, add their `Copyright (c) YEARS, Name` lines
inside the incorporation block, above the permission text.

## Documentation files

Markdown docs in this directory are also GPLv3-or-later; for brevity they use a
one-line header instead of the full block:

```
<!-- Copyright (c) 2026 Jason Self <j@jxself.org>. GPLv3-or-later; see qt/COPYING. -->
```
