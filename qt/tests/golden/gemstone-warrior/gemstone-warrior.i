-- Alan currently does not process metadata such as title, author, or description, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "Gemstone Warrior Trizbort map" by Andrew Schultz
-- description: I set the preferred distance between rooms to 16, and I set regions to .

Verb about
    "Not much"
End Verb about.

The Room 1 isa location Name 'Room 1'
  Description ""
  Exit East to Room 4.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 1.

The Room 4 isa location Name 'Room 4'
  Description ""
  Exit North to Room 0.
  End exit.
  Exit South to Room 5.
  End exit.
  Exit East to Room 6.
  End exit.
  Exit West to Room 1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 4.

The Room 0 isa location Name 'Room 0'
  Description ""
  Exit South to Room 4.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 0.

The Room 5 isa location Name 'Room 5'
  Description ""
  Exit North to Room 4.
  End exit.
  Exit South to Room 3.
  End exit.
  Exit East to Room 6.
  End exit.
  Exit West to Room 2.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 5.

The Room 3 isa location Name 'Room 3'
  Description ""
  Exit North to Room 5.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 3.

The Room 2 isa location Name 'Room 2'
  Description ""
  Exit East to Room 5.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 2.

The Room 6 isa location Name 'Room 6'
  Description ""
  Exit North to Room 4.
  End exit.
  Exit East to Room 7.
  End exit.
  Exit West to Room 5.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 6.

The Room 7 isa location Name 'Room 7'
  Description ""
  Exit North to Room 8.
  End exit.
  Exit West to Room 6.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 7.

The Room 8 isa location Name 'Room 8'
  Description ""
  Exit North to Room 9.
  End exit.
  Exit South to Room 7.
  End exit.
  Exit East to Room 10.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 8.

The Room 10 isa location Name 'Room 10'
  Description ""
  Exit East to Room 102.
  End exit.
  Exit West to Room 8.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 10.

The Room 9 isa location Name 'Room 9'
  Description ""
  Exit North to Room 12.
  End exit.
  Exit South to Room 8.
  End exit.
  Exit East to Room 102.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 9.

The Room 102 isa location Name 'Room 10'
  Description ""
  Exit North to Room 12.
  End exit.
  Exit West to Room 9.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 102.

The Room 12 isa location Name 'Room 12'
  Description ""
  Exit North to Room 15.
  End exit.
  Exit South to Room 102.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 12.

The Room 13 isa location Name 'Room 13'
  Description ""
  Exit North to Room 12.
  End exit.
  Exit East to Room 14.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 13.

The Room 14 isa location Name 'Room 14'
  Description ""
  Exit North to Room 16.
  End exit.
  Exit West to Room 13.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 14.

The Room 16 isa location Name 'Room 16'
  Description ""
  Exit North to Room 15.
  End exit.
  Exit South to Room 14.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 16.

The Room 15 isa location Name 'Room 15'
  Description ""
  Exit South to Room 16.
  End exit.
  Exit East to Room 17.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 15.

The Room 17 isa location Name 'Room 17'
  Description ""
  Exit South to Room 18.
  End exit.
  Exit East to Room 20.
  End exit.
  Exit West to Room 15.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 17.

The Room 18 isa location Name 'Room 18'
  Description ""
  Exit North to Room 17.
  End exit.
  Exit South to Room 19.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 18.

The Room 19 isa location Name 'Room 19'
  Description ""
  Exit North to Room 18.
  End exit.
  Exit South to Room 21.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 19.

The Room 21 isa location Name 'Room 21'
  Description ""
  Exit North to Room 19.
  End exit.
  Exit East to Room 22.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 21.

The Room 22 isa location Name 'Room 22'
  Description ""
  Exit East to Room 20.
  End exit.
  Exit West to Room 21.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 22.

The Room 20 isa location Name 'Room 20'
  Description ""
  Exit East to Room 24.
  End exit.
  Exit West to Room 17.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 20.

The Room 24 isa location Name 'Room 24'
  Description ""
  Exit West to Room 20.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 24.

The Room 25 isa location Name 'Room 25'
  Description ""
  Exit East to Room 24.
  End exit.
  Exit West to Room 23.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 25.

The Room 23 isa location Name 'Room 23'
  Description ""
  Exit South to Room 27.
  End exit.
  Exit East to Room 25.
  End exit.
  Exit West to Room 26.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 23.

The Room 26 isa location Name 'Room 26'
  Description ""
  Exit East to Room 23.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 26.

The Room 27 isa location Name 'Room 27'
  Description ""
  Exit North to Room 23.
  End exit.
  Exit South to Room 28.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 27.

The Room 28 isa location Name 'Room 28'
  Description ""
  Exit North to Room 27.
  End exit.
  Exit South to Room 29.
  End exit.
  Exit East to Room 31.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 28.

The Room 29 isa location Name 'Room 29'
  Description ""
  Exit North to Room 28.
  End exit.
  Exit West to Room 26.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 29.

The Room 31 isa location Name 'Room 31'
  Description ""
  Exit South to Room 34.
  End exit.
  Exit East to Room 32.
  End exit.
  Exit West to Room 28.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 31.

The Room 32 isa location Name 'Room 32'
  Description ""
  Exit West to Room 31.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 32.

The Room 34 isa location Name 'Room 34'
  Description ""
  Exit North to Room 31.
  End exit.
  Exit South to Room 35.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 34.

The Room 33 isa location Name 'Room 33'
  Description ""
  Exit East to Room 32.
  End exit.
  Exit West to Room 38.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 33.

The Room 38 isa location Name 'Room 38'
  Description ""
  Exit South to Room 39.
  End exit.
  Exit East to Room 33.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 38.

The Room 35 isa location Name 'Room 35'
  Description ""
  Exit North to Room 36.
  End exit.
  Exit East to Room 34.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 35.

The Room 36 isa location Name 'Room 36'
  Description ""
  Exit South to Room 35.
  End exit.
  Exit West to Room 37.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 36.

The Room 37 isa location Name 'Room 37'
  Description ""
  Exit East to Room 36.
  End exit.
  Exit West to Room 40.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 37.

The Room 40 isa location Name 'Room 40'
  Description ""
  Exit South to Room 42.
  End exit.
  Exit East to Room 37.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 40.

The Room 42 isa location Name 'Room 42'
  Description ""
  Exit North to Room 40.
  End exit.
  Exit South to Room 41.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 42.

The Room 43 isa location Name 'Room 43'
  Description ""
  Exit North to Room 40.
  End exit.
  Exit South to Room 41.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 43.

The Room 41 isa location Name 'Room 41'
  Description ""
  Exit North to Room 43.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 41.

The Room 69 isa location Name 'Room 69'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 69.

The Room 39 isa location Name 'Room 39'
  Description ""
  Exit North to Room 38.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 39.

The Room 692 isa location Name 'Room 69'
  Description ""
  Exit South to Room 68.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 692.

The Room 68 isa location Name 'Room 68'
  Description ""
  Exit South to Room 67.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 68.

The Room 67 isa location Name 'Room 67'
  Description ""
  Exit North to Room 68.
  End exit.
  Exit South to Room 66.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 67.

The Room 66 isa location Name 'Room 66'
  Description ""
  Exit North to Room 67.
  End exit.
  Exit South to Room 65.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 66.

The Room 65 isa location Name 'Room 65'
  Description ""
  Exit North to Room 66.
  End exit.
  Exit West to Room 64.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 65.

The Room 64 isa location Name 'Room 64'
  Description ""
  Exit North to Room 64.
  End exit.
  Exit South to Room 63.
  End exit.
  Exit East to Room 65.
  End exit.
  Exit West to Room 64.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 64.

The Room 63 isa location Name 'Room 63'
  Description ""
  Exit North to Room 62.
  End exit.
  Exit West to Room 61.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 63.

The Room 56 isa location Name 'Room 56'
  Description ""
  Exit East to Room 58.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 56.

The Room 61 isa location Name 'Room 61'
  Description ""
  Exit North to Room 58.
  End exit.
  Exit East to Room 63.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 61.

The Room 57 isa location Name 'Room 57'
  Description ""
  Exit East to Room 58.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 57.

The Room 58 isa location Name 'Room 58'
  Description ""
  Exit South to Room 61.
  End exit.
  Exit East to Room 59.
  End exit.
  Exit West to Room 57.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 58.

The Room 62 isa location Name 'Room 62'
  Description ""
  Exit South to Room 63.
  End exit.
  Exit East to Room 60.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 62.

The Room 59 isa location Name 'Room 59'
  Description ""
  Exit East to Room 60.
  End exit.
  Exit West to Room 58.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 59.

The Room 60 isa location Name 'Room 60'
  Description ""
  Exit West to Room 59.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 60.

The Room 52 isa location Name 'Room 52'
  Description ""
  Exit South to Room 53.
  End exit.
  Exit East to Room 54.
  End exit.
  Exit West to Room 51.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 52.

The Room 54 isa location Name 'Room 54'
  Description ""
  Exit East to Room 562.
  End exit.
  Exit West to Room 52.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 54.

The Room 53 isa location Name 'Room 53'
  Description ""
  Exit North to Room 52.
  End exit.
  Exit East to Room 55.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 53.

The Room 55 isa location Name 'Room 55'
  Description ""
  Exit East to Room 572.
  End exit.
  Exit West to Room 53.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 55.

The Room 572 isa location Name 'Room 57'
  Description ""
  Exit East to Room 582.
  End exit.
  Exit West to Room 55.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 572.

The Room 582 isa location Name 'Room 58'
  Description ""
  Exit West to Room 572.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 582.

The Room 562 isa location Name 'Room 56'
  Description ""
  Exit East to Room 583.
  End exit.
  Exit West to Room 54.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 562.

The Room 583 isa location Name 'Room 58'
  Description ""
  Exit West to Room 562.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 583.

The Room 72 isa location Name 'Room 7'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 72.

The Room 44 isa location Name 'Room 44'
  Description ""
  Exit West to Room 45.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 44.

The Room 45 isa location Name 'Room 45'
  Description ""
  Exit East to Room 44.
  End exit.
  Exit West to Room 47.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 45.

The Room 46 isa location Name 'Room 46'
  Description ""
  Exit East to Room 44.
  End exit.
  Exit West to Room 48.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 46.

The Room 48 isa location Name 'Room 48'
  Description ""
  Exit East to Room 46.
  End exit.
  Exit West to Room 50.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 48.

The Room 47 isa location Name 'Room 47'
  Description ""
  Exit East to Room 45.
  End exit.
  Exit West to Room 49.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 47.

The Room 49 isa location Name 'Room 49'
  Description ""
  Exit East to Room 47.
  End exit.
  Exit West to Room 50.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 49.

The Room 50 isa location Name 'Room 50'
  Description ""
  Exit East to Room 49.
  End exit.
  Exit West to Room 51.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 50.

The Room 51 isa location Name 'Room 51'
  Description ""
  Exit East to Room 52.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 51.

