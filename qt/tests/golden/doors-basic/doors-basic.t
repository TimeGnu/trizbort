#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'Doors Test'
    byline = 'By Trizbort Qt port'
    version = '1'
    desc = ''
;

WestRoom: Room 'West Room'

    north = NorthRoom
    east = EastRoom
;

EastRoom: Room 'East Room'

    west = WestRoom
;

NorthRoom: Room 'North Room'

    south = WestRoom
;

me: Actor
    location = WestRoom
;

gameMain: GameMainDef
    initialPlayerChar = me
;

