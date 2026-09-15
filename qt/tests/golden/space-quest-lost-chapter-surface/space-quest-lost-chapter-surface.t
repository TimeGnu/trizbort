#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'space-quest-lost-chapter-surface'
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

RockTree: Room 'Rock Tree'
    regions = [Later]

    north = RockWithVines
    south = EndofPath
;

RockWithVines: Room 'Rock With Vines'
    regions = [Later]

    south = RockTree
    east = RockPath
;

RockPath: Room 'Rock Path'
    regions = [Later]

    east = RockBridge
    west = RockWithVines
;

RockBridge: Room 'Rock Bridge'
    regions = [Later]

    east = NWRockPath2
    west = RockPath
;

NWRockPath2: Room 'N/W Rock Path 2'
    regions = [Later]

    east = NWRockPath
    west = RockBridge
;

+ Becarefulcanfall: Thing 'Be careful can fall' 'Be careful can fall'
;

NWRockPath: Room 'N/W Rock Path'
    regions = [Later]

    south = Shore
    west = NWRockPath2
;

Shore: Room 'Shore'
    regions = [Later]

    north = NWRockPath
    east = ShipFront
    west = Waterfall
;

Waterfall: Room 'Water fall'
    regions = [Useless]

    east = Shore
;

ShipFront: Room 'Ship Front'
    regions = [Through Way]

    south = Monsterchasesyou2
    east = ShipBody
    west = Shore
;

ShipBody: Room 'Ship Body'
    regions = [Through Way]

    south = Monsterchasesyou2
    east = ShipTail
    west = ShipFront
;

ShipTail: Room 'Ship Tail'
    regions = [Important item]

    south = Monsterchasesyou2
    east = Lake
    west = ShipBody
;

+ NAIL: Thing 'NAIL' 'NAIL'
;

Lake: Room 'Lake'
    regions = [Through Way]

    east = LagoonCLIMB
    west = ShipTail
;

LagoonCLIMB: Room 'Lagoon CLIMB'
    regions = [Through Way]

    north = Tree
    east = WaterWtolagoon
    west = Lake
;

+ GoSWWtonextlake: Thing 'Go SWW to next lake' 'Go SWW to next lake'
;

Tree: Room 'Tree'
    regions = [Through Way]

    south = LagoonCLIMB
;

Tree2: Room 'Tree'
    regions = [Through Way]

    east = FRUIT
;

FRUIT: Room 'FRUIT'
    regions = [Important item]

    west = Tree2
;

WaterWtolagoon: Room 'Water W to lagoon'
    regions = [Through Way]

    south = Monsterchasesyou
    east = LakeonS
    west = LagoonCLIMB
;

LakeonS: Room 'Lake on S'
    regions = [Through Way]

    south = Monsterchasesyou
    east = LakeinSW
    west = WaterWtolagoon
;

LakeinSW: Room 'Lake in SW'
    regions = [Through Way]

    north = jungle
    south = DeathinS
    east = GreyRock
    west = LakeonS
;

Monsterchasesyou: Room 'Monster chases you'
    regions = [Lethal]

    north = LakeonS
    east = DeathinS
;

jungle: Room 'jungle'
    regions = [Through Way]

    south = LakeinSW
    east = MOULDN
    west = LakeinSW
;

MOULDN: Room 'MOULD (N)'
    regions = [Important item]

    east = FlowerN
    west = jungle
;

FlowerN: Room 'Flower (N)'
    regions = [Important item]

    south = GreyTree
    east = jungle2
    west = MOULDN
;

+ Mould: Thing 'Mould' 'Mould'
;

jungle2: Room 'jungle'
    regions = [Through Way]

    south = jungle3
    west = FlowerN
;

jungle3: Room 'jungle'
    regions = [Through Way]

    north = jungle2
    south = jungle5
    east = YourShip
    west = GreyTree
;

YourShip: Room 'Your Ship'
    regions = [Through Way]

    south = jungle3
    east = jungle4
;

jungle4: Room 'jungle'
    regions = [Useless]

    north = YourShip
    west = jungle5
;

jungle5: Room 'jungle'
    regions = [Through Way]

    north = jungle3
    south = CODECARD
    east = jungle4
    west = BlackTree
;

BlackTree: Room 'Black Tree'
    regions = [Useless]

    north = GreyTree
    east = jungle5
;

GreyTree: Room 'Grey Tree'
    regions = [Useless]

    north = FlowerN
    south = BlackTree
    east = jungle3
    west = GreyRock
;

GreyRock: Room 'Grey Rock'
    regions = [Useless]

    east = GreyTree
    west = LakeinSW
;

CODECARD: Room 'CODE CARD'
    regions = [Important item]

    north = jungle5
;

DeathinS: Room 'Death in S'
    regions = [Lethal]

    north = LakeinSW
    west = Monsterchasesyou
;

EndofPath: Room 'End of Path'
    regions = [Later]

    north = RockTree
    up = Mountain
;

Mountain: Room 'Mountain'
    regions = [Later]

    north = Mountain2
    down = EndofPath
;

Mountain2: Room 'Mountain'
    regions = [Later]

    north = CAVE
    south = Mountain
;

CAVE: Room 'CAVE'
    regions = [Important item]

    south = Mountain2
;

+ GETSTONEWITHBONE: Thing 'GET STONE WITH BONE' 'GET STONE WITH BONE'
;

Undershipbody: Room 'Under ship body'
    regions = [Through Way]

    south = BAG
    east = Undershiptail
;

BAG: Room 'BAG'
    regions = [Important item]

    north = Undershipbody
;

Undershiptail: Room 'Under ship tail'
    regions = [Through Way]

    south = WallWEopen
    east = Scenic
    west = Undershipbody
;

Scenic: Room 'Scenic'
    regions = [Through Way]

    south = TreeRootNE
    east = Scenic2
    west = Undershiptail
;

Scenic2: Room 'Scenic'
    regions = [Through Way]

    south = TreeRootNW
    east = Scenic3
    west = Scenic
;

Scenic3: Room 'Scenic'
    regions = [Through Way]

    south = BigVineW
    east = HiddenPsgWest
    west = Scenic2
;

HiddenPsgWest: Room 'Hidden Psg West'
    regions = [Through Way]

    south = WallE
    east = HiddenPsgEast
    west = Scenic3
;

HiddenPsgEast: Room 'Hidden Psg East'
    regions = [Through Way]

    south = BlackWallWest
    east = Scenic4
    west = HiddenPsgWest
;

Scenic4: Room 'Scenic'
    regions = [Through Way]

    south = SeaWeed
    east = ForkNorth
    west = HiddenPsgEast
;

ForkNorth: Room 'Fork North'
    regions = [Through Way]

    north = TENTACLE2
    south = BrownTree
    east = TinyTooth
    west = Scenic4
;

TinyTooth: Room 'Tiny Tooth'
    regions = [Lethal]

    south = GelBlob
    east = Turtle
    west = ForkNorth
;

Turtle: Room 'Turtle'
    regions = [Through Way]

    east = JellyFish
    west = TinyTooth
;

JellyFish: Room 'Jelly Fish'
    regions = [Through Way]

    east = METALROD
    west = Turtle
;

METALROD: Room 'METAL ROD'
    regions = [Important item]

    east = Cavern
    west = JellyFish
;

Cavern: Room 'Cavern'
    regions = [Through Way]

    east = Cavern2
    west = METALROD
;

Cavern2: Room 'Cavern'
    regions = [Through Way]

    north = Pool
    west = Cavern
;

Pool: Room 'Pool'
    regions = [Through Way]

    south = Cavern2
    west = PuffyFish
;

PuffyFish: Room 'Puffy Fish'
    regions = [Important item]

    east = Pool
;

GelBlob: Room 'Gel Blob'
    regions = [Lethal]

    north = TinyTooth
    south = Clams
    west = BrownTree
;

BrownTree: Room 'Brown Tree'
    regions = [Through Way]

    north = ForkNorth
    south = Clams
    east = GelBlob
    west = SeaWeed
;

Clams: Room 'Clams'
    regions = [Lethal]

    north = BrownTree
    south = TrunkAlgae
    west = Clams2
;

Clams2: Room 'Clams'
    regions = [Lethal]

    north = SeaWeed
    south = TREEE
    east = Clams
    west = Clams3
;

Clams3: Room 'Clams'
    regions = [Lethal]

    north = BlackWallWest
    east = Clams2
;

BlackWallWest: Room 'Black Wall West'
    regions = [Useless]

    north = HiddenPsgEast
    south = Clams3
    east = SeaWeed
;

SeaWeed: Room 'Sea Weed'
    regions = [Through Way]

    north = Scenic4
    south = Clams2
    east = BrownTree
    west = BlackWallWest
;

TREEE: Room 'TREE (E)'
    regions = [Through Way]

    north = Clams2
    south = Clams5
    east = TrunkAlgae
    west = TREESearch
;

TrunkAlgae: Room 'Trunk/ Algae'
    regions = [Through Way]

    north = Clams
    south = Clams5
    east = Structures
    west = TREEE
;

Structures: Room 'Struc tures'
    regions = [Through Way]

    north = Clams6
    east = OCTOPUS
    west = TrunkAlgae
;

OCTOPUS: Room 'OCTO PUS'
    regions = [Important item]

    west = Structures
;

+ BONE: Thing 'BONE' 'BONE'
;

TREESearch: Room 'TREE (Search)'
    regions = [Important item]

    south = Clams5
    east = TREEE
    west = WallEAlgae
;

+ SEARCHHOLE: Thing 'SEARCH HOLE' 'SEARCH HOLE'
;

WallEAlgae: Room 'Wall E + Algae'
    regions = [Through Way]

    north = TreeWWall
    south = Eels
    east = TREESearch
    west = CoralPULL
;

+ Stoalgae: Thing 'S to algae' 'S to algae'
;

CoralPULL: Room 'Coral (PULL)'
    regions = [Through Way]

    north = NWtotreasure
    south = Eels
    east = WallEAlgae
    west = VineandTrunk
;

VineandTrunk: Room 'Vine and Trunk'
    regions = [Through Way]

    north = TrunkwNEHole
    south = Eels
    east = CoralPULL
    west = VineinEast
;

VineinEast: Room 'Vine in East'
    regions = [Through Way]

    north = 2rootedges
    south = Clams4
    east = VineandTrunk
    west = Turtle2
;

Turtle2: Room 'Turtle'
    regions = [Through Way]

    north = WallWETrunk
    south = Clams4
    east = VineinEast
;

Clams4: Room 'Clams'
    regions = [Lethal]

    north = Turtle2
;

Clams5: Room 'Clams'
    regions = [Lethal]

    north = TREEE
;

WallWETrunk: Room 'Wall W, E Trunk'
    regions = [Through Way]

    north = WallWEopen
    south = Turtle2
    east = 2rootedges
;

WallWEopen: Room 'Wall W, E open'
    regions = [Through Way]

    north = Undershiptail
    south = WallWETrunk
    east = TreeRootNE
;

TreeRootNE: Room 'Tree Root NE'
    regions = [Useless]

    north = Scenic
    south = 2rootedges
    east = TreeRootNW
    west = WallWEopen
;

TreeRootNW: Room 'Tree Root NW'
    regions = [Useless]

    north = Scenic2
    south = TrunkwNEHole
    east = BigVineW
    west = TreeRootNE
;

BigVineW: Room 'Big Vine W'
    regions = [Useless]

    north = Scenic3
    south = NWtotreasure
    east = WallE
    west = TreeRootNW
;

WallE: Room 'Wall E'
    regions = [Useless]

    north = HiddenPsgWest
    south = TreeWWall
    west = BigVineW
;

2rootedges: Room '2 root edges'
    regions = [Useless]

    north = TreeRootNE
    south = VineinEast
    east = TrunkwNEHole
    west = WallWETrunk
;

TrunkwNEHole: Room 'Trunk w/NE Hole'
    regions = [Useless]

    north = TreeRootNW
    south = VineandTrunk
    east = NWtotreasure
    west = 2rootedges
;

NWtotreasure: Room 'NW to treas ure'
    regions = [Through Way]

    north = BigVineW
    south = CoralPULL
    east = TreeWWall
    west = TrunkwNEHole
;

TreeWWall: Room 'Tree W + Wall'
    regions = [Through Way]

    north = WallE
    south = WallEAlgae
    west = NWtotreasure
;

WaterCave: Room 'Water Cave'
    regions = [Through Way]

    south = WaterCave3
    east = WaterCave2
;

WaterCave2: Room 'Water Cave'
    regions = [Through Way]

    east = updownpsgs
    west = WaterCave
;

WaterCave3: Room 'Water Cave'
    regions = [Through Way]

    north = WaterCave
    east = WaterCave4
;

WaterCave4: Room 'Water Cave'
    regions = [Through Way]

    east = Shipwcard
    west = WaterCave3
;

Shipwcard: Room 'Ship w/card'
    regions = [Important item]

    west = WaterCave4
;

updownpsgs: Room 'up/down psgs'
    regions = [Through Way]

    east = UpperPsg
    west = WaterCave2
;

TenTacle: Room 'Ten Tacle'
    regions = [Lethal]

    east = updownpsgs
;

HidPsgEast: Room 'Hid Psg East'
    regions = [Through Way]

    east = Oracle
    west = TenTacle
;

Oracle: Room 'Oracle'
    regions = [Important item]

    west = HidPsgEast
;

+ TouchExamine: Thing 'TouchExamine' 'TouchExamine'
;

Shark: Room 'Shark'
    regions = [Lethal]

    east = LowerPsg
;

+ Usefruit: Thing 'Use fruit' 'Use fruit'
;

+ Getpaper: Thing 'Get paper' 'Get paper'
;

UpperPsg: Room 'Upper Psg'
    regions = [Through Way]

    east = UpperPsg2
    west = updownpsgs
;

UpperPsg2: Room 'Upper Psg'
    regions = [Through Way]

    east = UDHole
    west = UpperPsg
;

UDHole: Room 'U/D Hole'
    regions = [Through Way]

    west = UpperPsg2
;

Monsterchasesyou2: Room 'Monster chases you'
    regions = [Lethal]

    north = ShipBody
;

TENTACLE2: Room 'TENT ACLE'
    regions = [Lethal]

    east = ForkNorth
    west = updownpsgs
;

FromNofCoral: Room 'From N of Coral'
    regions = [Important item]
;

+ LineJar: Thing 'Line Jar' 'Line Jar'
;

Clams6: Room 'Clams'
    regions = [Lethal]
;

Eels: Room 'Eels'
    regions = [Lethal]

    north = CoralPULL
;

LowerPsg: Room 'Lower Psg'
    regions = [Through Way]

    east = LowerPsg2
    west = Shark
;

LowerPsg2: Room 'Lower Psg'
    regions = [Through Way]

    east = UDHole
    west = LowerPsg
;

me: Actor
    location = RockTree
;

gameMain: GameMainDef
    initialPlayerChar = me
;

