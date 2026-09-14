<!-- Copyright (c) 2026 Jason Self <j@jxself.org>. GPLv3-or-later; see qt/COPYING. -->

# Trizbort (Qt/C++)

A from-scratch, cross-platform reimplementation of [Trizbort](https://www.trizbort.com/),
the interactive-fiction mapper, written in **C++17 + Qt 6**. GNU/Linux is the
primary development and testing platform; Windows and macOS are supported as
additional targets from the same source.

This exists because the original Trizbort is a Windows-only C#/WinForms
application that cannot run or be built natively on GNU/Linux. Rather than
depend on Windows or Microsoft's toolchain, this rewrite uses only free software
(Qt, GCC/Clang, CMake) and puts GNU/Linux first.

## Status

Early scaffolding. Current capability:

- Loads a `.trizbort` map file (rooms, connections, regions).
- Renders the map in a pannable / zoomable canvas (`QGraphicsView`).

Not yet: editing, saving, objects/handles, and the code exporters (Inform 6/7,
TADS, ZIL, Hugo, Alan, Quest, Adventuron). Those are the next milestones and are
validated against the specs in [`docs/`](docs/) and a golden-output corpus.

## Building (GNU/Linux)

Requires Qt 6 (Widgets) and CMake ≥ 3.16. On Trisquel 12 / Ubuntu 22.04+:

```sh
sudo apt install qt6-base-dev cmake g++
cmake -S qt -B qt/build
cmake --build qt/build -j
./qt/build/trizbort-qt [path/to/map.trizbort]
```

### Windows builds without Windows

The intended Windows-build path uses free cross-compilation from GNU/Linux
(e.g. MXE — the M cross environment — with MinGW), so no Windows machine or
Microsoft toolchain is required. (A Qt-on-`windows-latest` CI job is an optional
convenience, not a dependency.)

## Licensing

GNU **GPL v3 or later** (see [`COPYING`](COPYING)). The original C# Trizbort
elsewhere in this repository stays under its MIT license. See
[`LICENSING.md`](LICENSING.md) for the file-header policy, including how ported
(derivative) files credit the original MIT authors.

## Relationship to the C# version

The C# Trizbort (repository root) continues as the "legacy" line and still
receives Windows releases, so existing users are not stranded while this
native version reaches parity. The two share the `.trizbort` file format,
documented in [`docs/trizbort-file-format.md`](docs/trizbort-file-format.md).
