#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'properties-in-brackets-basic'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Cave: Room 'Cave'

    east = ErrorRoom
;

+ Fred: Thing 'Fred' 'Fred'
;

+ Wilma: Thing 'Wilma' 'Wilma'
;

+ Pat: Thing 'Pat' 'Pat'
;

+ Mice: Thing 'Mice' 'Mice'
;

+ Mess: Thing 'Mess' 'Mess'
;

+ Oven: Thing 'Oven' 'Oven'
;

+ Table: Thing 'Table' 'Table'
;

+ Window: Thing 'Window' 'Window'
;

ErrorRoom: Room 'Error Room'

    west = Cave
;

+ Genders: Thing 'Genders' 'Genders'
;

+ Genders2: Thing 'Genders' 'Genders'
;

+ Genders3: Thing 'Genders' 'Genders'
;

+ Oddperson: Thing 'Oddperson' 'Oddperson'
;

+ Oddperson2: Thing 'Oddperson' 'Oddperson'
;

+ Oddperson3: Thing 'Oddperson' 'Oddperson'
;

+ Singplur: Thing 'Singplur' 'Singplur'
;

me: Actor
    location = Cave
;

gameMain: GameMainDef
    initialPlayerChar = me
;

