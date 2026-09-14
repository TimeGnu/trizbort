-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "Doors Test" by Trizbort Qt port
The West Room isa location Name 'West Room'
  Description ""
  Exit North to North Room.
  End exit.
  Exit East to East Room.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The West Room.

The hero Isa actor at West Room
End The Hero.

The East Room isa location Name 'East Room'
  Description ""
  Exit West to West Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The East Room.

The North Room isa location Name 'North Room'
  Description ""
  Exit South to West Room.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The North Room.

