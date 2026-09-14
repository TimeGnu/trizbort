"spiral main file"

<VERSION ZIP>
<CONSTANT RELEASEID 1>

"Main Loop"

<CONSTANT GAME-BANNER "spiral|An interactive fiction by J. Lautzenheiser">

<ROUTINE GO ()
    <CRLF> <CRLF>
    <TELL "" CR CR>
    <V-VERSION> <CRLF>
    <SETG HERE ,MOUNTAINSIDE>
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

<ROOM MOUNTAINSIDE
    (DESC "Mountainside")
    (IN ROOMS)
    (SOUTH TO FOOT-OF-THE-MOUNTAIN-S)
    (FLAGS LIGHTBIT)>


<ROOM FOOT-OF-THE-MOUNTAIN-S
    (DESC "Foot of the Mountain S")
    (IN ROOMS)
    (NORTH TO MOUNTAINSIDE)
    (EAST TO FOOT-OF-THE-MOUNTAIN-E)
    (WEST TO FOOT-OF-THE-MOUNTAIN-W)
    (SE TO CHURCH-ISLE)
    (FLAGS LIGHTBIT)>


<OBJECT POLE
    (IN FOOT-OF-THE-MOUNTAIN-S)
    (DESC "Pole")
    (SYNONYM POLE)
    (FLAGS TAKEBIT)>


<ROOM FOOT-OF-THE-MOUNTAIN-W
    (DESC "Foot of the Mountain W")
    (IN ROOMS)
    (EAST TO FOOT-OF-THE-MOUNTAIN-S)
    (WEST PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<ROUTINE TRIZBORT-CONDITIONAL-EXIT ()
    <TELL "An export nymph appears on your keyboard. She says, 'You can't go that way, as that exit was marked as conditional, you know, a dotted line, in Trizbort. Obviously in your game you'll have a better rationale for this than, er, me.' She looks embarrassed. 'Bye!'" CR>
    <RFALSE>>


<ROOM FOOT-OF-THE-MOUNTAIN-E
    (DESC "Foot of the Mountain E")
    (IN ROOMS)
    (LDESC "fsdfds")
    (WEST TO FOOT-OF-THE-MOUNTAIN-S)
    (IN TO ON-THE-CLOUD)
    (FLAGS LIGHTBIT)>


<ROOM ON-THE-CLOUD
    (DESC "On the Cloud")
    (IN ROOMS)
    (OUT TO FOOT-OF-THE-MOUNTAIN-E)
    (FLAGS LIGHTBIT)>


<ROOM PLATFORM
    (DESC "Platform")
    (IN ROOMS)
    (EAST TO MACHINE-ROOM-WEST)
    (FLAGS LIGHTBIT)>


<ROOM MACHINE-ROOM-WEST
    (DESC "Machine Room: West")
    (IN ROOMS)
    (NORTH PER TRIZBORT-CONDITIONAL-EXIT)
    (EAST TO MACHINE-ROOM-CENTRAL)
    (WEST TO PLATFORM)
    (UP TO BENEATH-THE-PIPE)
    (FLAGS LIGHTBIT)>


<OBJECT PUT-POLE-IN-RECESS
    (IN MACHINE-ROOM-WEST)
    (DESC "(put pole in recess)")
    (SYNONYM RECESS)
    (ADJECTIVE PUT POLE IN)
    (FLAGS TAKEBIT)>


<ROOM FALL-TO-CARRIAGE
    (DESC "Fall to Carriage")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM FALL-TO-CARRIAGE-2
    (DESC "Fall to Carriage")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM FALL-TO-CARRIAGE-3
    (DESC "Fall to Carriage")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<ROOM MACHINE-ROOM-CENTRAL
    (DESC "Machine Room: Central")
    (IN ROOMS)
    (EAST TO MACHINE-ROOM-EAST)
    (WEST TO MACHINE-ROOM-WEST)
    (FLAGS LIGHTBIT)>


<OBJECT CRYSTAL
    (IN MACHINE-ROOM-CENTRAL)
    (DESC "Crystal")
    (SYNONYM CRYSTAL)
    (FLAGS TAKEBIT)>


<ROOM MACHINE-ROOM-EAST
    (DESC "Machine Room: East")
    (IN ROOMS)
    (WEST TO MACHINE-ROOM-CENTRAL)
    (DOWN TO HOSPITAL-ROOM)
    (FLAGS LIGHTBIT)>


<ROOM BENEATH-THE-PIPE
    (DESC "Beneath the Pipe")
    (IN ROOMS)
    (UP TO FALL-TO-CARRIAGE-4)
    (DOWN TO MACHINE-ROOM-WEST)
    (FLAGS LIGHTBIT)>


<OBJECT CRYSTAL2
    (IN BENEATH-THE-PIPE)
    (DESC "Crystal")
    (SYNONYM CRYSTAL)
    (FLAGS TAKEBIT)>


<ROOM CHURCH-ISLE
    (DESC "Church Isle")
    (IN ROOMS)
    (NW TO FOOT-OF-THE-MOUNTAIN-S)
    (FLAGS LIGHTBIT)>


<OBJECT PAGE
    (IN CHURCH-ISLE)
    (DESC "Page")
    (SYNONYM PAGE)
    (FLAGS TAKEBIT)>


<ROOM FALL-TO-CARRIAGE-4
    (DESC "Fall to Carriage")
    (IN ROOMS)
    (DOWN TO BENEATH-THE-PIPE)
    (FLAGS LIGHTBIT)>


<ROOM OVER-THE-GAP
    (DESC "Over the Gap")
    (IN ROOMS)
    (NORTH PER TRIZBORT-CONDITIONAL-EXIT)
    (SOUTH PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<OBJECT CRYSTAL3
    (IN OVER-THE-GAP)
    (DESC "Crystal")
    (SYNONYM CRYSTAL)
    (FLAGS TAKEBIT)>


<ROOM SCRAP-ROOM
    (DESC "Scrap Room")
    (IN ROOMS)
    (SOUTH PER TRIZBORT-CONDITIONAL-EXIT)
    (EAST TO RETHS-DEN)
    (FLAGS LIGHTBIT)>


<OBJECT PHONECRYSTAL
    (IN SCRAP-ROOM)
    (DESC "Phone/crystal")
    (SYNONYM PHONECRYSTAL)
    (FLAGS TAKEBIT)>


<ROOM COAST
    (DESC "Coast")
    (IN ROOMS)
    (EAST PER TRIZBORT-CONDITIONAL-EXIT)
    (UP PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<OBJECT PAGE2
    (IN COAST)
    (DESC "Page")
    (SYNONYM PAGE)
    (FLAGS TAKEBIT)>


<ROOM CAVE
    (DESC "Cave")
    (IN ROOMS)
    (SOUTH TO ATTIC)
    (WEST PER TRIZBORT-CONDITIONAL-EXIT)
    (DOWN PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<ROOM ATTIC
    (DESC "Attic")
    (IN ROOMS)
    (NORTH TO CAVE)
    (FLAGS LIGHTBIT)>


<OBJECT STUFF-PAGE
    (IN ATTIC)
    (DESC "Stuff (page)")
    (SYNONYM PAGE)
    (ADJECTIVE STUFF)
    (FLAGS TAKEBIT)>


<OBJECT LAPTOP
    (IN ATTIC)
    (DESC "Laptop")
    (SYNONYM LAPTOP)
    (FLAGS TAKEBIT)>


<ROOM TOMB
    (DESC "Tomb")
    (IN ROOMS)
    (EAST PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<OBJECT COFFIN-PAGE
    (IN TOMB)
    (DESC "Coffin (page)")
    (SYNONYM PAGE)
    (ADJECTIVE COFFIN)
    (FLAGS TAKEBIT)>


<ROOM CARRIAGE
    (DESC "Carriage")
    (IN ROOMS)
    (NE PER TRIZBORT-CONDITIONAL-EXIT)
    (SE PER TRIZBORT-CONDITIONAL-EXIT)
    (FLAGS LIGHTBIT)>


<ROOM UNDERGROUND-LINE
    (DESC "Underground Line")
    (IN ROOMS)
    (LDESC "Made in Twine, Men Don't Play With Dolls is an intimate and painful look into the beginnings of a lifelong struggle with addiction. Memories like these are not easily escaped.")
    (SOUTH TO TOMB)
    (FLAGS LIGHTBIT)>


<OBJECT HAT
    (IN UNDERGROUND-LINE)
    (DESC "Hat")
    (SYNONYM HAT)
    (FLAGS TAKEBIT)>


<OBJECT CAT
    (IN UNDERGROUND-LINE)
    (DESC "Cat")
    (SYNONYM CAT)
    (FLAGS TAKEBIT)>


<ROOM HOSPITAL-ROOM
    (DESC "Hospital Room")
    (IN ROOMS)
    (SOUTH TO MACHINE-ROOM-EAST)
    (FLAGS LIGHTBIT)>


<ROOM RETHS-DEN
    (DESC "Reth's Den")
    (IN ROOMS)
    (WEST TO SCRAP-ROOM)
    (FLAGS LIGHTBIT)>


<OBJECT BOX-SOUL
    (IN RETHS-DEN)
    (DESC "Box (Soul)")
    (SYNONYM SOUL)
    (ADJECTIVE BOX)
    (FLAGS TAKEBIT)>


<ROOM UNDERGROUND-LINE-2
    (DESC "Underground Line")
    (IN ROOMS)
    (SOUTH TO RETHS-DEN)
    (EAST TO VAT)
    (WEST TO SUMMIT)
    (IN TO TRAIN-WRECK)
    (FLAGS LIGHTBIT)>


<ROOM SUMMIT
    (DESC "Summit")
    (IN ROOMS)
    (DOWN TO UNDERGROUND-LINE-2)
    (FLAGS LIGHTBIT)>


<ROOM VAT
    (DESC "Vat")
    (IN ROOMS)
    (FLAGS LIGHTBIT)>


<OBJECT SYRINGE
    (IN VAT)
    (DESC "Syringe")
    (SYNONYM SYRINGE)
    (FLAGS TAKEBIT)>


<ROOM TRAIN-WRECK
    (DESC "Train Wreck")
    (IN ROOMS)
    (OUT TO UNDERGROUND-LINE-2)
    (FLAGS LIGHTBIT)>

