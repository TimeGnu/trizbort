# Copyright (c) 2026  Jason Self <j@jxself.org>
# GNU GPL v3 or later; see qt/COPYING.
#
# Resolve the program version with a portable fallback chain, so it works when
# cross-compiling for Windows from GNU/Linux, when building natively (including
# with Git for Windows' sh), from a release tarball with no git, and inside a
# Debian build with no git at all:
#
#   1. -DTRIZBORT_VERSION=... (explicit override; the Debian packaging passes
#      the changelog's upstream version here, keeping the .deb authoritative).
#   2. version.sh run through a POSIX shell, if a shell is found. The script
#      itself prefers a baked .tarball-version, then derives from git metadata.
#   3. A baked .tarball-version read directly (covers "no shell available").
#   4. A hardcoded last resort, so the build never fails for lack of a version.
#
# This runs at configure time; re-run CMake to refresh the version after new
# commits.
function(trizbort_determine_version out_var)
    set(_srcdir "${CMAKE_CURRENT_SOURCE_DIR}")

    # 1. Explicit override (cache variable, e.g. -DTRIZBORT_VERSION=1.2.3).
    if(DEFINED TRIZBORT_VERSION AND NOT "${TRIZBORT_VERSION}" STREQUAL "")
        set(${out_var} "${TRIZBORT_VERSION}" PARENT_SCOPE)
        return()
    endif()

    # 2. version.sh via a POSIX shell (sh on POSIX; sh.exe from Git for Windows).
    find_program(TRIZBORT_SH NAMES sh bash dash)
    if(TRIZBORT_SH AND EXISTS "${_srcdir}/version.sh")
        execute_process(
            COMMAND "${TRIZBORT_SH}" "${_srcdir}/version.sh"
            WORKING_DIRECTORY "${_srcdir}"
            OUTPUT_VARIABLE _v
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            RESULT_VARIABLE _rc)
        if(_rc EQUAL 0 AND NOT "${_v}" STREQUAL "")
            set(${out_var} "${_v}" PARENT_SCOPE)
            return()
        endif()
    endif()

    # 3. Baked release version, no shell required.
    if(EXISTS "${_srcdir}/.tarball-version")
        file(READ "${_srcdir}/.tarball-version" _v)
        string(STRIP "${_v}" _v)
        if(NOT "${_v}" STREQUAL "")
            set(${out_var} "${_v}" PARENT_SCOPE)
            return()
        endif()
    endif()

    # 4. Last resort.
    set(${out_var} "0-unknown" PARENT_SCOPE)
endfunction()
