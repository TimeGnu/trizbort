#!/bin/sh
# Copyright (c) 2026  Jason Self <j@jxself.org>
#
# This file is free software: you may copy, redistribute and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.  There is NO WARRANTY, to the extent permitted by law.
# See the GNU General Public License for more details:
# https://www.gnu.org/licenses/.
#
# Headless checks for the editing layer: the GUI-independent editing API
# (--edit-selftest) and the interactive scene/window machinery (--gui-selftest).
# Usage: sh qt/tests/run-editor.sh [path-to-trizbort-qt]

set -u

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
BIN=${1:-"$ROOT/qt/build/trizbort-qt"}
SAMPLE="$ROOT/samples/luster.trizbort"

export QT_QPA_PLATFORM=offscreen

fail=0

out=$("$BIN" --edit-selftest 2>/dev/null) || fail=1
echo "$out"
echo "$out" | grep -q "edit-selftest: PASS" || fail=1

out=$("$BIN" --gui-selftest "$SAMPLE" 2>/dev/null) || fail=1
echo "$out"
echo "$out" | grep -q "gui-selftest: PASS" || fail=1

out=$("$BIN" --undo-selftest 2>/dev/null) || fail=1
echo "$out"
echo "$out" | grep -q "undo-selftest: PASS" || fail=1

out=$("$BIN" --automap-selftest 2>/dev/null) || fail=1
echo "$out"
echo "$out" | grep -q "automap-selftest: PASS" || fail=1

out=$("$BIN" --transcript-selftest 2>/dev/null) || fail=1
echo "$out"
echo "$out" | grep -q "transcript-selftest: PASS" || fail=1

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

# Import a transcript from the CLI and confirm the result loads and exports.
tsample="$ROOT/testing/transcript-sample.txt"
if [ -f "$tsample" ]; then
    "$BIN" --import-transcript "$tsample" "$tmp/imported.trizbort" >/dev/null 2>&1
    if "$BIN" "$tmp/imported.trizbort" --inform7 "$tmp/imported.ni" >/dev/null 2>&1 \
        && grep -q "Hall of the Guildmasters" "$tmp/imported.ni"; then
        echo "import-transcript-cli: PASS"
    else
        echo "import-transcript-cli: FAIL"; fail=1
    fi
fi

# Image and PDF export produce non-empty, well-formed files.
"$BIN" "$SAMPLE" --render "$tmp/out.png" >/dev/null 2>&1
"$BIN" "$SAMPLE" --pdf "$tmp/out.pdf" >/dev/null 2>&1
if [ -s "$tmp/out.png" ]; then echo "render-png: PASS"; else echo "render-png: FAIL"; fail=1; fi
if head -c 5 "$tmp/out.pdf" 2>/dev/null | grep -q '%PDF'; then
    echo "render-pdf: PASS"
else
    echo "render-pdf: FAIL"; fail=1
fi

if [ "$fail" -eq 0 ]; then
    echo "editor: PASS"
else
    echo "editor: FAIL"
fi
[ "$fail" -eq 0 ]
