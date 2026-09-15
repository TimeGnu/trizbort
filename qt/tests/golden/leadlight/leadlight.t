#charset "us-ascii"

#include <tads.h>
#include "advlite.h"

versionInfo : GameID
    name = 'Leadlight Gamma Map'
    byline = 'By Andrew Schultz'
    version = '1'
    desc = 'Map of Wade Clarke\'s 2010 game Leadlight (and its Inform port Leadlight Gamma). Orange=dorm grey=library purple=indoor yellow=inner sanctum red=inner inner sanctum'
;

Dorms: Region
;

Endgame: Region
;

Hidden: Region
;

Library: Region
;

Outside: Region
;

School: Region
;

LinvilleStudentLibrary: Room 'Linville Student Library'
    regions = [Library]

    south = AmongsttheLibraryStacks
    east = IntheLibraryFoyer
    west = LibraryToilet
;

+ CharlotteJeffs: Thing 'Charlotte Jeffs' 'Charlotte Jeffs'
;

LibraryToilet: Room 'Library Toilet'
    regions = [Library]

    east = LinvilleStudentLibrary
;

AmongsttheLibraryStacks: Room 'Amongst the Library Stacks'
    regions = [Library]

    north = LinvilleStudentLibrary
;

IntheLibraryFoyer: Room 'In the Library Foyer'
    regions = [Library]

    east = IntheQuadrangle
    west = LinvilleStudentLibrary
;

+ NarelleParker: Thing 'Narelle Parker' 'Narelle Parker'
;

IntheQuadrangle: Room 'In the Quadrangle'
    regions = [Outside]

    north = OntheSchoolSteps
    south = OntheRoseGardenLawn
    east = IntheDormitoryCourtyard
    west = IntheLibraryFoyer
;

OntheRoseGardenLawn: Room 'On the Rose Garden Lawn'
    regions = [Outside]

    north = IntheQuadrangle
;

+ AlexisKarpa: Thing 'Alexis Karpa' 'Alexis Karpa'
;

IntheDormitoryCourtyard: Room 'In the Dormitory Courtyard'
    regions = [Outside]

    south = IntheGymEntry
    east = IntheDormitoryEntryway
    west = IntheQuadrangle
;

IntheGymEntry: Room 'In the Gym Entry'
    regions = [School]

    north = IntheDormitoryCourtyard
    south = OntheGymCourtNorth
;

OntheGymCourtNorth: Room 'On the Gym Court, North'
    regions = [School]

    north = IntheGymEntry
    south = OntheGymCourtSouth
    up = IntheUpstairsChangingRoom
;

OntheGymCourtSouth: DarkRoom 'On the Gym Court, South'
    regions = [School]

    north = OntheGymCourtNorth
;

IntheDormitoryEntryway: Room 'In the Dormitory Entryway'
    regions = [Dorms]

    north = Room106
    south = Room101
    east = HalfwayAlongtheDormHall
    west = IntheDormitoryCourtyard
;

+ MsPalmerDianeFallon: Thing 'Ms PalmerDiane Fallon' 'Ms PalmerDiane Fallon'
;

Room106: Room 'Room 106'
    regions = [Dorms]

    south = IntheDormitoryEntryway
;

Room101: Room 'Room 101'
    regions = [Dorms]

    north = IntheDormitoryEntryway
;

+ SherylBourke: Thing 'Sheryl Bourke' 'Sheryl Bourke'
;

HalfwayAlongtheDormHall: Room 'Halfway Along the Dorm Hall'
    regions = [Dorms]

    north = Room105
    south = Room102
    east = AttheDormHallsEastEnd
    west = IntheDormitoryEntryway
;

Room105: Room 'Room 105'
    regions = [Dorms]

    south = HalfwayAlongtheDormHall
;

Room102: Room 'Room 102'
    regions = [Dorms]

    north = HalfwayAlongtheDormHall
;

+ Louise: Thing 'Louise' 'Louise'
;

AttheDormHallsEastEnd: Room 'At the Dorm Hall\'s East End'
    regions = [Dorms]

    north = Room104
    south = Room103
    west = HalfwayAlongtheDormHall
;

Room104: Room 'Room 104'
    regions = [Dorms]

    south = AttheDormHallsEastEnd
;

Room103: Room 'Room 103'
    regions = [Dorms]

    north = AttheDormHallsEastEnd
;

OntheSchoolSteps: Room 'On the School Steps'
    regions = [Outside]

    north = IntheSchoolFoyer
    south = IntheQuadrangle
    east = OntheTrellisedPatio
    west = BytheSchoolFountain
;

IntheSchoolFoyer: Room 'In the School Foyer'
    regions = [School]

    north = IntheMarbleStateroom
    south = OntheSchoolSteps
    east = IntheSickBay
    west = IntheReceptionistsOffice
;

OntheTrellisedPatio: Room 'On the Trellised Patio'
    regions = [Outside]

    west = OntheSchoolSteps
;

+ Gardener: Thing 'Gardener' 'Gardener'
;

BytheSchoolFountain: Room 'By the School Fountain'
    regions = [Outside]

    north = IntheArtCottage
    south = InStaffCottageB
    east = OntheSchoolSteps
;

+ MysteryGirl: Thing 'Mystery Girl' 'Mystery Girl'
;

IntheArtCottage: Room 'In the Art Cottage'
    regions = [School]

    south = BytheSchoolFountain
;

+ MrRhodes: Thing 'Mr Rhodes' 'Mr Rhodes'
;

InStaffCottageB: Room 'In Staff Cottage B'
    regions = [School]

    north = BytheSchoolFountain
;

+ FileMissArden: Thing 'File Miss Arden' 'File Miss Arden'
;

IntheReceptionistsOffice: Room 'In the Receptionist\'s Office'
    regions = [School]

    east = IntheSchoolFoyer
;

IntheSickBay: Room 'In the Sick Bay'
    regions = [School]

    west = IntheSchoolFoyer
;

+ UnnamedGirl: Thing 'Unnamed Girl' 'Unnamed Girl'
;

IntheMarbleStateroom: Room 'In the Marble Stateroom'
    regions = [School]

    north = IntheSouthHall
    south = IntheSchoolFoyer
;

+ MaureenSallis: Thing 'Maureen Sallis' 'Maureen Sallis'
;

IntheSouthHall: Room 'In the South Hall'
    regions = [School]

    north = IntheGrandHall
    south = IntheMarbleStateroom
    east = IntheSittingRoom
;

IntheSittingRoom: Room 'In the Sitting Room'
    regions = [School]

    north = IntheEasternHallway
    west = IntheSouthHall
;

+ LucindaParker: Thing 'Lucinda Parker' 'Lucinda Parker'
;

IntheEasternHallway: Room 'In the Eastern Hallway'
    regions = [School]

    north = IntheWaitingRoom
    south = IntheSittingRoom
    east = IntheDanceRoom
    west = IntheGrandHall
;

+ MiaFellini: Thing 'Mia Fellini' 'Mia Fellini'
;

IntheDanceRoom: Room 'In the Dance Room'
    regions = [School]

    west = IntheEasternHallway
;

+ TheDancer: Thing 'The Dancer' 'The Dancer'
;

IntheWaitingRoom: Room 'In the Waiting Room'
    regions = [School]

    north = IntheStaffLibrary
    south = IntheEasternHallway
    west = InScienceLab1C
;

InScienceLab1C: Room 'In Science Lab 1C'
    regions = [School]

    south = IntheGrandHall
    east = IntheWaitingRoom
;

IntheGrandHall: Room 'In the Grand Hall'
    regions = [School]

    north = InScienceLab1C
    south = IntheSouthHall
    east = IntheEasternHallway
    west = IntheSchoolChapel
;

+ MadisonJones: Thing 'Madison Jones' 'Madison Jones'
;

+ KateSeung: Thing 'Kate Seung' 'Kate Seung'
;

IntheSchoolChapel: Room 'In the School Chapel'
    regions = [School]

    east = IntheGrandHall
;

IntheStaffLibrary: Room 'In the Staff Library'
    regions = [Hidden]

    north = IntheLeatherStudy
    south = IntheWaitingRoom
;

IntheLeatherStudy: Room 'In the Leather Study'
    regions = [Hidden]

    south = IntheStaffLibrary
    east = IntheStrangeGallery
    west = InaCurtainedHallway
    up = OntheBalcony
;

IntheStrangeGallery: Room 'In the Strange Gallery'
    regions = [Hidden]

    west = IntheLeatherStudy
;

OntheBalcony: Room 'On the Balcony'
    regions = [Hidden]

    down = IntheLeatherStudy
;

+ Witch: Thing 'Witch' 'Witch'
;

InaCurtainedHallway: Room 'In a Curtained Hallway'
    regions = [Hidden]

    east = IntheLeatherStudy
    west = InaCurtainedHallway2
;

InaCurtainedHallway2: Room 'In a Curtained Hallway'
    regions = [Hidden]

    east = InaCurtainedHallway
    west = AttheOpulentCorner
;

AttheOpulentCorner: Room 'At the Opulent Corner'
    regions = [Hidden]

    north = BeforetheLeadlightDoor
    east = InaCurtainedHallway2
;

+ EveJoshi: Thing 'Eve Joshi' 'Eve Joshi'
;

BeforetheLeadlightDoor: Room 'Before the Leadlight Door'
    regions = [Hidden]

    north = room2
    south = AttheOpulentCorner
;

room2: DarkRoom '???'
    regions = [Endgame]

    north = IntheFoundersRoom
    south = BeforetheLeadlightDoor
;

IntheFoundersRoom: Room 'In the Founder\'s Room'
    regions = [Endgame]

    south = room2
    east = IntheSecretPlace
;

+ Founder: Thing 'Founder' 'Founder'
;

IntheSecretPlace: Room 'In the Secret Place'
    regions = [Endgame]

    west = IntheFoundersRoom
;

+ Shadow: Thing 'Shadow' 'Shadow'
;

+ Founder2: Thing 'Founder' 'Founder'
;

IntheUpstairsChangingRoom: Room 'In the Upstairs Changing Room'
    regions = [School]

    down = OntheGymCourtNorth
;

+ IsabelleGreen: Thing 'Isabelle Green' 'Isabelle Green'
;

me: Actor
    location = LinvilleStudentLibrary
;

gameMain: GameMainDef
    initialPlayerChar = me
;

