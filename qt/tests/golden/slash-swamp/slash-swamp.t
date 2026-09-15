#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'Slasher Swamp map'
    byline = 'By Andrew Schultz'
    version = '1'
    desc = 'This is a map for Slasher Swamp, an IFComp 2014 entry. The dotted lines indicate places you need to search, and in what order.'
;

Forest: Region
;

Road: Room 'Road'

    south = MuddySwamp
;

+ TRUCK1: Thing 'TRUCK 1' 'TRUCK 1'
;

Pond: Room 'Pond'

    east = SwampForest
;

MuddySwamp: Room 'Muddy Swamp'

    north = Road
    south = RottingWoodenWalkway
    east = ConcreteBuilding
    west = MangroveForest
;

EmptyFloridaHighway: Room 'Empty Florida Highway'

    north = SwampForest
    south = ConcreteBuilding
    east = GravelRoad
;

BlacktopHighway: Room 'Blacktop Highway'

    south = MangroveForest
    west = CreepyHighway
;

BambooForest: Room 'Bamboo Forest'

    north = DarkGrove
    west = DryEverglades
;

SwampForest: Room 'Swamp Forest'

    north = PineForest
    south = EmptyFloridaHighway
    east = OakSwamp
    west = Pond
;

+ BRIARPATCH5: Thing 'BRIAR PATCH 5' 'BRIAR PATCH 5'
;

DryEverglades: Room 'Dry Everglades*'

    north = FrontPorch
    south = CreepyHighway
    east = BambooForest
    west = PalmettoFlat
;

DarkGrove: Room 'Dark Grove*'

    north = SouthBankofaBayou
    south = BambooForest
;

+ Etoswampforest: Thing 'E to swamp forest' 'E to swamp forest'
;

CreepyHighway: Room 'Creepy Highway'

    north = DryEverglades
    south = ScrubForest
    east = BlacktopHighway
    west = DesolateHighway
;

MangroveForest: Room 'Mangrove Forest'

    north = BlacktopHighway
    east = MuddySwamp
    west = ScrubForest
;

DesolateHighway: Room 'Desolate Highway'

    north = PalmettoFlat
    south = PineFlatwood
    east = CreepyHighway
;

ScrubForest: Room 'Scrub Forest'

    north = CreepyHighway
    east = MangroveForest
    west = PineFlatwood
;

PineFlatwood: Room 'Pine Flatwood*'

    north = DesolateHighway
    east = ScrubForest
;

+ WStoswamp: Thing 'WS to swamp' 'WS to swamp'
;

RottingWoodenWalkway: Room 'Rotting Wooden Walkway'

    north = MuddySwamp
    south = SandyGrass
;

ConcreteBuilding: Room 'Concrete Building'

    north = EmptyFloridaHighway
    south = DarkConcreteBuilding
    east = ConcreteSlab
    west = MuddySwamp
;

SandyGrass: Room 'Sandy Grass'

    north = RottingWoodenWalkway
    south = DustyClearing
    east = DustyClearing
    west = Gazebo
;

+ KingofSpades: Thing 'King of Spades' 'King of Spades'
;

Gazebo: Room 'Gazebo'

    east = SandyGrass
;

+ QueenofClubs: Thing 'Queen of Clubs' 'Queen of Clubs'
;

DustyClearing: Room 'Dusty Clearing'

    north = SandyGrass
    east = Gate
;

Gate: Room 'Gate'

    east = Garden
    west = DustyClearing
;

Garden: Room 'Garden'

    north = Garden2
    east = ConcreteRuins
    west = Gate
;

ConcreteRuins: Room 'Concrete Ruins'

    north = RoseBush
    west = Garden
;

+ JackofSpades: Thing 'Jack of Spades' 'Jack of Spades'
;

RoseBush: Room 'Rose Bush'

    south = ConcreteRuins
    west = Garden2
;

Garden2: Room 'Garden'

    south = Garden
    east = RoseBush
;

DarkConcreteBuilding: DarkRoom 'Dark Concrete Building'

    north = ConcreteBuilding
;

+ Deerdollhead: Thing 'Deer doll head' 'Deer doll head'
;

ConcreteSlab: Room 'Concrete Slab'

    north = GravelRoad
    south = Graveyard
    east = BushThicket
    west = ConcreteBuilding
;

GravelRoad: Room 'Gravel Road'

    north = OakSwamp
    south = BushThicket
    east = GasStationParkingLot
    west = EmptyFloridaHighway
;

BushThicket: Room 'Bush Thicket'

    north = GravelRoad
    east = Meadow
    west = ConcreteSlab
;

+ StoSwampForest: Thing 'S to Swamp Forest' 'S to Swamp Forest'
;

OakSwamp: Room 'Oak Swamp'

    north = SwampClearingEtograssfield
    south = GravelRoad
    west = SwampForest
;

+ EtoSwampForest2: Thing 'E to Swamp Forest' 'E to Swamp Forest'
;

PalmettoFlat: Room 'Palmetto Flat'

    north = DustyHill
    south = DesolateHighway
    east = DryEverglades
;

+ WtoSwampForest: Thing 'W to Swamp Forest' 'W to Swamp Forest'
;

DustyHill: Room 'Dusty Hill*'

    north = SouthBankofaBayou
    south = PalmettoFlat
;

FrontPorch: Room 'Front Porch'

    north = LivingRoom
    south = DryEverglades
;

LivingRoom: DarkRoom 'Living Room'

    north = Kitchen
    south = FrontPorch
;

Kitchen: Room 'Kitchen'

    north = SouthBankofaBayou
    south = LivingRoom
;

+ SINK2: Thing 'SINK 2' 'SINK 2'
;

SouthBankofaBayou: Room 'South Bank of a Bayou*'

    south = Kitchen
    east = DarkGrove
    west = DustyHill
;

Meadow: Room 'Meadow*'

    north = GravelRoad
    south = PalmettoClearing
;

SwampClearingEtograssfield: Room 'Swamp Clearing (E to grass field)'

    south = OakSwamp
;

+ NWtoSwampForest: Thing 'NW to Swamp Forest' 'NW to Swamp Forest'
;

PalmettoClearing: Room 'Palmetto Clearing'

    north = Meadow
    south = PalmettoField
;

+ WEtoSwampForest: Thing 'WE to Swamp Forest' 'WE to Swamp Forest'
;

PalmettoField: Room 'Palmetto Field'

    north = PalmettoClearing
    south = InFrontofaStrawHut
;

+ WEtoSwampForest2: Thing 'WE to Swamp Forest' 'WE to Swamp Forest'
;

InFrontofaStrawHut: Room 'In Front of a Straw Hut'

    north = PalmettoField
    south = StrawHut
;

StrawHut: Room 'Straw Hut'

    north = InFrontofaStrawHut
;

+ SHACK11: Thing 'SHACK11' 'SHACK11'
;

+ Needseveredheadandrevolvernotmanwrevolver: Thing 'Need severed head and revolver not man wrevolver' 'Need severed head and revolver not man wrevolver'
;

PineForest: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest2
    south = SwampForest
    east = PineForest6
;

PineForest2: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest3
    south = PineForest
    east = PineForest5
;

PineForest3: Room 'Pine Forest'
    regions = [Forest]

    south = PineForest2
    east = PineForest4
;

PineForest4: Room 'Pine Forest'
    regions = [Forest]

    south = PineForest5
    east = PineForest9
    west = PineForest3
;

PineForest5: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest4
    south = PineForest6
    east = PineForest8
    west = PineForest2
;

PineForest6: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest5
    east = PineForest7
    west = PineForest
;

PineForest7: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest8
    west = PineForest6
;

PineForest8: Room 'Pine Forest'
    regions = [Forest]

    north = PineForest9
    south = PineForest7
    west = PineForest5
;

PineForest9: Room 'Pine Forest'
    regions = [Forest]

    south = PineForest8
    east = InFrontofShack
    west = PineForest4
;

InFrontofShack: Room 'In Front of Shack'

    north = InShack
    west = PineForest9
;

InShack: Room 'In Shack'

    north = Altar
    south = InFrontofShack
;

Altar: Room 'Altar'

    south = InShack
    west = Shrine
;

+ ALTAR6BODY7: Thing 'ALTAR 6 BODY 7' 'ALTAR 6 BODY 7'
;

Graveyard: Room 'Graveyard'

    north = ConcreteSlab
;

+ GRAVE9needshovel: Thing 'GRAVE 9 need shovel' 'GRAVE 9 need shovel'
;

+ COFFINwironkeyMAN10: Thing 'COFFIN wiron key MAN10' 'COFFIN wiron key MAN10'
;

GasStationParkingLot: Room 'Gas Station Parking Lot'

    north = AlleyNorthofGasStation
    south = SouthoftheGasStation
    east = InsideGasStation
    west = GravelRoad
;

AlleyNorthofGasStation: Room 'Alley North of Gas Station'

    east = GasStationDumpster
    west = GasStationParkingLot
;

SouthoftheGasStation: Room 'South of the Gas Station'

    east = GasStationDumpster
    west = GasStationParkingLot
;

GasStationDumpster: Room 'Gas Station Dumpster'

    north = AlleyNorthofGasStation
    south = SouthoftheGasStation
;

+ DUMPSTER3: Thing 'DUMPSTER 3' 'DUMPSTER 3'
;

+ ESwampForest: Thing 'E Swamp Forest' 'E Swamp Forest'
;

InsideGasStation: Room 'Inside Gas Station'

    north = GasStationOffice
    west = GasStationParkingLot
;

+ GARBAGE: Thing 'GARBAGE' 'GARBAGE'
;

GasStationOffice: DarkRoom 'Gas Station Office'

    south = InsideGasStation
;

+ BLANKET4: Thing 'BLANKET 4' 'BLANKET 4'
;

Shrine: DarkRoom 'Shrine'

    east = Altar
;

+ Headneededtowin: Thing 'Head needed to win' 'Head needed to win'
;

GrassField: Room 'Grass Field'

    north = CloverField
    south = SunflowerField
    west = FlowerField
;

+ Wtoflower: Thing 'W to flower' 'W to flower'
;

CloverField: Room 'Clover Field'

    south = MuckField
    east = FlowerField
    west = GrassField
;

+ NtoSwamp: Thing 'N to Swamp' 'N to Swamp'
;

MuckField: Room 'Muck Field'

    north = InFrontofanOuthouse
    south = FlowerField
    east = MuckField
    west = CloverField
;

+ Etograss: Thing 'E to grass' 'E to grass'
;

SunflowerField: Room 'Sunflower Field'

    north = GrassField
    south = FlowerField
    east = FlowerField
    west = SunflowerField
;

FlowerField: Room 'Flower Field'

    south = CloverField
    east = MuckField
    west = SunflowerField
;

+ Wtograss: Thing 'W to grass' 'W to grass'
;

InFrontofanOuthouse: Room 'In Front of an Outhouse'

    north = Outhouse
    south = MuckField
;

Outhouse: Room 'Outhouse'

    south = InFrontofanOuthouse
;

+ HOLE8: Thing 'HOLE 8' 'HOLE 8'
;

Swamp1fromvariouslocations: Room 'Swamp-1 (from various locations)'

    south = Swamp2fromvariouslocations
;

+ Northto: Thing 'North to' 'North to'
;

+ DesolateHighway2: Thing 'Desolate Highway' 'Desolate Highway'
;

Swamp2fromvariouslocations: Room 'Swamp-2 (from various locations)'

    south = Swamp3
;

+ Alwaysto: Thing 'Always to' 'Always to'
;

+ Swamp32: Thing 'Swamp-3' 'Swamp-3'
;

Swamp3: Room 'Swamp-3'

    south = Pond
;

+ SouthtoPondOtherwisetoSwamp2: Thing 'South to Pond Otherwise to Swamp-2' 'South to Pond Otherwise to Swamp-2'
;

me: Actor
    location = Road
;

gameMain: GameMainDef
    initialPlayerChar = me
;

