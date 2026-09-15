"Doors Test main file"

<VERSION ZIP>
<CONSTANT RELEASEID 1>

"Main Loop"

<CONSTANT GAME-BANNER "Doors Test|An interactive fiction by Trizbort Qt port">

<ROUTINE GO ()
    <CRLF> <CRLF>
    <TELL "" CR CR>
    <V-VERSION> <CRLF>
    <SETG HERE ,WEST-ROOM>
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

<ROOM WEST-ROOM
    (DESC "West Room")
    (IN ROOMS)
    (NORTH TO NORTH-ROOM)
    (EAST TO EAST-ROOM)
    (FLAGS LIGHTBIT)>


<ROOM EAST-ROOM
    (DESC "East Room")
    (IN ROOMS)
    (WEST TO WEST-ROOM)
    (FLAGS LIGHTBIT)>


<ROOM NORTH-ROOM
    (DESC "North Room")
    (IN ROOMS)
    (SOUTH TO WEST-ROOM)
    (FLAGS LIGHTBIT)>

