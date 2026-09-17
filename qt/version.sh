#!/bin/sh
# Generate a version string from git metadata.
# Called from CMakeLists.txt at configure time (see cmake/GetVersion.cmake).
#
# version.sh distinguishes two kinds of build:
#
#   Release line  -- HEAD is on a release line. version.sh emits a
#                    release number derived from the most recent
#                    reachable release tag:
#                      TAG    -- HEAD is exactly on a release tag (e.g. "3.8")
#                      TAG.N  -- HEAD is N commits past it       (e.g. "3.8.1")
#
#   Development   -- HEAD is not on a release line (e.g. master or a
#                    feature branch). version.sh emits a development
#                    string of the form "dev-COUNT-gHASH" so bug reports
#                    can identify the exact commit.
#
# Release tags are git tags whose names start with a digit (matched by
# the glob "[0-9]*"), which is the convention used by this project.
#
# Membership in a release line is determined TOPOLOGICALLY: HEAD is "on"
# a release line when it is an ancestor of (or equal to) some release-*
# ref (local branch or remote-tracking). This is deliberately
# independent of the checked-out branch name, so it also works in
# detached-HEAD checkouts (CI, tag builds). It also means master never
# inherits a release number merely because it descends from the commit
# a release tag points at -- git tags are not branch-scoped, and
# "git describe" would otherwise walk back through shared history and
# find that tag from master too.
#
# Copyright (C) 2026 Jason Self <j@jxself.org>
#
# You can redistribute and/or modify this file under the terms of the GNU
# General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This file is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file. If not, see <https://www.gnu.org/licenses/>.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# 1. If a baked-in version file exists, use it (release tarball mode).
if [ -f .tarball-version ]; then
    cat .tarball-version
    exit 0
fi

# 2. Determine whether HEAD is on a release line (topological).
#
# HEAD is "on" a release line when it is an ancestor of (or equal to)
# some release-* ref, local or remote-tracking.  This is independent of
# the checked-out branch name, so it also works in detached-HEAD
# checkouts (CI, tag builds).
on_release=
for ref in $(git for-each-ref --format='%(refname)' \
        'refs/heads/release-*' 'refs/remotes/*/release-*' 2>/dev/null); do
    if git merge-base --is-ancestor HEAD "$ref" 2>/dev/null; then
        on_release=yes
        break
    fi
done

# 3. On a release line, derive a version from the most recent release
# tag reachable from HEAD.  A release tag is one whose name starts with
# a digit.  If we are not on a release line (or no such tag is
# reachable), fall through to the development version below.
tag=
if [ -n "$on_release" ]; then
    tag=$(git describe --tags --abbrev=0 --match '[0-9]*' 2>/dev/null)
fi
if [ -n "$tag" ]; then
    count=$(git rev-list "$tag"..HEAD --count 2>/dev/null)
    if [ -z "$count" ] || [ "$count" -eq 0 ]; then
        echo "$tag"
    else
        echo "$tag.$count"
    fi
    exit 0
fi

# 4. Not on a release line -- emit a development version.
commit_count=$(git rev-list HEAD --count 2>/dev/null || echo "0")
short_hash=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")
echo "dev-$commit_count-g$short_hash"
