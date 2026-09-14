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

if [ "$fail" -eq 0 ]; then
    echo "editor: PASS"
else
    echo "editor: FAIL"
fi
[ "$fail" -eq 0 ]
