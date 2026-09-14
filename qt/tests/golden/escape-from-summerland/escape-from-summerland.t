#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'escape-from-summerland'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Cooperate: Region
;

Jacquotte: Region
;

Man: Region
;

ManandJacquotte: Region
;

Manneedstoshow: Region
;

Shinobi: Region
;

IntheCaravanPark: Room 'In the Caravan Park'
    regions = [Man]

    south = MainTrack
    in = InsidetheCaravan
;

+ Enterthenexamineperson: Thing 'Enter then examine person' 'Enter then examine person'
;

MainTrack: Room 'Main Track'
    regions = [Man]

    north = IntheCaravanPark
    east = BeneaththeHedge
;

BeneaththeHedge: Room 'Beneath the Hedge'
    regions = [Man]

    east = DrizzleCourt
    west = MainTrack
;

+ Jacquottegonegoeastorwest: Thing 'Jacquotte gone go east or west' 'Jacquotte gone go east or west'
;

DrizzleCourt: Room 'Drizzle Court'
    regions = [Man]

    east = OrbisAlius
    west = BeneaththeHedge
;

OrbisAlius: Room 'Orbis Alius'
    regions = [Man and Jacquotte]

    north = MirrorMaze
    south = SelkieAquariumAquaticRegion
    east = UpintheStands
    west = DrizzleCourt
;

+ CLIMBorPUSHrobot: Thing 'CLIMB or PUSH robot' 'CLIMB or PUSH robot'
;

+ asJacquotte: Thing 'as Jacquotte' 'as Jacquotte'
;

MirrorMaze: Room 'Mirror Maze'
    regions = [Man]

    south = OrbisAlius
;

UpintheStands: Room 'Up in the Stands'
    regions = [Man]

    west = OrbisAlius
    down = BigTopWest
;

BigTopWest: Room 'Big Top West'
    regions = [Shinobi]

    east = BigTopEast
    up = UpintheStands
;

+ Leftunitfallshere: Thing 'Left unit falls here' 'Left unit falls here'
;

BigTopEast: Room 'Big Top East'
    regions = [Cooperate]

    north = AnotherHighPlace
    south = Backways
    west = BigTopWest
;

Backways: Room 'Backways'
    regions = [Cooperate]

    north = BigTopEast
    east = TheDressingRooms
    west = HomePlace
;

+ Jacquotteneedslight: Thing 'Jacquotte needs light' 'Jacquotte needs light'
;

TheDressingRooms: Room 'The Dressing Rooms'
    regions = [Shinobi]

    west = Backways
    in = GoingUpThingskippedbyman
;

HomePlace: Room 'Home Place'

    east = Backways
;

+ RightInteraction: Thing 'Right Interaction' 'Right Interaction'
;

+ CivilianDiscouragement: Thing 'Civilian Discouragement' 'Civilian Discouragement'
;

InsidetheCaravan: Room 'Inside the Caravan'
    regions = [Man]

    out = IntheCaravanPark
;

+ DeadMan: Thing 'Dead Man' 'Dead Man'
;

AnotherHighPlace: Room 'Another High Place'
    regions = [Jacquotte]

    south = BigTopEast
;

+ ShinyThingLeftunit: Thing 'Shiny Thing Left unit' 'Shiny Thing Left unit'
;

JacquottesCage: Room 'Jacquotte\'s Cage'
    regions = [Jacquotte]

    out = HomePlace
;

GoingUpThingskippedbyman: Room 'Going Up Thing (skipped by man)'
    regions = [Cooperate]

    out = TheDressingRooms
;

+ Skippedbyman: Thing 'Skipped by man' 'Skipped by man'
;

SelkieAquariumAquaticRegion: Room 'Selkie Aquarium/Aquatic Region'
    regions = [Shinobi]

    north = OrbisAlius
    east = ControlBooth
;

ControlBooth: Room 'Control Booth'
    regions = [Cooperate]

    east = ServiceCorridor
    west = SelkieAquariumAquaticRegion
;

+ Crank: Thing 'Crank' 'Crank'
;

ServiceCorridor: Room 'Service Corridor'
    regions = [Shinobi]

    west = ControlBooth
    in = GoingUpThingskippedbyman
;

+ searchrubbleforplunger: Thing 'search rubble for plunger' 'search rubble for plunger'
;

+ installSUCKDOOR: Thing 'install SUCK DOOR' 'install SUCK DOOR'
;

me: Actor
    location = IntheCaravanPark
;

gameMain: GameMainDef
    initialPlayerChar = me
;

