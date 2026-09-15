"properties-in-brackets-basic main file"

<VERSION ZIP>
<CONSTANT RELEASEID 1>

"Main Loop"

<CONSTANT GAME-BANNER "properties-in-brackets-basic|An interactive fiction by A Trizbort User">

<ROUTINE GO ()
    <CRLF> <CRLF>
    <TELL "" CR CR>
    <V-VERSION> <CRLF>
    <SETG HERE ,CAVE>
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

<ROOM CAVE
    (DESC "Cave")
    (IN ROOMS)
    (EAST TO ERROR-ROOM)
    (FLAGS LIGHTBIT)>


<OBJECT FRED
    (IN CAVE)
    (DESC "Fred")
    (SYNONYM FRED)
    (FLAGS TAKEBIT)>


<OBJECT WILMA
    (IN CAVE)
    (DESC "Wilma")
    (SYNONYM WILMA)
    (FLAGS TAKEBIT)>


<OBJECT PAT
    (IN CAVE)
    (DESC "Pat")
    (SYNONYM PAT)
    (FLAGS TAKEBIT)>


<OBJECT MICE
    (IN CAVE)
    (DESC "Mice")
    (SYNONYM MICE)
    (FLAGS TAKEBIT)>


<OBJECT MESS
    (IN CAVE)
    (DESC "Mess")
    (SYNONYM MESS)
    (FLAGS TAKEBIT)>


<OBJECT OVEN
    (IN CAVE)
    (DESC "Oven")
    (SYNONYM OVEN)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT TABLE
    (IN CAVE)
    (DESC "Table")
    (SYNONYM TABLE)
    (FLAGS TAKEBIT)>


<OBJECT WINDOW
    (IN CAVE)
    (DESC "Window")
    (SYNONYM WINDOW)
    (FLAGS TAKEBIT)>


<ROOM ERROR-ROOM
    (DESC "Error Room")
    (IN ROOMS)
    (WEST TO CAVE)
    (FLAGS LIGHTBIT)>


<OBJECT GENDERS
    (IN ERROR-ROOM)
    (DESC "Genders")
    (SYNONYM GENDERS)
    (FLAGS TAKEBIT)>


<OBJECT GENDERS2
    (IN ERROR-ROOM)
    (DESC "Genders")
    (SYNONYM GENDERS)
    (FLAGS TAKEBIT)>


<OBJECT GENDERS3
    (IN ERROR-ROOM)
    (DESC "Genders")
    (SYNONYM GENDERS)
    (FLAGS TAKEBIT)>


<OBJECT ODDPERSON
    (IN ERROR-ROOM)
    (DESC "Oddperson")
    (SYNONYM ODDPERSON)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT ODDPERSON2
    (IN ERROR-ROOM)
    (DESC "Oddperson")
    (SYNONYM ODDPERSON)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT ODDPERSON3
    (IN ERROR-ROOM)
    (DESC "Oddperson")
    (SYNONYM ODDPERSON)
    (FLAGS TAKEBIT VOWELBIT)>


<OBJECT SINGPLUR
    (IN ERROR-ROOM)
    (DESC "Singplur")
    (SYNONYM SINGPLUR)
    (FLAGS TAKEBIT)>

