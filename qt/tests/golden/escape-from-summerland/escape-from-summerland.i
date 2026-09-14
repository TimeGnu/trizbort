-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "escape-from-summerland" by A Trizbort User
The In the Caravan Park1 isa location Name 'In the Caravan Park'
  Description ""
  Exit South to Main Track.
  End exit.
  Exit Inside to Inside the Caravan1.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Caravan Park1.

The Enter then examine person isa thing at In the Caravan Park1.
  IsDisplayedAs Enter then examine person.
End The Enter then examine person.

The Main Track isa location Name 'Main Track'
  Description ""
  Exit North to In the Caravan Park1.
  End exit.
  Exit East to Beneath the Hedge.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Main Track.

The Beneath the Hedge isa location Name 'Beneath the Hedge'
  Description ""
  Exit East to Drizzle Court.
  End exit.
  Exit West to Main Track.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Beneath the Hedge.

The Jacquotte gone go east or west isa thing at Beneath the Hedge.
  IsDisplayedAs Jacquotte gone, go east or west.
End The Jacquotte gone go east or west.

The Drizzle Court isa location Name 'Drizzle Court'
  Description ""
  Exit East to Orbis Alius.
  End exit.
  Exit West to Beneath the Hedge.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Drizzle Court.

The Orbis Alius isa location Name 'Orbis Alius'
  Description ""
  Exit North to Mirror Maze.
  End exit.
  Exit South to Selkie AquariumAquatic Region.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Up in the Stands1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Drizzle Court.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Orbis Alius.

The CLIMB or PUSH robot isa thing at Orbis Alius.
  IsDisplayedAs CLIMB or PUSH robot.
End The CLIMB or PUSH robot.

The as Jacquotte isa thing at Orbis Alius.
  IsDisplayedAs as Jacquotte.
End The as Jacquotte.

The Mirror Maze isa location Name 'Mirror Maze'
  Description ""
  Exit South to Orbis Alius.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mirror Maze.

The Up in the Stands1 isa location Name 'Up in the Stands'
  Description ""
  Exit West to Orbis Alius.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Down to Big Top West.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up in the Stands1.

The Big Top West isa location Name 'Big Top West'
  Description ""
  Exit East to Big Top East.
  End exit.
  Exit Up to Up in the Stands1.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Big Top West.

The Left unit falls here isa thing at Big Top West.
  IsDisplayedAs Left unit falls here.
End The Left unit falls here.

The Big Top East isa location Name 'Big Top East'
  Description ""
  Exit North to Another High Place.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Backways.
  End exit.
  Exit West to Big Top West.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Big Top East.

The Backways isa location Name 'Backways'
  Description ""
  Exit North to Big Top East.
  End exit.
  Exit East to The Dressing Rooms.
  End exit.
  Exit West to Home Place.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Backways.

The Jacquotte needs light isa thing at Backways.
  IsDisplayedAs Jacquotte needs light.
End The Jacquotte needs light.

The The Dressing Rooms isa location Name 'The Dressing Rooms'
  Description ""
  Exit West to Backways.
  End exit.
  Exit Inside to Going Up Thing skipped by man1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The The Dressing Rooms.

The Home Place isa location Name 'Home Place'
  Description ""
  Exit East to Backways.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Home Place.

The Right Interaction isa thing at Home Place.
  IsDisplayedAs Right Interaction.
End The Right Interaction.

The Civilian Discouragement isa thing at Home Place.
  IsDisplayedAs Civilian Discouragement.
End The Civilian Discouragement.

The Inside the Caravan1 isa location Name 'Inside the Caravan'
  Description ""
  Exit Outside to In the Caravan Park1.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Inside the Caravan1.

The Dead Man isa thing at Inside the Caravan1.
  IsDisplayedAs Dead Man.
End The Dead Man.

The Another High Place isa location Name 'Another High Place'
  Description ""
  Exit South to Big Top East.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Another High Place.

The Shiny Thing Left unit1 isa thing at Another High Place.
  IsDisplayedAs Shiny Thing (Left unit).
End The Shiny Thing Left unit1.

The Jacquottes Cage isa location Name 'Jacquotte's Cage'
  Description ""
  Exit Outside to Home Place.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Jacquottes Cage.

The Going Up Thing skipped by man1 isa location Name 'Going Up Thing (skipped by man)'
  Description ""
  Exit Outside to The Dressing Rooms.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Going Up Thing skipped by man1.

The Skipped by man isa thing at Going Up Thing skipped by man1.
  IsDisplayedAs Skipped by man.
End The Skipped by man.

The Selkie AquariumAquatic Region isa location Name 'Selkie Aquarium/Aquatic Region'
  Description ""
  Exit North to Orbis Alius.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Control Booth.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Selkie AquariumAquatic Region.

The Control Booth isa location Name 'Control Booth'
  Description ""
  Exit East to Service Corridor.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Selkie AquariumAquatic Region.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Control Booth.

The Crank isa thing at Control Booth.
  IsDisplayedAs Crank.
End The Crank.

The Service Corridor isa location Name 'Service Corridor'
  Description ""
  Exit West to Control Booth.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Inside to Going Up Thing skipped by man1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Service Corridor.

The search rubble for plunger isa thing at Service Corridor.
  IsDisplayedAs search rubble for plunger.
End The search rubble for plunger.

The install SUCK DOOR1 isa thing at Service Corridor.
  IsDisplayedAs install, SUCK DOOR.
End The install SUCK DOOR1.

