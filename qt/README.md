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

A working editor with a complete file format and all exporters.

- **Complete `.trizbort` file format**, modelled with typed fields (not a
  subset): every room, connection, region and settings attribute. Loading and
  saving round-trip losslessly — re-saving a modern-format map is byte-identical
  to the original, and older maps are upgraded to the current format exactly as
  Trizbort itself would. Verified by `qt/tests/run-roundtrip.sh`.
- **All eight exporters** — ZIL, Inform 6, Inform 7, TADS 3, Hugo, Alan, Quest,
  Adventuron — byte-identical to the C# exporters across the whole golden corpus
  (`qt/tests/run-golden.sh`, in CI), including the door and object-property
  paths exercised by dedicated feature maps.
- **Interactive editor** (`QGraphicsView`): create, move (grid-snapped), edit
  and delete rooms; draw, edit and delete connections; a full properties dialog
  for rooms (name, region, description, objects, shape, colours, dark/start/end)
  and connections (flow, style, texts, doors); map properties (title/author/…).
- **File & export UI**: New / Open / Save / Save As with unsaved-change
  prompts, and an Export menu covering every format. Rubber-band selection,
  wheel zoom, middle-button pan.

The editing layer is covered headlessly by `qt/tests/run-editor.sh`. Still to
come: undo/redo, automap, and the finer rendering options of the C# original.

## Building (GNU/Linux)

Requires Qt 6 (Widgets) and CMake ≥ 3.16. On Trisquel 12 / Ubuntu 22.04+:

```sh
sudo apt install qt6-base-dev cmake g++
cmake -S qt -B qt/build
cmake --build qt/build -j
./qt/build/trizbort-qt [path/to/map.trizbort]
```

### Command line

```sh
trizbort-qt map.trizbort                 # open in the editor
trizbort-qt map.trizbort --save out.trizbort   # load and re-save (round-trip)
trizbort-qt map.trizbort --zil out.zil   # export (--inform6/7, --tads, --hugo,
                                         #   --alan, --quest, --adventuron, …)
trizbort-qt map.trizbort --render out.png
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
