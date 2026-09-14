#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'Gemstone Warrior Trizbort map'
    byline = 'By Andrew Schultz'
    version = '1'
    desc = 'I set the preferred distance between rooms to 16, and I set regions to '

    showAbout()
    {
    "Not much";
    }
;

Beginning: Region
;

Inward: Region
;

Outward: Region
;

Room1: Room 'Room 1'
    regions = [Beginning]

    east = Room4
;

Room4: Room 'Room 4'
    regions = [Beginning]

    north = Room0
    south = Room5
    east = Room6
    west = Room1
;

Room0: Room 'Room 0'
    regions = [Beginning]

    south = Room4
;

Room5: Room 'Room 5'
    regions = [Beginning]

    north = Room4
    south = Room3
    east = Room6
    west = Room2
;

Room3: Room 'Room 3'
    regions = [Beginning]

    north = Room5
;

Room2: Room 'Room 2'
    regions = [Beginning]

    east = Room5
;

Room6: Room 'Room 6'
    regions = [Beginning]

    north = Room4
    east = Room7
    west = Room5
;

Room7: Room 'Room 7'

    north = Room8
    west = Room6
;

Room8: Room 'Room 8'
    regions = [Inward]

    north = Room9
    south = Room7
    east = Room10
;

Room10: Room 'Room 10'
    regions = [Inward]

    east = Room102
    west = Room8
;

Room9: Room 'Room 9'
    regions = [Inward]

    north = Room12
    south = Room8
    east = Room102
;

Room102: Room 'Room 10'
    regions = [Inward]

    north = Room12
    west = Room9
;

Room12: Room 'Room 12'
    regions = [Inward]

    north = Room15
    south = Room102
;

Room13: Room 'Room 13'
    regions = [Inward]

    north = Room12
    east = Room14
;

Room14: Room 'Room 14'
    regions = [Inward]

    north = Room16
    west = Room13
;

Room16: Room 'Room 16'
    regions = [Inward]

    north = Room15
    south = Room14
;

Room15: Room 'Room 15'
    regions = [Inward]

    south = Room16
    east = Room17
;

Room17: Room 'Room 17'
    regions = [Inward]

    south = Room18
    east = Room20
    west = Room15
;

Room18: Room 'Room 18'
    regions = [Inward]

    north = Room17
    south = Room19
;

Room19: Room 'Room 19'
    regions = [Inward]

    north = Room18
    south = Room21
;

Room21: Room 'Room 21'
    regions = [Inward]

    north = Room19
    east = Room22
;

Room22: Room 'Room 22'
    regions = [Inward]

    east = Room20
    west = Room21
;

Room20: Room 'Room 20'
    regions = [Inward]

    east = Room24
    west = Room17
;

Room24: Room 'Room 24'
    regions = [Inward]

    west = Room20
;

Room25: Room 'Room 25'
    regions = [Inward]

    east = Room24
    west = Room23
;

Room23: Room 'Room 23'
    regions = [Inward]

    south = Room27
    east = Room25
    west = Room26
;

Room26: Room 'Room 26'
    regions = [Inward]

    east = Room23
;

Room27: Room 'Room 27'
    regions = [Inward]

    north = Room23
    south = Room28
;

Room28: Room 'Room 28'
    regions = [Inward]

    north = Room27
    south = Room29
    east = Room31
;

Room29: Room 'Room 29'
    regions = [Inward]

    north = Room28
    west = Room26
;

Room31: Room 'Room 31'
    regions = [Inward]

    south = Room34
    east = Room32
    west = Room28
;

Room32: Room 'Room 32'
    regions = [Inward]

    west = Room31
;

Room34: Room 'Room 34'
    regions = [Inward]

    north = Room31
    south = Room35
;

Room33: Room 'Room 33'
    regions = [Inward]

    east = Room32
    west = Room38
;

Room38: Room 'Room 38'
    regions = [Inward]

    south = Room39
    east = Room33
;

Room35: Room 'Room 35'
    regions = [Inward]

    north = Room36
    east = Room34
;

Room36: Room 'Room 36'
    regions = [Inward]

    south = Room35
    west = Room37
;

Room37: Room 'Room 37'
    regions = [Inward]

    east = Room36
    west = Room40
;

Room40: Room 'Room 40'
    regions = [Inward]

    south = Room42
    east = Room37
;

Room42: Room 'Room 42'
    regions = [Inward]

    north = Room40
    south = Room41
;

Room43: Room 'Room 43'
    regions = [Inward]

    north = Room40
    south = Room41
;

Room41: Room 'Room 41'
    regions = [Inward]

    north = Room43
;

Room69: Room 'Room 69'
;

Room39: Room 'Room 39'
    regions = [Inward]

    north = Room38
;

Room692: Room 'Room 69'

    south = Room68
;

Room68: Room 'Room 68'

    south = Room67
;

Room67: Room 'Room 67'

    north = Room68
    south = Room66
;

Room66: Room 'Room 66'

    north = Room67
    south = Room65
;

Room65: Room 'Room 65'

    north = Room66
    west = Room64
;

Room64: Room 'Room 64'

    north = Room64
    south = Room63
    east = Room65
    west = Room64
;

Room63: Room 'Room 63'

    north = Room62
    west = Room61
;

Room56: Room 'Room 56'

    east = Room58
;

Room61: Room 'Room 61'

    north = Room58
    east = Room63
;

Room57: Room 'Room 57'

    east = Room58
;

Room58: Room 'Room 58'

    south = Room61
    east = Room59
    west = Room57
;

Room62: Room 'Room 62'

    south = Room63
    east = Room60
;

Room59: Room 'Room 59'

    east = Room60
    west = Room58
;

Room60: Room 'Room 60'

    west = Room59
;

Room52: Room 'Room 52'
    regions = [Outward]

    south = Room53
    east = Room54
    west = Room51
;

Room54: Room 'Room 54'
    regions = [Outward]

    east = Room562
    west = Room52
;

Room53: Room 'Room 53'
    regions = [Outward]

    north = Room52
    east = Room55
;

Room55: Room 'Room 55'
    regions = [Outward]

    east = Room572
    west = Room53
;

Room572: Room 'Room 57'
    regions = [Outward]

    east = Room582
    west = Room55
;

Room582: Room 'Room 58'
    regions = [Outward]

    west = Room572
;

Room562: Room 'Room 56'
    regions = [Outward]

    east = Room583
    west = Room54
;

Room583: Room 'Room 58'
    regions = [Outward]

    west = Room562
;

Room72: Room 'Room 7'
;

Room44: Room 'Room 44'
    regions = [Outward]

    west = Room45
;

Room45: Room 'Room 45'
    regions = [Outward]

    east = Room44
    west = Room47
;

Room46: Room 'Room 46'
    regions = [Outward]

    east = Room44
    west = Room48
;

Room48: Room 'Room 48'
    regions = [Outward]

    east = Room46
    west = Room50
;

Room47: Room 'Room 47'
    regions = [Outward]

    east = Room45
    west = Room49
;

Room49: Room 'Room 49'
    regions = [Outward]

    east = Room47
    west = Room50
;

Room50: Room 'Room 50'
    regions = [Outward]

    east = Room49
    west = Room51
;

Room51: Room 'Room 51'
    regions = [Outward]

    east = Room52
;

me: Actor
    location = Room1
;

gameMain: GameMainDef
    initialPlayerChar = me
;

