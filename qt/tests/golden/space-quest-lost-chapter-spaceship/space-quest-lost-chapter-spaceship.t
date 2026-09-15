#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'space-quest-lost-chapter-spaceship'
    byline = 'By A Trizbort User'
    version = '1'
    desc = ''
;

Elevator: Region
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

EngineRoom: Room 'Engine Room'
    regions = [Through Way]

    south = Hallway
;

Hallway: Room 'Hallway'
    regions = [Through Way]

    north = EngineRoom
    west = Entry
;

Entry: Room 'Entry'
    regions = [Through Way]

    north = WiresLeft
    east = Hallway
    west = PilotRoom
;

PilotRoom: Room 'Pilot Room'
    regions = [Through Way]

    south = Entry
    southwest = Hall
;

WiresLeft: Room 'Wires Left'
    regions = [Important item]

    south = Entry
    east = WiresRight
;

Elevator2: Room 'Elevator'
    regions = [Elevator]

    south = L2room
;

L2room: Room 'L2 room'
    regions = [Through Way]

    north = Elevator2
    east = FromPod
;

FromPod: Room 'From Pod'
    regions = [Through Way]

    west = L2room
;

+ GETNEWHAIR: Thing 'GET NEW HAIR' 'GET NEW HAIR'
;

L3hall: Room 'L3 hall'
    regions = [Through Way]

    north = Elevator3
    east = L3hall2
    west = Guards9
;

Elevator3: Room 'Elevator'
    regions = [Elevator]

    south = L3hall
;

L3hall2: Room 'L3 hall'
    regions = [Through Way]

    north = Lookout
    east = Guards8
    west = L3hall
;

Lookout: Room 'Lookout'
    regions = [Important item]

    south = L3hall2
;

Bar: Room 'Bar'
    regions = [Important item]

    east = BarLISTEN
;

+ OrderFrost: Thing 'Order Frost' 'Order Frost'
;

+ Tormeenian: Thing 'Tormeenian' 'Tormeenian'
;

BarLISTEN: Room 'Bar LISTEN'
    regions = [Through Way]

    south = L4Hall
    east = Bar2
    west = Bar
;

Bar2: Room 'Bar'
    regions = [Important item]

    west = BarLISTEN
;

+ tradestone: Thing 'trade stone' 'trade stone'
;

+ forclearance: Thing 'for clearance' 'for clearance'
;

L4Hall: Room 'L4 Hall'
    regions = [Through Way]

    north = BarLISTEN
    east = Guards
    west = L4Hall2
;

Guards: Room 'Guards'
    regions = [Useless]

    west = L4Hall
;

L4Hall2: Room 'L4 Hall'
    regions = [Through Way]

    north = Elevator4
    east = L4Hall
    west = Guards2
;

Elevator4: Room 'Elevator'
    regions = [Elevator]

    south = L4Hall2
;

Guards2: Room 'Guards'
    regions = [Useless]

    east = L4Hall2
;

Elevator5: Room 'Elevator'
    regions = [Elevator]

    south = L5hall
;

L5hall: Room 'L5 hall'
    regions = [Useless]

    north = Elevator5
    east = Guards4
    west = Guards3
;

Guards3: Room 'Guards'
    regions = [Useless]

    east = L5hall
;

Guards4: Room 'Guards'
    regions = [Useless]

    west = L5hall
;

L6hall: Room 'L6 hall'
    regions = [Useless]

    north = ScienceRoom
    east = L6hall2
    west = Guards6
;

ScienceRoom: Room 'Science Room'
    regions = [Useless]

    south = L6hall
;

L6hall2: Room 'L6 hall'
    regions = [Useless]

    north = Elevator6
    east = L6hall3
    west = L6hall
;

L6hall3: Room 'L6 hall'
    regions = [Useless]

    north = Elevator6
    east = Guards7
    west = L6hall2
;

Elevator6: Room 'Elevator'
    regions = [Elevator]

    south = L6hall2
    east = L6hall3
;

Science: Room 'Science'
    regions = [Useless]

    east = Science2
;

Science2: Room 'Science'
    regions = [Useless]

    south = L7hall
    west = Science
;

L7hall: Room 'L7 hall'
    regions = [Useless]

    north = Science2
    east = L7hall2
    west = Guards10
;

L7hall2: Room 'L7 hall'
    regions = [Useless]

    north = Elevator7
    east = Guards11
    west = L7hall
;

Elevator7: Room 'Elevator'
    regions = [Elevator]

    south = L7hall2
;

Elevator8: Room 'Elevator'
    regions = [Elevator]

    south = HallL10
;

HallL10: Room 'Hall L10'
    regions = [Through Way]

    north = Elevator8
    east = HallL102
;

HallL102: Room 'Hall L10'
    regions = [Through Way]

    north = WAITLISTEN
    east = Guards12
    west = HallL10
;

WAITLISTEN: Room 'WAIT & LISTEN'
    regions = [Important item]

    south = HallL102
;

Elevator9: Room 'Elevator'
    regions = [Elevator]

    south = GuardsL8
;

Elevator10: Room 'Elevator'
    regions = [Elevator]

    south = GuardsL9
;

Elevator11: Room 'Elevator'
    regions = [Elevator]

    south = HallL11
;

HallL11: Room 'Hall L11'
    regions = [Through Way]

    north = Elevator11
    east = HallL112
;

HallL112: Room 'Hall L11'
    regions = [Through Way]

    north = Scienceroom2
    east = Guards13
    west = HallL11
;

Scienceroom2: Room 'Science room'
    regions = [Through Way]

    south = HallL112
    west = BombRoom
;

BombRoom: Room 'Bomb Room'
    regions = [Important item]

    east = Scienceroom2
;

DockingBay: Room 'Docking Bay'
    regions = [Important item]

    east = ControlsL12
;

+ LISTEN: Thing 'LISTEN' 'LISTEN'
;

ControlsL12: Room 'Controls L12'
    regions = [Through Way]

    north = Elevator12
    west = DockingBay
;

Elevator12: Room 'Elevator'
    regions = [Elevator]

    south = ControlsL12
;

WiresRight: Room 'Wires Right'
    regions = [Important item]

    west = WiresLeft
;

+ POD: Thing 'POD' 'POD'
;

Leverroom: Room 'Lever room'
    regions = [Important item]

    north = EngineRoom
;

Hall: Room 'Hall'
    regions = [Through Way]

    east = Fight2Aliens
    northwest = PilotRoom
;

Fight2Aliens: Room 'Fight 2 Aliens'
    regions = [Lethal]

    east = Hall2
    west = Hall
;

Hall2: Room 'Hall'
    regions = [Through Way]

    south = Hall3
    west = Fight2Aliens
;

Hall3: Room 'Hall'
    regions = [Through Way]

    north = Hall2
    east = SUITHELMET
;

SUITHELMET: Room 'SUIT HELMET'
    regions = [Important item]

    west = Hall3
;

GunCompartment: Room 'Gun Compart ment'
    regions = [Important item]

    north = PilotRoom
;

+ GUN: Thing 'GUN' 'GUN'
;

Guards5: Room 'Guards'
    regions = [Useless]

    south = L1hall
;

L1hall: Room 'L1 hall'
    regions = [Useless]

    east = L1hall2
    west = Guards5
;

L1hall2: Room 'L1 hall'
    regions = [Through Way]

    north = Elevator13
    east = L1hall3
    west = L1hall
;

Elevator13: Room 'Elevator'
    regions = [Elevator]

    south = L1hall2
;

L1hall3: Room 'L1 hall'
    regions = [Through Way]

    north = Science3
    west = L1hall2
;

Science3: Room 'Science'
    regions = [Through Way]

    south = L1hall3
    west = Science4
;

Science4: Room 'Science'
    regions = [Important item]

    east = Science3
;

Guards6: Room 'Guards'
    regions = [Useless]

    east = L6hall
;

Guards7: Room 'Guards'
    regions = [Useless]

    west = L6hall3
;

Guards8: Room 'Guards'
    regions = [Useless]

    west = L3hall2
;

Guards9: Room 'Guards'
    regions = [Useless]

    east = L3hall
;

Guards10: Room 'Guards'
    regions = [Useless]

    east = L7hall
;

Guards11: Room 'Guards'
    regions = [Useless]

    west = L7hall2
;

GuardsL8: Room 'Guards L8'
    regions = [Useless]

    north = Elevator9
;

GuardsL9: Room 'Guards L9'
    regions = [Useless]

    north = Elevator10
;

Guards12: Room 'Guards'
    regions = [Useless]

    west = HallL102
;

Guards13: Room 'Guards'
    regions = [Useless]

    west = HallL112
;

me: Actor
    location = EngineRoom
;

gameMain: GameMainDef
    initialPlayerChar = me
;

