#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'luster'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Forest: Region
;

LeverBuilding: Region
;

MainArea: Region
;

Water: Region
;

Fork: Room 'Fork'

    north = TrainCrossing
    south = MeanderingPath
    west = DirtPath
;

+ Returnfromplaintohere: Thing 'Return from plain to here' 'Return from plain to here'
;

TrainCrossing: Room 'Train Crossing'

    north = Field
    south = Fork
;

DirtPath: Room 'Dirt Path'

    south = SheriffStationEntrance
    east = Fork
    northeast = GatedPath
    northwest = LakeShore
;

SheriffStationEntrance: Room 'Sheriff Station Entrance'

    north = DirtPath
    south = SheriffStation
    west = PowerRoom
    in = SheriffStation
;

LakeShore: Room 'Lake Shore'

    south = LakeShore2
    southeast = DirtPath
;

LakeShore2: Room 'Lake Shore'

    north = LakeShore
    west = Dock
;

Dock: Room 'Dock'

    east = LakeShore2
    west = Lakeonraft
;

+ Raft: Thing 'Raft' 'Raft'
;

Lakeonraft: Room 'Lake (on raft)'

    east = Dock
;

+ Bluegemfallintolake: Thing 'Blue gem fall into lake' 'Blue gem fall into lake'
;

TempleEntrance: Room 'Temple Entrance'
    regions = [Water]

    west = StatueRoombecomesplain
;

+ Backtodockonceyougetgem: Thing 'Back to dock once you get gem' 'Back to dock once you get gem'
;

StatueRoombecomesplain: Room 'Statue Room (becomes plain)'
    regions = [Water]

    north = SpectatorBox
    south = Armory
    east = TempleEntrance
    west = ArenaRoom
;

ArenaRoom: Room 'Arena Room'
    regions = [Water]

    east = StatueRoombecomesplain
;

+ Swordsman: Thing 'Swordsman' 'Swordsman'
;

SpectatorBox: Room 'Spectator Box'
    regions = [Water]

    south = StatueRoombecomesplain
;

+ Ballpointpen: Thing 'Ballpoint pen' 'Ballpoint pen'
;

Armory: Room 'Armory'
    regions = [Water]

    north = StatueRoombecomesplain
;

GatedPath: Room 'Gated Path'

    north = BlankRoom
    southwest = DirtPath
;

BlankRoom: Room 'Blank Room'
    regions = [Lever Building]

    north = OceanRoom
    south = GatedPath
    east = MetallicRoom
    northeast = SpaceRoom
    northwest = RainForestRoom
;

+ Leverswitchit: Thing 'Lever switch it' 'Lever switch it'
;

OceanRoom: Room 'Ocean Room'
    regions = [Lever Building]

    south = BlankRoom
;

+ pedestal1: Thing 'pedestal1' 'pedestal1'
;

+ Bluegem: Thing 'Blue gem' 'Blue gem'
;

RainForestRoom: Room 'Rain-Forest Room'
    regions = [Lever Building]

    southeast = BlankRoom
;

+ pedestal3: Thing 'pedestal3' 'pedestal3'
;

+ Greengem: Thing 'Green gem' 'Green gem'
;

SpaceRoom: Room 'Space Room'
    regions = [Lever Building]

    southwest = BlankRoom
;

+ pedestal2: Thing 'pedestal2' 'pedestal2'
;

+ Midnightgem: Thing 'Midnight gem' 'Midnight gem'
;

MetallicRoom: Room 'Metallic Room'
    regions = [Lever Building]

    west = BlankRoom
;

+ pedestal4: Thing 'pedestal4' 'pedestal4'
;

+ Silvergem: Thing 'Silver gem' 'Silver gem'
;

MeanderingPath: Room 'Meandering Path'

    north = Fork
    south = DeadEnd
    west = RosarioEntrance
    southeast = RamenBooth
    northwest = AbandStoreFrontMyersEntrance
;

AbandStoreFrontMyersEntrance: Room 'Aband. Store Front Myers Entrance'

    west = AbandonedShop
    southeast = MeanderingPath
;

+ Matsteelkey: Thing 'Mat steel key' 'Mat steel key'
;

+ MyersEntrance: Thing 'Myers Entrance' 'Myers Entrance'
;

AbandonedShop: Room 'Abandoned Shop'

    east = AbandStoreFrontMyersEntrance
;

+ CashRegisterDime: Thing 'Cash RegisterDime' 'Cash RegisterDime'
;

RosarioEntrance: Room 'Rosario Entrance'

    east = MeanderingPath
;

DeadEnd: Room 'Dead End'

    north = MeanderingPath
    south = TimberPath
;

+ Rectangleputmat: Thing 'Rectangle put mat' 'Rectangle put mat'
;

RamenBooth: Room 'Ramen Booth'

    west = InsideRamenBooth
    northwest = MeanderingPath
;

InsideRamenBooth: Room 'Inside Ramen Booth'

    east = RamenBooth
;

+ Bulbuntiefromrope: Thing 'Bulb untie from rope' 'Bulb untie from rope'
;

+ Needsteelkey: Thing 'Need steel key' 'Need steel key'
;

TimberPath: Room 'Timber Path'
    regions = [Forest]

    north = DeadEnd
    south = ForestMaze
;

+ Note: Thing 'Note' 'Note'
;

+ forestroomnamesarerandom: Thing 'forest room names are random' 'forest room names are random'
;

+ Lightgreenindicateswrappingmaze: Thing 'Light green indicates wrapping maze' 'Light green indicates wrapping maze'
;

+ Diagonaldirectionsarepossiblebutignored: Container 'Diagonal directions are possible but ignored' 'Diagonal directions are possible but ignored'
;

++ Wejustwantawaythrough: Thing 'We just want a way through' 'We just want a way through'
;

ForestMaze: Room 'Forest Maze'
    regions = [Forest]

    north = TimberPath
    south = ForestMaze2
;

ForestMaze2: Room 'Forest Maze'
    regions = [Forest]

    north = ForestMaze
    south = ForestCenter
;

ForestCenter: Room 'Forest Center'
    regions = [Forest]

    north = ForestMaze2
    south = ForestMaze6
    east = ForestMaze8
    west = ForestMaze3
;

ForestMaze3: Room 'Forest Maze'
    regions = [Forest]

    north = ForestMaze4
    south = ForestMaze5
    east = ForestCenter
    west = ForestMaze4
;

ForestMaze4: Room 'Forest Maze'
    regions = [Forest]

    south = ForestMaze9
    east = ForestMaze3
    west = ForestCenter2
;

ForestCenter2: Room '(Forest Center)'
    regions = [Forest]

    east = ForestMaze4
;

ForestMaze5: Room 'Forest Maze'
    regions = [Forest]

    north = ForestMaze3
    south = ForestMaze7
;

ForestMaze6: Room 'Forest Maze'
    regions = [Forest]

    north = ForestCenter
    east = Clearing
;

Clearing: Room 'Clearing'
    regions = [Forest]

    west = ForestMaze6
;

+ GreenGem2: Thing 'Green Gem' 'Green Gem'
;

ForestMaze7: Room 'Forest Maze'
    regions = [Forest]

    north = ForestMaze5
    south = EndofCave
;

EndofCave: Room 'End of Cave'
    regions = [Forest]

    north = ForestMaze7
;

+ MidnightGemneedbulb: Thing 'Midnight Gem need bulb' 'Midnight Gem need bulb'
;

ForestMaze8: Room '(Forest Maze)'
    regions = [Forest]

    west = ForestCenter
;

SheriffStation: Room 'Sheriff Station'

    north = SheriffStationEntrance
    west = CrampedBathroom
;

+ Needcopperkeytoexit: Thing 'Need copper key to exit' 'Need copper key to exit'
;

CrampedBathroom: Room 'Cramped Bathroom'

    east = SheriffStation
;

+ soapwashsoap: Thing 'soap wash soap' 'soap wash soap'
;

Field: Room 'Field'

    south = TrainCrossing
;

+ Cubeputcoininslot: Thing 'Cube put coin in slot' 'Cube put coin in slot'
;

+ Makessilvergem: Thing 'Makes silver gem' 'Makes silver gem'
;

PowerRoom: Room 'Power Room'

    east = SheriffStationEntrance
;

+ switchbuttonsRBI: Thing 'switch buttons RBI' 'switch buttons RBI'
;

ForestMaze9: Room 'Forest Maze'
    regions = [Forest]

    north = ForestMaze4
    east = ForestMaze4
;

TreeCanopy: Room 'Tree Canopy'
    regions = [Forest]
;

+ Fromtoclearing: Thing 'Fromto clearing' 'Fromto clearing'
;

me: Actor
    location = Fork
;

gameMain: GameMainDef
    initialPlayerChar = me
;

