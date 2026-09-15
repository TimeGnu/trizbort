#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'space-quest-coral-maze'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Importantitem: Region
;

Later: Region
;

Lethal: Region
;

ThroughWay: Region
;

Useless: Region
;

MazeStart: Room 'Maze Start'
    regions = [Through Way]

    south = Tright
;

Tright: Room 'T right'
    regions = [Through Way]

    north = MazeStart
    south = UpDown
    east = LeftRight
;

LeftRight: Room 'Left Right'
    regions = [Through Way]

    east = Tdown
    west = Tright
;

UpDown: Room 'Up Down'
    regions = [Useless]

    north = Tright
    south = Tleft
;

Tleft: Room 'T left'
    regions = [Useless]

    north = UpDown
    west = DeadEnd
;

DeadEnd: Room 'Dead End'
    regions = [Useless]

    east = Tleft
;

Tdown: Room 'T down'
    regions = [Through Way]

    south = Updown2
    west = LeftRight
;

Updown2: Room 'Up down'
    regions = [Through Way]

    north = Tdown
    south = UpRight
;

UpRight: Room 'Up Right'
    regions = [Through Way]

    north = Updown2
    east = LeftRight2
;

LeftRight2: Room 'Left Right'
    regions = [Through Way]

    east = LeftRight3
    west = UpRight
;

LeftRight3: Room 'Left Right'
    regions = [Through Way]

    east = Tdown2
    west = LeftRight2
;

Tdown2: Room 'T down'
    regions = [Through Way]

    south = Tright2
    west = LeftRight3
;

Tright2: Room 'T right'
    regions = [Through Way]

    north = Tdown2
    south = CrossRoads
;

CrossRoads: Room 'Cross Roads'
    regions = [Through Way]

    north = Tright2
    south = UpDown3
;

UpDown3: Room 'Up Down'
    regions = [Through Way]

    north = CrossRoads
    west = UpLeft
;

UpLeft: Room 'Up Left'
    regions = [Through Way]

    east = UpDown3
    west = Tdown3
;

Tdown3: Room 'T down'
    regions = [Through Way]

    south = Tleft2
    east = UpLeft
;

Tleft2: Room 'T left'
    regions = [Through Way]

    north = Tdown3
    south = Upright2
;

Upright2: Room 'Up right'
    regions = [Through Way]

    north = Tleft2
    east = Panel
;

Panel: Room 'Panel'
    regions = [Through Way]

    east = GreenButton
    west = Upright2
;

+ Needalienpaper: Thing 'Need alien paper' 'Need alien paper'
;

+ Frombehindshark: Thing 'From behind shark' 'From behind shark'
;

GreenButton: Room 'Green Button'
    regions = [Through Way]

    south = Hall
    east = Nicescene
    west = Panel
;

+ Usepanel3: Thing 'Use panel  3' 'Use panel  3'
;

Nicescene: Room 'Nice scene'
    regions = [Useless]

    west = GreenButton
;

Hall: Room 'Hall'
    regions = [Through Way]

    north = GreenButton
    south = Hall2
;

Hall2: Room 'Hall'
    regions = [Through Way]

    north = Hall
    east = Ship
;

Ship: Room 'Ship'
    regions = [Through Way]

    west = Hall2
;

CraftStart: Room 'Craft Start'
    regions = [Through Way]

    west = R00
;

R00: Room 'R00'
    regions = [Through Way]

    east = CraftStart
    west = R002
;

R002: Room 'R00'
    regions = [Through Way]

    east = R00
    west = R003
;

R003: Room 'R00'
    regions = [Through Way]

    east = R002
    west = OtherShip
;

OtherShip: Room 'Other Ship'
    regions = [Important item]

    east = R003
    west = R004
;

+ Box: Thing 'Box' 'Box'
;

+ Needpressuresuit: Thing 'Need pressure suit' 'Need pressure suit'
;

R004: Room 'R00'
    regions = [Through Way]

    north = CityEntry
    east = OtherShip
;

CityEntry: Room 'City Entry'
    regions = [Through Way]

    south = R004
    northwest = DockBay
;

+ USEBOX: Thing 'USE BOX' 'USE BOX'
;

+ EXITCITYtoleave: Thing 'EXIT CITY to leave' 'EXIT CITY to leave'
;

DockBay: Room 'Dock Bay'
    regions = [Through Way]

    east = BigDoor
    southeast = CityEntry
;

BigDoor: Room 'Big Door'
    regions = [Through Way]

    east = Hall3
    west = DockBay
;

+ USEPANEL: Thing 'USE PANEL' 'USE PANEL'
;

Hall3: Room 'Hall'
    regions = [Through Way]

    south = BigDoor
    east = Hall4
    west = Aliens
;

Aliens: Room 'Aliens'
    regions = [Lethal]

    east = Hall3
;

Hall4: Room 'Hall'
    regions = [Through Way]

    south = Hall5
    west = Hall3
;

Hall5: Room 'Hall'
    regions = [Through Way]

    north = Hall4
    east = StayNorth
;

StayNorth: Room 'Stay North'
    regions = [Through Way]

    east = Hall6
    west = Hall5
;

Hall6: Room 'Hall'
    regions = [Through Way]

    north = Hall7
    east = Aliens2
    west = StayNorth
;

Aliens2: Room 'Aliens'
    regions = [Lethal]

    west = Hall6
;

+ Extrabad: Thing 'Extra bad' 'Extra bad'
;

+ Theyllfollow: Thing 'Theyll follow' 'Theyll follow'
;

Hall7: Room 'Hall'
    regions = [Through Way]

    north = Hall8
    south = Hall6
;

Hall8: Room 'Hall'
    regions = [Through Way]

    south = Hall7
    east = Hall9
;

Hall9: Room 'Hall'
    regions = [Through Way]

    east = Museum
    west = Hall8
;

Museum: Room 'Mus-eum'
    regions = [Important item]

    west = Hall9
;

+ MASK: Thing 'MASK' 'MASK'
;

+ OUTFIT: Thing 'OUTFIT' 'OUTFIT'
;

Item: Room 'Item'
    regions = [Important item]
;

Useless2: Room 'Use-less'
    regions = [Useless]
;

Death: Room 'Death'
    regions = [Lethal]
;

WalkThru: Room 'Walk Thru'
    regions = [Through Way]
;

me: Actor
    location = MazeStart
;

gameMain: GameMainDef
    initialPlayerChar = me
;

