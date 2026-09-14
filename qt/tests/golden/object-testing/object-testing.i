-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "object-testing" by A Trizbort User
The Test Room isa location Name 'Test Room'
  Description ""
  Exit East to cave.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Test Room.

The Mr Jones isa actor at Test Room.
  IsDisplayedAs Mr Jones.
End The Mr Jones.

The Hat isa thing at Test Room.
  IsDisplayedAs Hat.
End The Hat.

The Hatband isa thing at Test Room.
  IsDisplayedAs Hatband.
End The Hatband.

The fishbowl isa thing at Test Room.
  IsDisplayedAs fishbowl.
End The fishbowl.

The goldfish isa thing at Test Room.
  IsDisplayedAs goldfish.
End The goldfish.

The Mrs Jones isa actor at Test Room.
  IsDisplayedAs Mrs Jones.
End The Mrs Jones.

The rabbit isa thing at Test Room.
  IsDisplayedAs rabbit.
End The rabbit.

The cave isa location Name 'cave'
  Description ""
  Exit West to Test Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The cave.

