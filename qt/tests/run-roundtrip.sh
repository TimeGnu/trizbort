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
# Validate that loading a .trizbort map and saving it back preserves the whole
# document. For every sample we check two things:
#   1. Save is a fixed point: save(load(f)) == save(load(save(load(f)))).
#   2. Nothing export-relevant is lost: the eight exporters produce byte-for-byte
#      the same golden output whether run on the original map or on the re-saved
#      one (load -> save -> reload -> export == golden).
# Usage: sh qt/tests/run-roundtrip.sh [path-to-trizbort-qt]

set -u

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
BIN=${1:-"$ROOT/qt/build/trizbort-qt"}
GOLDEN="$ROOT/qt/tests/golden"
SAMPLES="$ROOT/samples"

export QT_QPA_PLATFORM=offscreen

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
# Re-saved maps keep the ORIGINAL basename (in separate dirs), because the
# exporters fall back to the file's base name for an untitled map's title.
mkdir -p "$tmp/rt1" "$tmp/rt2"

FORMATS="zil:--zil adv:--adventuron hug:--hugo i:--alan t:--tads inf:--inform6 ni:--inform7 aslx:--quest"

pass=0
fail=0

normalize() {
    if [ "$1" = "aslx" ]; then
        sed -e 's:<gameid>[^<]*</gameid>:<gameid>NORMALIZED</gameid>:' \
            -e 's:<firstpublished>[^<]*</firstpublished>:<firstpublished>NORMALIZED</firstpublished>:' \
            "$2"
    else
        cat "$2"
    fi
}

for m in "$SAMPLES"/*.trizbort; do
    base=$(basename "${m%.trizbort}")

    # 1. Save idempotence.
    s1="$tmp/rt1/$base.trizbort"
    s2="$tmp/rt2/$base.trizbort"
    if ! "$BIN" "$m" --save "$s1" >/dev/null 2>&1; then
        echo "FAIL (save) $base"; fail=$((fail + 1)); continue
    fi
    if ! "$BIN" "$s1" --save "$s2" >/dev/null 2>&1; then
        echo "FAIL (resave) $base"; fail=$((fail + 1)); continue
    fi
    if diff -q "$s1" "$s2" >/dev/null 2>&1; then
        pass=$((pass + 1))
    else
        echo "FAIL (not idempotent) $base"
        diff "$s1" "$s2" | head -12
        fail=$((fail + 1))
    fi

    # 2. Export-from-resaved matches golden.
    for fmt in $FORMATS; do
        ext=${fmt%%:*}
        flag=${fmt##*:}
        gold="$GOLDEN/$base/$base.$ext"
        [ -f "$gold" ] || continue
        out="$tmp/$base.$ext"
        if ! "$BIN" "$s1" "$flag" "$out" >/dev/null 2>&1; then
            echo "FAIL (run)  $base.$ext (from re-saved)"; fail=$((fail + 1)); continue
        fi
        normalize "$ext" "$gold" >"$tmp/g.norm"
        normalize "$ext" "$out" >"$tmp/o.norm"
        if diff -q "$tmp/g.norm" "$tmp/o.norm" >/dev/null 2>&1; then
            pass=$((pass + 1))
        else
            echo "FAIL (diff) $base.$ext (from re-saved)"
            diff "$tmp/g.norm" "$tmp/o.norm" | head -16
            fail=$((fail + 1))
        fi
    done
done

echo "round-trip: PASS=$pass FAIL=$fail"
[ "$fail" -eq 0 ]
