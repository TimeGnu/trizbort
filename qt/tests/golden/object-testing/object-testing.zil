"object-testing main file"

<VERSION ZIP>
<CONSTANT RELEASEID 1>

"Main Loop"

<CONSTANT GAME-BANNER "object-testing|An interactive fiction by A Trizbort User">

<ROUTINE GO ()
    <CRLF> <CRLF>
    <TELL "" CR CR>
    <V-VERSION> <CRLF>
    <SETG HERE ,TEST-ROOM>
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

<ROOM TEST-ROOM
    (DESC "Test Room")
    (IN ROOMS)
    (EAST TO CAVE)
    (FLAGS LIGHTBIT)>


<OBJECT MR-JONES
    (IN TEST-ROOM)
    (DESC "Mr Jones")
    (SYNONYM JONES)
    (ADJECTIVE MR)
    (FLAGS TAKEBIT CONTBIT)>


<OBJECT HAT
    (IN MR-JONES)
    (DESC "Hat")
    (SYNONYM HAT)
    (FLAGS TAKEBIT CONTBIT)>


<OBJECT HATBAND
    (IN HAT)
    (DESC "Hatband")
    (SYNONYM HATBAND)
    (FLAGS TAKEBIT)>


<OBJECT FISHBOWL
    (IN MR-JONES)
    (DESC "fishbowl")
    (SYNONYM FISHBOWL)
    (FLAGS TAKEBIT CONTBIT)>


<OBJECT GOLDFISH
    (IN FISHBOWL)
    (DESC "goldfish")
    (SYNONYM GOLDFISH)
    (FLAGS TAKEBIT)>


<OBJECT MRS-JONES
    (IN TEST-ROOM)
    (DESC "Mrs Jones")
    (SYNONYM JONES)
    (ADJECTIVE MRS)
    (FLAGS TAKEBIT CONTBIT)>


<OBJECT RABBIT
    (IN MRS-JONES)
    (DESC "rabbit")
    (SYNONYM RABBIT)
    (FLAGS TAKEBIT)>


<ROOM CAVE
    (DESC "cave")
    (IN ROOMS)
    (WEST TO TEST-ROOM)
    (FLAGS LIGHTBIT)>

