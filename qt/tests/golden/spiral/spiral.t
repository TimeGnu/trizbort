#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'spiral'
    byline = 'By J. Lautzenheiser'
    version = '1'
    desc = ''
;

Both: Region
;

Helen: Region
;

Ross: Region
;

Mountainside: Room 'Mountainside'
    regions = [Helen]

    south = FootoftheMountainS
;

FootoftheMountainS: Room 'Foot of the Mountain S'
    regions = [Helen]

    north = Mountainside
    east = FootoftheMountainE
    west = FootoftheMountainW
    southeast = ChurchIsle
;

+ Pole: Thing 'Pole' 'Pole'
;

FootoftheMountainW: Room 'Foot of the Mountain W'
    regions = [Helen]

    east = FootoftheMountainS
    west = Coast
;

FootoftheMountainE: Room 'Foot of the Mountain E'
    "fsdfds"
    regions = [Helen]

    west = FootoftheMountainS
    in = OntheCloud
;

OntheCloud: Room 'On the Cloud'
    regions = [Helen]

    out = FootoftheMountainE
;

Platform: Room 'Platform'
    regions = [Ross]

    east = MachineRoomWest
;

MachineRoomWest: Room 'Machine Room: West'
    regions = [Ross]

    north = OvertheGap
    east = MachineRoomCentral
    west = Platform
    up = BeneaththePipe
;

+ putpoleinrecess: Thing 'put pole in recess' 'put pole in recess'
;

FalltoCarriage: Room 'Fall to Carriage'
;

FalltoCarriage2: Room 'Fall to Carriage'
;

FalltoCarriage3: Room 'Fall to Carriage'
;

MachineRoomCentral: Room 'Machine Room: Central'
    regions = [Ross]

    east = MachineRoomEast
    west = MachineRoomWest
;

+ Crystal: Thing 'Crystal' 'Crystal'
;

MachineRoomEast: Room 'Machine Room: East'
    regions = [Ross]

    west = MachineRoomCentral
    down = HospitalRoom
;

BeneaththePipe: Room 'Beneath the Pipe'
    regions = [Ross]

    up = FalltoCarriage4
    down = MachineRoomWest
;

+ Crystal2: Thing 'Crystal' 'Crystal'
;

ChurchIsle: Room 'Church Isle'
    regions = [Helen]

    northwest = FootoftheMountainS
;

+ Page: Thing 'Page' 'Page'
;

FalltoCarriage4: Room 'Fall to Carriage'

    down = BeneaththePipe
;

OvertheGap: Room 'Over the Gap'
    regions = [Ross]

    north = ScrapRoom
    south = MachineRoomWest
;

+ Crystal3: Thing 'Crystal' 'Crystal'
;

ScrapRoom: Room 'Scrap Room'
    regions = [Ross]

    south = OvertheGap
    east = RethsDen
;

+ Phonecrystal: Thing 'Phonecrystal' 'Phonecrystal'
;

Coast: Room 'Coast'
    regions = [Helen]

    east = FootoftheMountainW
    up = Cave
;

+ Page2: Thing 'Page' 'Page'
;

Cave: Room 'Cave'
    regions = [Helen]

    south = Attic
    west = Tomb
    down = Coast
;

Attic: Room 'Attic'
    regions = [Helen]

    north = Cave
;

+ Stuffpage: Thing 'Stuff page' 'Stuff page'
;

+ Laptop: Thing 'Laptop' 'Laptop'
;

Tomb: Room 'Tomb'
    regions = [Helen]

    east = Cave
;

+ Coffinpage: Thing 'Coffin page' 'Coffin page'
;

Carriage: Room 'Carriage'
    regions = [Both]

    northeast = Mountainside
    southeast = MachineRoomWest
;

UndergroundLine: Room 'Underground Line'
    "Made in Twine, Men Don't Play With Dolls is an intimate and painful look into the beginnings of a lifelong struggle with addiction. Memories like these are not easily escaped."
    regions = [Helen]

    south = Tomb
;

+ Hat: Thing 'Hat' 'Hat'
;

+ Cat: Thing 'Cat' 'Cat'
;

HospitalRoom: Room 'Hospital Room'
    regions = [Ross]

    south = MachineRoomEast
;

RethsDen: Room 'Reth\'s Den'
    regions = [Ross]

    west = ScrapRoom
;

+ BoxSoul: Thing 'Box Soul' 'Box Soul'
;

UndergroundLine2: Room 'Underground Line'
    regions = [Both]

    south = RethsDen
    east = Vat
    west = Summit
    in = TrainWreck
;

Summit: Room 'Summit'
    regions = [Both]

    down = UndergroundLine2
;

Vat: Room 'Vat'
    regions = [Both]
;

+ Syringe: Thing 'Syringe' 'Syringe'
;

TrainWreck: Room 'Train Wreck'
    regions = [Ross]

    out = UndergroundLine2
;

me: Actor
    location = Mountainside
;

gameMain: GameMainDef
    initialPlayerChar = me
;

