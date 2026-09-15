-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "luster" by A Trizbort User
The Fork isa location Name 'Fork'
  Description ""
  Exit North to Train Crossing.
  End exit.
  Exit South to Meandering Path.
  End exit.
  Exit West to Dirt Path.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fork.

The Return from plain to here isa thing at Fork.
  IsDisplayedAs Return from plain to here.
End The Return from plain to here.

The Train Crossing isa location Name 'Train Crossing'
  Description ""
  Exit North to Field.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Fork.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Train Crossing.

The Dirt Path isa location Name 'Dirt Path'
  Description ""
  Exit South to Sheriff Station Entrance.
  End exit.
  Exit East to Fork.
  End exit.
  Exit Northeast to Gated Path.
  End exit.
  Exit Northwest to Lake Shore.
  End exit.

  Exit North West Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dirt Path.

The Sheriff Station Entrance isa location Name 'Sheriff Station Entrance'
  Description ""
  Exit North to Dirt Path.
  End exit.
  Exit South to Sheriff Station.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Power Room.
  End exit.
  Exit Inside to Sheriff Station.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sheriff Station Entrance.

The Lake Shore isa location Name 'Lake Shore'
  Description ""
  Exit South to Lake Shore2.
  End exit.
  Exit Southeast to Dirt Path.
  End exit.

  Exit North East West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake Shore.

The Lake Shore2 isa location Name 'Lake Shore'
  Description ""
  Exit North to Lake Shore.
  End exit.
  Exit West to Dock.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake Shore2.

The Dock isa location Name 'Dock'
  Description ""
  Exit East to Lake Shore2.
  End exit.
  Exit West to Lake on raft1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dock.

The Raft isa thing at Dock.
  IsDisplayedAs Raft.
End The Raft.

The Lake on raft1 isa location Name 'Lake (on raft)'
  Description ""
  Exit East to Dock.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake on raft1.

The Blue gem fall into lake isa thing at Lake on raft1.
  IsDisplayedAs Blue gem (fall into lake).
End The Blue gem fall into lake.

The Temple Entrance isa location Name 'Temple Entrance'
  Description ""
  Exit West to Statue Room becomes plain.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Temple Entrance.

The Back to dock once you get gem isa thing at Temple Entrance.
  IsDisplayedAs (Back to dock once you get gem).
End The Back to dock once you get gem.

The Statue Room becomes plain isa location Name 'Statue Room (becomes plain)'
  Description ""
  Exit North to Spectator Box.
  End exit.
  Exit South to Armory.
  End exit.
  Exit East to Temple Entrance.
  End exit.
  Exit West to Arena Room.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Statue Room becomes plain.

The Arena Room isa location Name 'Arena Room'
  Description ""
  Exit East to Statue Room becomes plain.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Arena Room.

The Swordsman isa thing at Arena Room.
  IsDisplayedAs Swordsman.
End The Swordsman.

The Spectator Box isa location Name 'Spectator Box'
  Description ""
  Exit South to Statue Room becomes plain.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Spectator Box.

The Ballpoint pen isa thing at Spectator Box.
  IsDisplayedAs Ballpoint pen.
End The Ballpoint pen.

The Armory isa location Name 'Armory'
  Description ""
  Exit North to Statue Room becomes plain.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Armory.

The Gated Path isa location Name 'Gated Path'
  Description ""
  Exit North to Blank Room.
  End exit.
  Exit Southwest to Dirt Path.
  End exit.

  Exit South East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gated Path.

The Blank Room isa location Name 'Blank Room'
  Description ""
  Exit North to Ocean Room.
  End exit.
  Exit South to Gated Path.
  End exit.
  Exit East to Metallic Room.
  End exit.
  Exit Northeast to Space Room.
  End exit.
  Exit Northwest to Rain-Forest Room.
  End exit.

  Exit West Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Blank Room.

The Lever switch it isa thing at Blank Room.
  IsDisplayedAs Lever (switch it).
End The Lever switch it.

The Ocean Room isa location Name 'Ocean Room'
  Description ""
  Exit South to Blank Room.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ocean Room.

The pedestal1 isa thing at Ocean Room.
  IsDisplayedAs pedestal1.
End The pedestal1.

The Blue gem isa thing at Ocean Room.
  IsDisplayedAs (Blue gem).
End The Blue gem.

The Rain-Forest Room isa location Name 'Rain-Forest Room'
  Description ""
  Exit Southeast to Blank Room.
  End exit.

  Exit North South East West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rain-Forest Room.

The pedestal3 isa thing at Rain-Forest Room.
  IsDisplayedAs pedestal3.
End The pedestal3.

The Green gem isa thing at Rain-Forest Room.
  IsDisplayedAs (Green gem).
End The Green gem.

The Space Room isa location Name 'Space Room'
  Description ""
  Exit Southwest to Blank Room.
  End exit.

  Exit North South East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Space Room.

The pedestal2 isa thing at Space Room.
  IsDisplayedAs pedestal2.
End The pedestal2.

The Midnight gem isa thing at Space Room.
  IsDisplayedAs (Midnight gem).
End The Midnight gem.

The Metallic Room isa location Name 'Metallic Room'
  Description ""
  Exit West to Blank Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Metallic Room.

The pedestal4 isa thing at Metallic Room.
  IsDisplayedAs pedestal4.
End The pedestal4.

The Silver gem isa thing at Metallic Room.
  IsDisplayedAs (Silver gem).
End The Silver gem.

The Meandering Path isa location Name 'Meandering Path'
  Description ""
  Exit North to Fork.
  End exit.
  Exit South to Dead End.
  End exit.
  Exit West to Rosario Entrance.
  End exit.
  Exit Southeast to Ramen Booth.
  End exit.
  Exit Northwest to Aband Store Front Myers Entrance.
  End exit.

  Exit East Northeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Meandering Path.

The Aband Store Front Myers Entrance isa location Name 'Aband. Store Front Myers Entrance'
  Description ""
  Exit West to Abandoned Shop.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Southeast to Meandering Path.
  End exit.

  Exit North South East Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Aband Store Front Myers Entrance.

The Mat steel key isa thing at Aband Store Front Myers Entrance.
  IsDisplayedAs Mat (steel key).
End The Mat steel key.

The Myers Entrance isa thing at Aband Store Front Myers Entrance.
  IsDisplayedAs Myers Entrance.
End The Myers Entrance.

The Abandoned Shop isa location Name 'Abandoned Shop'
  Description ""
  Exit East to Aband Store Front Myers Entrance.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Abandoned Shop.

The Cash RegisterDime isa thing at Abandoned Shop.
  IsDisplayedAs Cash Register/Dime.
End The Cash RegisterDime.

The Rosario Entrance isa location Name 'Rosario Entrance'
  Description ""
  Exit East to Meandering Path.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rosario Entrance.

The Dead End isa location Name 'Dead End'
  Description ""
  Exit North to Meandering Path.
  End exit.
  Exit South to Timber Path.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dead End.

The Rectangle put mat isa thing at Dead End.
  IsDisplayedAs Rectangle (put mat).
End The Rectangle put mat.

The Ramen Booth isa location Name 'Ramen Booth'
  Description ""
  Exit West to Inside Ramen Booth1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Northwest to Meandering Path.
  End exit.

  Exit North South East Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ramen Booth.

The Inside Ramen Booth1 isa location Name 'Inside Ramen Booth'
  Description ""
  Exit East to Ramen Booth.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Inside Ramen Booth1.

The Bulb untie from rope isa thing at Inside Ramen Booth1.
  IsDisplayedAs Bulb (untie from rope).
End The Bulb untie from rope.

The Need steel key isa thing at Inside Ramen Booth1.
  IsDisplayedAs Need steel key.
End The Need steel key.

The Timber Path isa location Name 'Timber Path'
  Description ""
  Exit North to Dead End.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Forest Maze.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Timber Path.

The Note isa thing at Timber Path.
  IsDisplayedAs Note:.
End The Note.

The  forest room names are random1 isa thing at Timber Path.
  IsDisplayedAs * forest room names are random..
End The  forest room names are random1.

The  Light green indicates wrapping maze isa thing at Timber Path.
  IsDisplayedAs * Light green indicates wrapping maze..
End The  Light green indicates wrapping maze.

The  Diagonal directions are possible but ignored1 isa thing at Timber Path.
  IsDisplayedAs * Diagonal directions are possible but ignored..
End The  Diagonal directions are possible but ignored1.

The We just want a way through isa thing at Timber Path.
  IsDisplayedAs We just want a way through..
End The We just want a way through.

The Forest Maze isa location Name 'Forest Maze'
  Description ""
  Exit North to Timber Path.
  End exit.
  Exit South to Forest Maze2.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze.

The Forest Maze2 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Maze.
  End exit.
  Exit South to Forest Center.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze2.

The Forest Center isa location Name 'Forest Center'
  Description ""
  Exit North to Forest Maze2.
  End exit.
  Exit South to Forest Maze6.
  End exit.
  Exit East to Forest Maze8.
  End exit.
  Exit West to Forest Maze3.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Center.

The Forest Maze3 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Maze4.
  End exit.
  Exit South to Forest Maze5.
  End exit.
  Exit East to Forest Center.
  End exit.
  Exit West to Forest Maze4.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze3.

The Forest Maze4 isa location Name 'Forest Maze'
  Description ""
  Exit South to Forest Maze9.
  End exit.
  Exit East to Forest Maze3.
  End exit.
  Exit West to Forest Center2.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze4.

The Forest Center2 isa location Name '(Forest Center)'
  Description ""
  Exit East to Forest Maze4.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Center2.

The Forest Maze5 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Maze3.
  End exit.
  Exit South to Forest Maze7.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze5.

The Forest Maze6 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Center.
  End exit.
  Exit East to Clearing.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze6.

The Clearing isa location Name 'Clearing'
  Description ""
  Exit West to Forest Maze6.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clearing.

The Green Gem2 isa thing at Clearing.
  IsDisplayedAs Green Gem.
End The Green Gem2.

The Forest Maze7 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Maze5.
  End exit.
  Exit South to End of Cave.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze7.

The End of Cave isa location Name 'End of Cave'
  Description ""
  Exit North to Forest Maze7.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The End of Cave.

The Midnight Gem need bulb isa thing at End of Cave.
  IsDisplayedAs Midnight Gem (need bulb).
End The Midnight Gem need bulb.

The Forest Maze8 isa location Name '(Forest Maze)'
  Description ""
  Exit West to Forest Center.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze8.

The Sheriff Station isa location Name 'Sheriff Station'
  Description ""
  Exit North to Sheriff Station Entrance.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Cramped Bathroom.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sheriff Station.

The Need copper key to exit isa thing at Sheriff Station.
  IsDisplayedAs Need copper key to exit.
End The Need copper key to exit.

The Cramped Bathroom isa location Name 'Cramped Bathroom'
  Description ""
  Exit East to Sheriff Station.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cramped Bathroom.

The soap wash soap isa thing at Cramped Bathroom.
  IsDisplayedAs soap (wash soap).
End The soap wash soap.

The Field isa location Name 'Field'
  Description ""
  Exit South to Train Crossing.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Field.

The Cube put coin in slot1 isa thing at Field.
  IsDisplayedAs Cube (put coin in slot).
End The Cube put coin in slot1.

The Makes silver gem isa thing at Field.
  IsDisplayedAs Makes silver gem.
End The Makes silver gem.

The Power Room isa location Name 'Power Room'
  Description ""
  Exit East to Sheriff Station Entrance.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Power Room.

The switch buttons RBI isa thing at Power Room.
  IsDisplayedAs (switch buttons RBI).
End The switch buttons RBI.

The Forest Maze9 isa location Name 'Forest Maze'
  Description ""
  Exit North to Forest Maze4.
  End exit.
  Exit East to Forest Maze4.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest Maze9.

The Tree Canopy isa location Name 'Tree Canopy'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree Canopy.

The Fromto clearing isa thing at Tree Canopy.
  IsDisplayedAs From/to clearing.
End The Fromto clearing.

