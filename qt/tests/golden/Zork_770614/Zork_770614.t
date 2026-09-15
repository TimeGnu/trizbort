#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'Zork_770614'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

WestofHouse: Room 'West of House'
    regions = []

    north = NorthofHouse
    south = SouthofHouse
    west = Forest2
;

SouthofHouse: Room 'South of House'
    regions = []

    south = Forest1
    east = BehindHouse
    west = WestofHouse
;

BehindHouse: Room 'Behind House'
    regions = []

    north = NorthofHouse
    south = SouthofHouse
    east = Clearing
    west = Kitchen
;

+ Smallwindow: Thing 'Small window' 'Small window'
;

NorthofHouse: Room 'North of House'
    regions = []

    north = Forest3
    east = BehindHouse
    west = WestofHouse
;

Clearing: Room 'Clearing'
    regions = []

    south = Forest1
    west = Forest3
    southwest = BehindHouse
;

+ Pileofleaves: Thing 'Pile of leaves' 'Pile of leaves'
;

Kitchen: Room 'Kitchen'
    regions = []

    east = BehindHouse
    west = LivingRoom
    up = Attic
;

+ Bottle: Thing 'Bottle' 'Bottle'
;

+ Sack: Thing 'Sack' 'Sack'
;

LivingRoom: Room 'Living Room'
    regions = []

    east = Kitchen
;

+ Tropgycase: Thing 'Tropgy case' 'Tropgy case'
;

+ Lantern: Thing 'Lantern' 'Lantern'
;

+ Rug: Thing 'Rug' 'Rug'
;

Attic: Room 'Attic'
    regions = []

    down = Kitchen
;

+ Rope: Thing 'Rope' 'Rope'
;

+ Knife: Thing 'Knife' 'Knife'
;

Forest1: Room 'Forest 1'
    regions = []

    north = SouthofHouse
    east = Clearing
    west = Forest2
;

Forest2: Room 'Forest 2'
    regions = []

    east = Forest3
;

Forest3: Room 'Forest 3'
    regions = []

    south = Clearing
    east = Clearing
    west = NorthofHouse
;

Cellar: Room 'Cellar'
    regions = []

    south = WestofChasm
    east = TrollRoom
;

WestofChasm: Room 'West of Chasm'
    regions = []

    north = NorthSouthCrawlway
    south = Gallery
    west = Cellar
;

Gallery: Room 'Gallery'
    regions = []

    north = WestofChasm
    south = Studio
;

+ Painting: Thing 'Painting' 'Painting'
;

Studio: Room 'Studio'
    regions = []

    north = NorthSouthCrawlway
    northwest = Gallery
;

+ Chimney: Thing 'Chimney' 'Chimney'
;

NorthSouthCrawlway: Room 'North-South Crawlway'
    regions = []

    north = WestofChasm
    south = Studio
    east = TrollRoom
;

TrollRoom: Room 'Troll Room'
    regions = []

    north = EastWestPassage
    east = NorthSouthCrawlway
    west = Cellar
;

+ Troll: Thing 'Troll' 'Troll'
;

EastWestPassage: Room 'East-West Passage'
    regions = []

    north = DeepRavine
    east = RoundRoom
    west = TrollRoom
;

CyclopsRoom: Room 'Cyclops Room'
    regions = []

    up = TreasureRoom
;

+ Cyclop: Thing 'Cyclop' 'Cyclop'
;

StrangePassage: Room 'Strange Passage'
    regions = []
;

TreasureRoom: Room 'Treasure Room'
    regions = []

    down = CyclopsRoom
;

+ Thief: Thing 'Thief' 'Thief'
;

+ Chalice: Thing 'Chalice' 'Chalice'
;

DomeRoom: Room 'Dome Room'
    regions = []

    east = RockyCrawl
    down = TorchRoom
;

+ Railing: Thing 'Railing' 'Railing'
;

RockyCrawl: Room 'Rocky Crawl'
    regions = []

    east = DomeRoom
    west = DeepRavine
    northwest = EgyptianRoom
;

DeepRavine: Room 'Deep Ravine'
    regions = []

    south = EastWestPassage
    east = Chasm
    west = RockyCrawl
    down = ReservoirSouth
;

Chasm: Room 'Chasm'
    regions = []

    south = DeepRavine
    east = NorthSouthPassage
;

NorthSouthPassage: Room 'North-South Passage'
    regions = []

    north = Chasm
    south = RoundRoom
    northeast = LoudRoom
;

LoudRoom: Room 'Loud Room'
    regions = []

    east = AncientChasm
    west = NorthSouthPassage
    up = DampCave
;

+ PlatinumBar: Thing 'Platinum Bar' 'Platinum Bar'
;

RoundRoom: Room 'Round Room'
    regions = []
;

EgyptianRoom: Room 'Egyptian Room'
    regions = []

    east = RockyCrawl
    west = GlacierRoom
;

+ GoldCoffin: Thing 'Gold Coffin' 'Gold Coffin'
;

GlacierRoom: Room 'Glacier Room'
    regions = []

    north = StreamView
    east = EgyptianRoom
    west = RubyRoom
;

+ Ice: Thing 'Ice' 'Ice'
;

StreamView: Room 'Stream View'
    regions = []

    north = GlacierRoom
    east = ReservoirSouth
;

ReservoirSouth: Room 'Reservoir South'
    regions = []

    north = ReservoirNorth
    south = DeepRavine
    west = StreamView
    up = DeepCanyon
;

+ Rope2: Thing 'Rope' 'Rope'
;

+ TrunkwithJewels: Thing 'Trunk with Jewels' 'Trunk with Jewels'
;

DeepCanyon: Room 'Deep Canyon'
    regions = []

    east = Dam
    southeast = ReservoirSouth
;

TorchRoom: Room 'Torch Room'
    regions = []
;

+ Torch: Thing 'Torch' 'Torch'
;

Dam: Room 'Dam'
    regions = []

    north = DamLobby
    south = DeepCanyon
    east = DampCave
;

DampCave: Room ' Damp Cave'
    regions = []

    south = LoudRoom
    east = Dam
;

+ SmallCrack: Thing 'Small Crack' 'Small Crack'
;

DamLobby: Room 'Dam Lobby'
    regions = []

    north = MaintenanceRoom
    south = Dam
    east = MaintenanceRoom
;

+ Matchbook: Thing 'Matchbook' 'Matchbook'
;

+ Guidebooks: Thing 'Guidebooks' 'Guidebooks'
;

MaintenanceRoom: Room 'Maintenance Room'
    regions = []

    south = DamLobby
    west = DamLobby
;

+ Tubeoftoothpaste: Thing 'Tube of toothpaste' 'Tube of toothpaste'
;

+ Wrench: Thing 'Wrench' 'Wrench'
;

+ Screwdriver: Thing 'Screwdriver' 'Screwdriver'
;

AncientChasm: Room 'Ancient Chasm'
    regions = []

    north = DeadEnd2
    south = LoudRoom
    west = DeadEnd
;

ReservoirNorth: Room 'Reservoir North'
    regions = []

    north = AtlantisRoom
    south = ReservoirSouth
;

AtlantisRoom: Room 'Atlantis Room'
    regions = []

    southeast = ReservoirNorth
    up = Cave
;

+ Trident: Thing 'Trident' 'Trident'
;

Cave: Room 'Cave'
    regions = []

    north = MirrorRoom
    down = AtlantisRoom
;

MirrorRoom: Room 'Mirror Room'
    regions = []

    north = SteepCrawlway
    east = Cave
    west = ColdPassage
;

ColdPassage: Room 'Cold Passage'
    regions = []

    north = SteepCrawlway
    east = MirrorRoom
    west = SlideRoom
;

SteepCrawlway: Room 'Steep Crawlway'
    regions = []

    south = MirrorRoom
    southwest = ColdPassage
;

SlideRoom: Room 'Slide Room'
    regions = []

    east = ColdPassage
    down = Cellar
;

EntrancetoHades: Room '*Entrance to Hades'
    regions = []

    east = LandoftheLivingDead
    up = Cave2
;

+ EvilSpirits: Thing 'Evil Spirits' 'Evil Spirits'
;

NarrowCrawlway: Room 'Narrow Crawlway'
    regions = []

    north = GrailRoom
    south = Cave2
    southwest = MirrorRoom2
;

GrailRoom: Room 'Grail Room'
    regions = []

    east = NarrowCrawlway
    west = RoundRoom2
    up = Temple
;

+ Grail: Thing 'Grail' 'Grail'
;

RoundRoom2: Room 'Round Room'
    regions = []

    north = DeepCanyon2
    south = WindingPassage
    east = GrailRoom
    west = EastWestPassage2
    northeast = Maze
    southwest = EngravingsCave
    northwest = NorthSouthPassage2
;

EastWestPassage2: Room 'East-West Passage'
    regions = []

    east = RoundRoom2
;

NorthSouthPassage2: Room 'North-South Passage'
    regions = []

    southeast = RoundRoom2
;

WindingPassage: Room 'Winding Passage'
    regions = []

    north = RoundRoom2
    east = MirrorRoom2
;

MirrorRoom2: Room 'Mirror Room'
    regions = []

    north = NarrowCrawlway
    east = Cave2
    west = WindingPassage
;

+ Mirror: Thing 'Mirror' 'Mirror'
;

Cave2: Room 'Cave'
    regions = []

    north = NarrowCrawlway
    west = MirrorRoom2
    down = EntrancetoHades
;

DeepCanyon2: Room 'Deep Canyon'
    regions = []

    south = RoundRoom2
;

Maze: Room 'Maze'
    regions = []

    southwest = RoundRoom2
;

EngravingsCave: Room 'Engravings Cave'
    regions = []

    north = RoundRoom2
    southeast = RiddleRoom
;

+ Engravings: Thing 'Engravings' 'Engravings'
;

RiddleRoom: Room 'Riddle Room'
    regions = []

    east = PearlRoom
    down = EngravingsCave
;

PearlRoom: Room 'Pearl Room'
    regions = []

    west = RiddleRoom
;

+ PearlNecklace: Thing 'Pearl Necklace' 'Pearl Necklace'
;

Temple: Room 'Temple'
    regions = []

    east = Altar
    west = GrailRoom
;

+ Brassbell: Thing 'Brass bell' 'Brass bell'
;

Altar: Room 'Altar'
    regions = []

    west = Temple
;

+ BlackBook: Thing 'Black Book' 'Black Book'
;

+ Candles: Thing 'Candles' 'Candles'
;

DeadEnd: Room 'Dead End'
    regions = []

    east = AncientChasm
;

DeadEnd2: Room 'Dead End'
    regions = []

    southwest = AncientChasm
;

LandoftheLivingDead: Room 'Land of the Living Dead'
    regions = []

    west = EntrancetoHades
;

RubyRoom: Room 'Ruby Room'
    regions = []

    south = GlacierRoom
;

+ Ruby: Thing 'Ruby' 'Ruby'
;

me: Actor
    location = WestofHouse
;

gameMain: GameMainDef
    initialPlayerChar = me
;

