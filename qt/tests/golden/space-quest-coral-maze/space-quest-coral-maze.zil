"space-quest-coral-maze main file"

<VERSION ZIP>
<CONSTANT RELEASEID 1>

"Main Loop"

<CONSTANT GAME-BANNER "space-quest-coral-maze|An interactive fiction by A Trizbort User">

<ROUTINE GO ()
    <CRLF> <CRLF>
    <TELL "" CR CR>
    <V-VERSION> <CRLF>
    <SETG HERE ,MAZE-START>
    <MOVE ,PLAYER ,HERE>
    <V-LOOK>
    <REPEAT ()
        <COND (<PARSER>
               <PERFORM ,PRSA ,PRSO ,PRSI>
               <COND (<NOT <GAME-VERB?>>
                      <APPLY <GETP ,HERE ,P?ACTION> ,M-END>
                      <CLOCKER>)>)>
        <SETG HERE <LOC ,WINNER>>>>

<INSERT-FILE "parser">

"Objects"

<ROOM MAZE-START
    (DESC "Maze Start")
    (IN ROOMS)
    (SOUTH TO T-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM T-RIGHT
    (DESC "T right")
    (IN ROOMS)
    (NORTH TO MAZE-START)
    (SOUTH TO UP-DOWN)
    (EAST TO LEFT-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM LEFT-RIGHT
    (DESC "Left Right")
    (IN ROOMS)
    (EAST TO T-DOWN)
    (WEST TO T-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM UP-DOWN
    (DESC "Up Down")
    (IN ROOMS)
    (NORTH TO T-RIGHT)
    (SOUTH TO T-LEFT)
    (FLAGS LIGHTBIT)>


<ROOM T-LEFT
    (DESC "T left")
    (IN ROOMS)
    (NORTH TO UP-DOWN)
    (WEST TO DEAD-END)
    (FLAGS LIGHTBIT)>


<ROOM DEAD-END
    (DESC "Dead End")
    (IN ROOMS)
    (EAST TO T-LEFT)
    (FLAGS LIGHTBIT)>


<ROOM T-DOWN
    (DESC "T down")
    (IN ROOMS)
    (SOUTH TO UP-DOWN-2)
    (WEST TO LEFT-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM UP-DOWN-2
    (DESC "Up down")
    (IN ROOMS)
    (NORTH TO T-DOWN)
    (SOUTH TO UP-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM UP-RIGHT
    (DESC "Up Right")
    (IN ROOMS)
    (NORTH TO UP-DOWN-2)
    (EAST TO LEFT-RIGHT-2)
    (FLAGS LIGHTBIT)>


<ROOM LEFT-RIGHT-2
    (DESC "Left Right")
    (IN ROOMS)
    (EAST TO LEFT-RIGHT-3)
    (WEST TO UP-RIGHT)
    (FLAGS LIGHTBIT)>


<ROOM LEFT-RIGHT-3
    (DESC "Left Right")
    (IN ROOMS)
    (EAST TO T-DOWN-2)
    (WEST TO LEFT-RIGHT-2)
    (FLAGS LIGHTBIT)>


<ROOM T-DOWN-2
    (DESC "T down")
    (IN ROOMS)
    (SOUTH TO T-RIGHT-2)
    (WEST TO LEFT-RIGHT-3)
    (FLAGS LIGHTBIT)>


<ROOM T-RIGHT-2
    (DESC "T right")
    (IN ROOMS)
    (NORTH TO T-DOWN-2)
    (SOUTH TO CROSS-ROADS)
    (FLAGS LIGHTBIT)>


<ROOM CROSS-ROADS
    (DESC "Cross Roads")
    (IN ROOMS)
    (NORTH TO T-RIGHT-2)
    (SOUTH TO UP-DOWN-3)
    (FLAGS LIGHTBIT)>


<ROOM UP-DOWN-3
    (DESC "Up Down")
    (IN ROOMS)
    (NORTH TO CROSS-ROADS)
    (WEST TO UP-LEFT)
    (FLAGS LIGHTBIT)>


<ROOM UP-LEFT
    (DESC "Up Left")
    (IN ROOMS)
    (EAST TO UP-DOWN-3)
    (WEST TO T-DOWN-3)
    (FLAGS LIGHTBIT)>


<ROOM T-DOWN-3
    (DESC "T down")
    (IN ROOMS)
    (SOUTH TO T-LEFT-2)
    (EAST TO UP-LEFT)
    (FLAGS LIGHTBIT)>


<ROOM T-LEFT-2
    (DESC "T left")
    (IN ROOMS)
    (NORTH TO T-DOWN-3)
    (SOUTH TO UP-RIGHT-2)
    (FLAGS LIGHTBIT)>


<ROOM UP-RIGHT-2
    (DESC "Up right")
    (IN ROOMS)
    (NORTH TO T-LEFT-2)
    (EAST TO PANEL)
    (FLAGS LIGHTBIT)>


<ROOM PANEL
    (DESC "Panel")
    (IN ROOMS)
    (EAST PER TRIZBORT-CONDITIONAL-EXIT)
    (WEST TO UP-RIGHT-2)
    (FLAGS LIGHTBIT)>


<ROUTINE TRIZBORT-CONDITIONAL-EXIT ()
    <TELL "An export nymph appears on your keyboard. She says, 'You can't go that way, as that exit was marked as conditional, you know, a dotted line, in Trizbort. Obviously in your game you'll have a better rationale for this than, er, me.' She looks embarrassed. 'Bye!'" CR>
    <RFALSE>>


<OBJECT NEED-ALIEN-PAPER
    (IN PANEL)
    (DESC "Need alien paper")
    (SYNONYM PAPER)
    (ADJECTIVE NEED ALIEN)
    (FLAGS TAKEBIT)>


<OBJECT FROM-BEHIND-SHARK
    (IN PANEL)
    (DESC "From behind shark")
    (SYNONYM SHARK)
    (ADJECTIVE FROM BEHIND)
    (FLAGS TAKEBIT)>


<ROOM GREEN-BUTTON
    (DESC "Green Button")
    (IN ROOMS)
    (SOUTH TO HALL)
    (EAST TO NICE-SCENE)
    (WEST PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<OBJECT USE-PANEL--3
    (IN GREEN-BUTTON)
    (DESC "Use panel * 3")
    (SYNONYM 3)
    (ADJECTIVE USE PANEL OBJECT)
    (FLAGS TAKEBIT VOWELBIT)>


<ROOM NICE-SCENE
    (DESC "Nice scene")
    (IN ROOMS)
    (WEST TO GREEN-BUTTON)
    (FLAGS LIGHTBIT)>


<ROOM HALL
    (DESC "Hall")
    (IN ROOMS)
    (NORTH TO GREEN-BUTTON)
    (SOUTH TO HALL-2)
    (FLAGS LIGHTBIT)>


<ROOM HALL-2
    (DESC "Hall")
    (IN ROOMS)
    (NORTH TO HALL)
    (EAST TO SHIP)
    (FLAGS LIGHTBIT)>


<ROOM SHIP
    (DESC "Ship")
    (IN ROOMS)
    (WEST TO HALL-2)
    (FLAGS LIGHTBIT)>


<ROOM CRAFT-START
    (DESC "Craft Start")
    (IN ROOMS)
    (WEST TO R00)
    (FLAGS LIGHTBIT)>


<ROOM R00
    (DESC "R00")
    (IN ROOMS)
    (EAST TO CRAFT-START)
    (WEST TO R00-2)
    (FLAGS LIGHTBIT)>


<ROOM R00-2
    (DESC "R00")
    (IN ROOMS)
    (EAST TO R00)
    (WEST TO R00-3)
    (FLAGS LIGHTBIT)>


<ROOM R00-3
    (DESC "R00")
    (IN ROOMS)
    (EAST TO R00-2)
    (WEST TO OTHER-SHIP)
    (FLAGS LIGHTBIT)>


<ROOM OTHER-SHIP
    (DESC "Other Ship")
    (IN ROOMS)
    (EAST TO R00-3)
    (WEST TO R00-4)
    (FLAGS LIGHTBIT)>


<OBJECT BOX
    (IN OTHER-SHIP)
    (DESC "Box")
    (SYNONYM BOX)
    (FLAGS TAKEBIT)>


<OBJECT NEED-PRESSURE-SUIT
    (IN OTHER-SHIP)
    (DESC "Need pressure suit")
    (SYNONYM SUIT)
    (ADJECTIVE NEED PRESSURE)
    (FLAGS TAKEBIT)>


<ROOM R00-4
    (DESC "R00")
    (IN ROOMS)
    (NORTH TO CITY-ENTRY)
    (EAST TO OTHER-SHIP)
    (FLAGS LIGHTBIT)>


<ROOM CITY-ENTRY
    (DESC "City Entry")
    (IN ROOMS)
    (SOUTH TO R00-4)
    (NW PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<OBJECT USE-BOX
    (IN CITY-ENTRY)
    (DESC "USE BOX")
    (SYNONYM BOX)
    (ADJECTIVE USE)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT EXIT-CITY-TO-LEAVE
    (IN CITY-ENTRY)
    (DESC "EXIT CITY to leave")
    (SYNONYM LEAVE)
    (ADJECTIVE EXIT CITY TO)
    (FLAGS TAKEBIT VOWELBIT)>


<ROOM DOCK-BAY
    (DESC "Dock Bay")
    (IN ROOMS)
    (EAST TO BIG-DOOR)
    (SE PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<ROOM BIG-DOOR
    (DESC "Big Door")
    (IN ROOMS)
    (EAST TO HALL-3)
    (WEST TO DOCK-BAY)
    (FLAGS LIGHTBIT)>


<OBJECT USE-PANEL
    (IN BIG-DOOR)
    (DESC "USE PANEL")
    (SYNONYM PANEL)
    (ADJECTIVE USE)
    (FLAGS TAKEBIT VOWELBIT)>


<ROOM HALL-3
    (DESC "Hall")
    (IN ROOMS)
    (SOUTH TO BIG-DOOR)
    (EAST TO HALL-4)
    (WEST TO ALIENS)
    (FLAGS LIGHTBIT)>


<ROOM ALIENS
    (DESC "Aliens")
    (IN ROOMS)
    (EAST TO HALL-3)
    (FLAGS LIGHTBIT)>


<ROOM HALL-4
    (DESC "Hall")
    (IN ROOMS)
    (SOUTH TO HALL-5)
    (WEST TO HALL-3)
    (FLAGS LIGHTBIT)>


<ROOM HALL-5
    (DESC "Hall")
    (IN ROOMS)
    (NORTH TO HALL-4)
    (EAST TO STAY-NORTH)
    (FLAGS LIGHTBIT)>


<ROOM STAY-NORTH
    (DESC "Stay North")
    (IN ROOMS)
    (EAST TO HALL-6)
    (WEST TO HALL-5)
    (FLAGS LIGHTBIT)>


<ROOM HALL-6
    (DESC "Hall")
    (IN ROOMS)
    (NORTH TO HALL-7)
    (EAST TO ALIENS-2)
    (WEST TO STAY-NORTH)
    (FLAGS LIGHTBIT)>


<ROOM ALIENS-2
    (DESC "Aliens")
    (IN ROOMS)
    (WEST TO HALL-6)
    (FLAGS LIGHTBIT)>


<OBJECT EXTRA-BAD
    (IN ALIENS-2)
    (DESC "Extra bad")
    (SYNONYM BAD)
    (ADJECTIVE EXTRA)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT THEYLL-FOLLOW
    (IN ALIENS-2)
    (DESC "They'll follow")
    (SYNONYM FOLLOW)
    (ADJECTIVE THEYLL)
    (FLAGS TAKEBIT)>


<ROOM HALL-7
    (DESC "Hall")
    (IN ROOMS)
    (NORTH TO HALL-8)
    (SOUTH TO HALL-6)
    (FLAGS LIGHTBIT)>


<ROOM HALL-8
    (DESC "Hall")
    (IN ROOMS)
    (SOUTH TO HALL-7)
    (EAST TO HALL-9)
    (FLAGS LIGHTBIT)>


<ROOM HALL-9
    (DESC "Hall")
    (IN ROOMS)
    (EAST TO MUS-EUM)
    (WEST TO HALL-8)
    (FLAGS LIGHTBIT)>


<ROOM MUS-EUM
    (DESC "Mus-eum")
    (IN ROOMS)
    (WEST TO HALL-9)
    (FLAGS LIGHTBIT)>


<OBJECT MASK
    (IN MUS-EUM)
    (DESC "MASK")
    (SYNONYM MASK)
    (FLAGS TAKEBIT)>


<OBJECT OUTFIT
    (IN MUS-EUM)
    (DESC "OUTFIT")
    (SYNONYM OUTFIT)
    (FLAGS TAKEBIT VOWELBIT)>


<ROOM ITEM
    (DESC "Item")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM USE-LESS
    (DESC "Use-less")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM DEATH
    (DESC "Death")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM WALK-THRU
    (DESC "Walk Thru")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>

