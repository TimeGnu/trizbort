#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'further'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Blue: Region
;

Green: Region
;

Red: Region
;

Yellow: Region
;

FoggyRoom: Room 'Foggy Room'

    north = Prairie
    south = Cave
    east = Hearth
    west = Sandstorm
    northeast = Tundra
;

+ RuddyBell: Thing 'Ruddy Bell' 'Ruddy Bell'
;

Prairie: Room 'Prairie'
    regions = [Green]

    south = FoggyRoom
    northwest = ManicSky
;

Cave: Room 'Cave'
    regions = [Blue]

    north = FoggyRoom
    down = ShiftingGrotto
;

Hearth: Room 'Hearth'
    regions = [Red]

    west = FoggyRoom
    down = LowRoom
;

Sandstorm: Room 'Sandstorm'
    regions = [Yellow]

    east = FoggyRoom
    down = CountlessDunes
;

Tundra: Room 'Tundra'

    northeast = InfiniteIceFloes
    southwest = FoggyRoom
;

ManicSky: Room 'Manic Sky'
    regions = [Green]

    west = Grassland
    southeast = Prairie
;

+ Becomes: Thing 'Becomes' 'Becomes'
;

+ PetShop: Thing 'Pet Shop' 'Pet Shop'
;

Grassland: Room 'Grassland'
    regions = [Green]

    east = ManicSky
;

+ AshenRobe: Thing 'Ashen Robe' 'Ashen Robe'
;

InfiniteIceFloes: Room 'Infinite Ice Floes'

    southwest = Tundra
    up = CementRoom
;

+ BecomesFrozenPond: Thing 'Becomes Frozen Pond' 'Becomes Frozen Pond'
;

LowRoom: Room 'Low Room'
    regions = [Red]

    southeast = Alcove
    up = Hearth
;

+ Becomes2: Thing 'Becomes' 'Becomes'
;

+ YourBasement: Thing 'Your Basement' 'Your Basement'
;

Alcove: Room 'Alcove'
    regions = [Red]

    northwest = LowRoom
;

+ JaundicedPenny: Thing 'Jaundiced Penny' 'Jaundiced Penny'
;

CountlessDunes: Room 'Countless Dunes'
    regions = [Yellow]

    southwest = Oasis
    up = Sandstorm
;

+ BecomesAlansDen: Thing 'Becomes Alans Den' 'Becomes Alans Den'
;

Oasis: Room 'Oasis'
    regions = [Yellow]

    northeast = CountlessDunes
;

+ CobaltRibbon: Thing 'Cobalt Ribbon' 'Cobalt Ribbon'
;

ShiftingGrotto: Room 'Shifting Grotto'
    regions = [Blue]

    southeast = CrumblingChimney
    up = Cave
;

+ Becomes3: Thing 'Becomes' 'Becomes'
;

+ LindasRoom: Thing 'Lindas Room' 'Lindas Room'
;

CrumblingChimney: Room 'Crumbling Chimney'
    regions = [Blue]

    northwest = ShiftingGrotto
;

+ OliveEnvelope: Thing 'Olive Envelope' 'Olive Envelope'
;

CementRoom: Room 'Cement Room'

    north = BrickLinedRoom
    down = InfiniteIceFloes
;

BrickLinedRoom: Room 'Brick-Lined Room'

    north = SandyRoom
    south = CementRoom
;

SandyRoom: Room 'Sandy Room'

    north = WaterRoom
    south = BrickLinedRoom
;

WaterRoom: Room 'Water Room'

    north = WritingRoom
    south = SandyRoom
;

WritingRoom: Room 'Writing Room'

    north = SpinningExpanse
    south = WaterRoom
;

SpinningExpanse: Room 'Spinning Expanse'

    south = WritingRoom
;

me: Actor
    location = FoggyRoom
;

gameMain: GameMainDef
    initialPlayerChar = me
;

