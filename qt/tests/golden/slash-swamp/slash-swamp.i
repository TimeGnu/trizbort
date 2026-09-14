-- Alan currently does not process metadata such as title, author, or description, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "Slasher Swamp map" by Andrew Schultz
-- description: This is a map for Slasher Swamp, an IFComp 2014 entry. The dotted lines indicate places you need to search, and in what order.

The Road isa location Name 'Road'
  Description ""
  Exit South to Muddy Swamp.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Road.

The TRUCK 1 isa thing at Road.
  IsDisplayedAs TRUCK (1).
End The TRUCK 1.

The Pond isa location Name 'Pond'
  Description ""
  Exit East to Swamp Forest.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pond.

The Muddy Swamp isa location Name 'Muddy Swamp'
  Description ""
  Exit North to Road.
  End exit.
  Exit South to Rotting Wooden Walkway.
  End exit.
  Exit East to Concrete Building.
  End exit.
  Exit West to Mangrove Forest.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Muddy Swamp.

The Empty Florida Highway isa location Name 'Empty Florida Highway'
  Description ""
  Exit North to Swamp Forest.
  End exit.
  Exit South to Concrete Building.
  End exit.
  Exit East to Gravel Road.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Empty Florida Highway.

The Blacktop Highway isa location Name 'Blacktop Highway'
  Description ""
  Exit South to Mangrove Forest.
  End exit.
  Exit West to Creepy Highway.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Blacktop Highway.

The Bamboo Forest isa location Name 'Bamboo Forest'
  Description ""
  Exit North to Dark Grove.
  End exit.
  Exit West to Dry Everglades.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bamboo Forest.

The Swamp Forest isa location Name 'Swamp Forest'
  Description ""
  Exit North to Pine Forest.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Empty Florida Highway.
  End exit.
  Exit East to Oak Swamp.
  End exit.
  Exit West to Pond.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Swamp Forest.

The BRIAR PATCH 5 isa thing at Swamp Forest.
  IsDisplayedAs BRIAR PATCH (5).
End The BRIAR PATCH 5.

The Dry Everglades isa location Name 'Dry Everglades*'
  Description ""
  Exit North to Front Porch.
  End exit.
  Exit South to Creepy Highway.
  End exit.
  Exit East to Bamboo Forest.
  End exit.
  Exit West to Palmetto Flat.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dry Everglades.

The Dark Grove isa location Name 'Dark Grove*'
  Description ""
  Exit North to South Bank of a Bayou.
  End exit.
  Exit South to Bamboo Forest.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dark Grove.

The E to swamp forest isa thing at Dark Grove.
  IsDisplayedAs E to swamp forest.
End The E to swamp forest.

The Creepy Highway isa location Name 'Creepy Highway'
  Description ""
  Exit North to Dry Everglades.
  End exit.
  Exit South to Scrub Forest.
  End exit.
  Exit East to Blacktop Highway.
  End exit.
  Exit West to Desolate Highway.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Creepy Highway.

The Mangrove Forest isa location Name 'Mangrove Forest'
  Description ""
  Exit North to Blacktop Highway.
  End exit.
  Exit East to Muddy Swamp.
  End exit.
  Exit West to Scrub Forest.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mangrove Forest.

The Desolate Highway isa location Name 'Desolate Highway'
  Description ""
  Exit North to Palmetto Flat.
  End exit.
  Exit South to Pine Flatwood.
  End exit.
  Exit East to Creepy Highway.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Desolate Highway.

The Scrub Forest isa location Name 'Scrub Forest'
  Description ""
  Exit North to Creepy Highway.
  End exit.
  Exit East to Mangrove Forest.
  End exit.
  Exit West to Pine Flatwood.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scrub Forest.

The Pine Flatwood isa location Name 'Pine Flatwood*'
  Description ""
  Exit North to Desolate Highway.
  End exit.
  Exit East to Scrub Forest.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Flatwood.

The WS to swamp isa thing at Pine Flatwood.
  IsDisplayedAs W/S to swamp.
End The WS to swamp.

The Rotting Wooden Walkway isa location Name 'Rotting Wooden Walkway'
  Description ""
  Exit North to Muddy Swamp.
  End exit.
  Exit South to Sandy Grass.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rotting Wooden Walkway.

The Concrete Building isa location Name 'Concrete Building'
  Description ""
  Exit North to Empty Florida Highway.
  End exit.
  Exit South to Dark Concrete Building.
  End exit.
  Exit East to Concrete Slab.
  End exit.
  Exit West to Muddy Swamp.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Concrete Building.

The Sandy Grass isa location Name 'Sandy Grass'
  Description ""
  Exit North to Rotting Wooden Walkway.
  End exit.
  Exit South to Dusty Clearing.
  End exit.
  Exit East to Dusty Clearing.
  End exit.
  Exit West to Gazebo.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sandy Grass.

The King of Spades isa thing at Sandy Grass.
  IsDisplayedAs King of Spades.
End The King of Spades.

The Gazebo isa location Name 'Gazebo'
  Description ""
  Exit East to Sandy Grass.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gazebo.

The Queen of Clubs isa thing at Gazebo.
  IsDisplayedAs Queen of Clubs.
End The Queen of Clubs.

The Dusty Clearing isa location Name 'Dusty Clearing'
  Description ""
  Exit North to Sandy Grass.
  End exit.
  Exit East to Gate.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dusty Clearing.

The Gate isa location Name 'Gate'
  Description ""
  Exit East to Garden.
  End exit.
  Exit West to Dusty Clearing.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gate.

The Garden isa location Name 'Garden'
  Description ""
  Exit North to Garden2.
  End exit.
  Exit East to Concrete Ruins.
  End exit.
  Exit West to Gate.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Garden.

The Concrete Ruins isa location Name 'Concrete Ruins'
  Description ""
  Exit North to Rose Bush.
  End exit.
  Exit West to Garden.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Concrete Ruins.

The Jack of Spades isa thing at Concrete Ruins.
  IsDisplayedAs Jack of Spades.
End The Jack of Spades.

The Rose Bush isa location Name 'Rose Bush'
  Description ""
  Exit South to Concrete Ruins.
  End exit.
  Exit West to Garden2.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rose Bush.

The Garden2 isa location Name 'Garden'
  Description ""
  Exit South to Garden.
  End exit.
  Exit East to Rose Bush.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Garden2.

The Dark Concrete Building isa location Name 'Dark Concrete Building'
  Description ""
  Exit North to Concrete Building.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The Dark Concrete Building.

The Deer doll head isa thing at Dark Concrete Building.
  IsDisplayedAs Deer (doll head).
End The Deer doll head.

The Concrete Slab isa location Name 'Concrete Slab'
  Description ""
  Exit North to Gravel Road.
  End exit.
  Exit South to Graveyard.
  End exit.
  Exit East to Bush Thicket.
  End exit.
  Exit West to Concrete Building.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Concrete Slab.

The Gravel Road isa location Name 'Gravel Road'
  Description ""
  Exit North to Oak Swamp.
  End exit.
  Exit South to Bush Thicket.
  End exit.
  Exit East to Gas Station Parking Lot.
  End exit.
  Exit West to Empty Florida Highway.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gravel Road.

The Bush Thicket isa location Name 'Bush Thicket'
  Description ""
  Exit North to Gravel Road.
  End exit.
  Exit East to Meadow.
  End exit.
  Exit West to Concrete Slab.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bush Thicket.

The S to Swamp Forest isa thing at Bush Thicket.
  IsDisplayedAs S to Swamp Forest.
End The S to Swamp Forest.

The Oak Swamp isa location Name 'Oak Swamp'
  Description ""
  Exit North to Swamp Clearing E to grass field.
  End exit.
  Exit South to Gravel Road.
  End exit.
  Exit West to Swamp Forest.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Oak Swamp.

The E to Swamp Forest2 isa thing at Oak Swamp.
  IsDisplayedAs E to Swamp Forest.
End The E to Swamp Forest2.

The Palmetto Flat isa location Name 'Palmetto Flat'
  Description ""
  Exit North to Dusty Hill.
  End exit.
  Exit South to Desolate Highway.
  End exit.
  Exit East to Dry Everglades.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Palmetto Flat.

The W to Swamp Forest isa thing at Palmetto Flat.
  IsDisplayedAs W to Swamp Forest.
End The W to Swamp Forest.

The Dusty Hill isa location Name 'Dusty Hill*'
  Description ""
  Exit North to South Bank of a Bayou.
  End exit.
  Exit South to Palmetto Flat.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dusty Hill.

The Front Porch isa location Name 'Front Porch'
  Description ""
  Exit North to Living Room.
  End exit.
  Exit South to Dry Everglades.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Front Porch.

The Living Room isa location Name 'Living Room'
  Description ""
  Exit North to Kitchen.
  End exit.
  Exit South to Front Porch.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The Living Room.

The Kitchen isa location Name 'Kitchen'
  Description ""
  Exit North to South Bank of a Bayou.
  End exit.
  Exit South to Living Room.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Kitchen.

The SINK 2 isa thing at Kitchen.
  IsDisplayedAs SINK (2).
End The SINK 2.

The South Bank of a Bayou isa location Name 'South Bank of a Bayou*'
  Description ""
  Exit South to Kitchen.
  End exit.
  Exit East to Dark Grove.
  End exit.
  Exit West to Dusty Hill.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The South Bank of a Bayou.

The Meadow isa location Name 'Meadow*'
  Description ""
  Exit North to Gravel Road.
  End exit.
  Exit South to Palmetto Clearing.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Meadow.

The Swamp Clearing E to grass field isa location Name 'Swamp Clearing (E to grass field)'
  Description ""
  Exit South to Oak Swamp.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Swamp Clearing E to grass field.

The NW to Swamp Forest isa thing at Swamp Clearing E to grass field.
  IsDisplayedAs N/W to Swamp Forest.
End The NW to Swamp Forest.

The Palmetto Clearing isa location Name 'Palmetto Clearing'
  Description ""
  Exit North to Meadow.
  End exit.
  Exit South to Palmetto Field.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Palmetto Clearing.

The WE to Swamp Forest isa thing at Palmetto Clearing.
  IsDisplayedAs W/E to Swamp Forest.
End The WE to Swamp Forest.

The Palmetto Field isa location Name 'Palmetto Field'
  Description ""
  Exit North to Palmetto Clearing.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to In Front of a Straw Hut1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Palmetto Field.

The WE to Swamp Forest2 isa thing at Palmetto Field.
  IsDisplayedAs W/E to Swamp Forest.
End The WE to Swamp Forest2.

The In Front of a Straw Hut1 isa location Name 'In Front of a Straw Hut'
  Description ""
  Exit North to Palmetto Field.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Straw Hut.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Front of a Straw Hut1.

The Straw Hut isa location Name 'Straw Hut'
  Description ""
  Exit North to In Front of a Straw Hut1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Straw Hut.

The SHACK11 isa thing at Straw Hut.
  IsDisplayedAs SHACK(11).
End The SHACK11.

The Need severed head and revolver not man wrevolver1 isa thing at Straw Hut.
  IsDisplayedAs Need severed head and revolver (not man w/revolver).
End The Need severed head and revolver not man wrevolver1.

The Pine Forest isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest2.
  End exit.
  Exit South to Swamp Forest.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Pine Forest6.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest.

The Pine Forest2 isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest3.
  End exit.
  Exit South to Pine Forest.
  End exit.
  Exit East to Pine Forest5.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest2.

The Pine Forest3 isa location Name 'Pine Forest'
  Description ""
  Exit South to Pine Forest2.
  End exit.
  Exit East to Pine Forest4.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest3.

The Pine Forest4 isa location Name 'Pine Forest'
  Description ""
  Exit South to Pine Forest5.
  End exit.
  Exit East to Pine Forest9.
  End exit.
  Exit West to Pine Forest3.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest4.

The Pine Forest5 isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest4.
  End exit.
  Exit South to Pine Forest6.
  End exit.
  Exit East to Pine Forest8.
  End exit.
  Exit West to Pine Forest2.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest5.

The Pine Forest6 isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest5.
  End exit.
  Exit East to Pine Forest7.
  End exit.
  Exit West to Pine Forest.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest6.

The Pine Forest7 isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest8.
  End exit.
  Exit West to Pine Forest6.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest7.

The Pine Forest8 isa location Name 'Pine Forest'
  Description ""
  Exit North to Pine Forest9.
  End exit.
  Exit South to Pine Forest7.
  End exit.
  Exit West to Pine Forest5.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest8.

The Pine Forest9 isa location Name 'Pine Forest'
  Description ""
  Exit South to Pine Forest8.
  End exit.
  Exit East to In Front of Shack1.
  End exit.
  Exit West to Pine Forest4.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pine Forest9.

The In Front of Shack1 isa location Name 'In Front of Shack'
  Description ""
  Exit North to In Shack1.
  End exit.
  Exit West to Pine Forest9.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Front of Shack1.

The In Shack1 isa location Name 'In Shack'
  Description ""
  Exit North to Altar.
  End exit.
  Exit South to In Front of Shack1.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Shack1.

The Altar isa location Name 'Altar'
  Description ""
  Exit South to In Shack1.
  End exit.
  Exit West to Shrine.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Altar.

The ALTAR 6 BODY 7 isa thing at Altar.
  IsDisplayedAs ALTAR (6), BODY (7).
End The ALTAR 6 BODY 7.

The Graveyard isa location Name 'Graveyard'
  Description ""
  Exit North to Concrete Slab.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Graveyard.

The GRAVE 9 need shovel isa thing at Graveyard.
  IsDisplayedAs GRAVE (9) (need shovel).
End The GRAVE 9 need shovel.

The COFFIN wiron key MAN10 isa thing at Graveyard.
  IsDisplayedAs COFFIN w/iron key, MAN(10).
End The COFFIN wiron key MAN10.

The Gas Station Parking Lot isa location Name 'Gas Station Parking Lot'
  Description ""
  Exit North to Alley North of Gas Station.
  End exit.
  Exit South to South of the Gas Station.
  End exit.
  Exit East to Inside Gas Station1.
  End exit.
  Exit West to Gravel Road.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gas Station Parking Lot.

The Alley North of Gas Station isa location Name 'Alley North of Gas Station'
  Description ""
  Exit East to Gas Station Dumpster.
  End exit.
  Exit West to Gas Station Parking Lot.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Alley North of Gas Station.

The South of the Gas Station isa location Name 'South of the Gas Station'
  Description ""
  Exit East to Gas Station Dumpster.
  End exit.
  Exit West to Gas Station Parking Lot.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The South of the Gas Station.

The Gas Station Dumpster isa location Name 'Gas Station Dumpster'
  Description ""
  Exit North to Alley North of Gas Station.
  End exit.
  Exit South to South of the Gas Station.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gas Station Dumpster.

The DUMPSTER 3 isa thing at Gas Station Dumpster.
  IsDisplayedAs DUMPSTER (3).
End The DUMPSTER 3.

The E Swamp Forest isa thing at Gas Station Dumpster.
  IsDisplayedAs E: Swamp Forest.
End The E Swamp Forest.

The Inside Gas Station1 isa location Name 'Inside Gas Station'
  Description ""
  Exit North to Gas Station Office.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Gas Station Parking Lot.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Inside Gas Station1.

The GARBAGE isa thing at Inside Gas Station1.
  IsDisplayedAs GARBAGE.
End The GARBAGE.

The Gas Station Office isa location Name 'Gas Station Office'
  Description ""
  Exit South to Inside Gas Station1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The Gas Station Office.

The BLANKET 4 isa thing at Gas Station Office.
  IsDisplayedAs BLANKET (4).
End The BLANKET 4.

The Shrine isa location Name 'Shrine'
  Description ""
  Exit East to Altar.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The Shrine.

The Head needed to win isa thing at Shrine.
  IsDisplayedAs Head (needed to win).
End The Head needed to win.

The Grass Field isa location Name 'Grass Field'
  Description ""
  Exit North to Clover Field.
  End exit.
  Exit South to Sunflower Field.
  End exit.
  Exit West to Flower Field.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Grass Field.

The W to flower isa thing at Grass Field.
  IsDisplayedAs (W to flower).
End The W to flower.

The Clover Field isa location Name 'Clover Field'
  Description ""
  Exit South to Muck Field.
  End exit.
  Exit East to Flower Field.
  End exit.
  Exit West to Grass Field.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clover Field.

The N to Swamp isa thing at Clover Field.
  IsDisplayedAs N to Swamp.
End The N to Swamp.

The Muck Field isa location Name 'Muck Field'
  Description ""
  Exit North to In Front of an Outhouse1.
  End exit.
  Exit South to Flower Field.
  End exit.
  Exit East to Muck Field.
  End exit.
  Exit West to Clover Field.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Muck Field.

The E to grass isa thing at Muck Field.
  IsDisplayedAs (E to grass).
End The E to grass.

The Sunflower Field isa location Name 'Sunflower Field'
  Description ""
  Exit North to Grass Field.
  End exit.
  Exit South to Flower Field.
  End exit.
  Exit East to Flower Field.
  End exit.
  Exit West to Sunflower Field.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sunflower Field.

The Flower Field isa location Name 'Flower Field'
  Description ""
  Exit South to Clover Field.
  End exit.
  Exit East to Muck Field.
  End exit.
  Exit West to Sunflower Field.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Flower Field.

The W to grass isa thing at Flower Field.
  IsDisplayedAs (W to grass).
End The W to grass.

The In Front of an Outhouse1 isa location Name 'In Front of an Outhouse'
  Description ""
  Exit North to Outhouse.
  End exit.
  Exit South to Muck Field.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Front of an Outhouse1.

The Outhouse isa location Name 'Outhouse'
  Description ""
  Exit South to In Front of an Outhouse1.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Outhouse.

The HOLE 8 isa thing at Outhouse.
  IsDisplayedAs HOLE (8).
End The HOLE 8.

The Swamp-1 from various locations isa location Name 'Swamp-1 (from various locations)'
  Description ""
  Exit South to Swamp-2 from various locations.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Swamp-1 from various locations.

The North to isa thing at Swamp-1 from various locations.
  IsDisplayedAs North to.
End The North to.

The Desolate Highway2 isa thing at Swamp-1 from various locations.
  IsDisplayedAs Desolate Highway.
End The Desolate Highway2.

The Swamp-2 from various locations isa location Name 'Swamp-2 (from various locations)'
  Description ""
  Exit South to Swamp-3.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Swamp-2 from various locations.

The Always to isa thing at Swamp-2 from various locations.
  IsDisplayedAs Always to.
End The Always to.

The Swamp-32 isa thing at Swamp-2 from various locations.
  IsDisplayedAs Swamp-3.
End The Swamp-32.

The Swamp-3 isa location Name 'Swamp-3'
  Description ""
  Exit South to Pond.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Swamp-3.

The South to Pond Otherwise to Swamp-2 isa thing at Swamp-3.
  IsDisplayedAs South to Pond, Otherwise to Swamp-2.
End The South to Pond Otherwise to Swamp-2.

