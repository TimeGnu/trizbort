#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'object-testing'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

TestRoom: Room 'Test Room'

    east = cave
;

+ MrJones: Container 'Mr Jones' 'Mr Jones'
;

++ Hat: Container 'Hat' 'Hat'
;

+++ Hatband: Thing 'Hatband' 'Hatband'
;

++ fishbowl: Container 'fishbowl' 'fishbowl'
;

+++ goldfish: Thing 'goldfish' 'goldfish'
;

+ MrsJones: Container 'Mrs Jones' 'Mrs Jones'
;

++ rabbit: Thing 'rabbit' 'rabbit'
;

cave: Room 'cave'

    west = TestRoom
;

me: Actor
    location = TestRoom
;

gameMain: GameMainDef
    initialPlayerChar = me
;

