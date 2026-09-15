-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "space-quest-lost-chapter-spaceship" by A Trizbort User
The Engine Room isa location Name 'Engine Room'
  Description ""
  Exit South to Hallway.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Engine Room.

The Hallway isa location Name 'Hallway'
  Description ""
  Exit North to Engine Room.
  End exit.
  Exit West to Entry.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hallway.

The Entry isa location Name 'Entry'
  Description ""
  Exit North to Wires Left.
  End exit.
  Exit East to Hallway.
  End exit.
  Exit West to Pilot Room.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Entry.

The Pilot Room isa location Name 'Pilot Room'
  Description ""
  Exit South to Entry.
  End exit.
  Exit Southwest to Hall.
  End exit.

  Exit North East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pilot Room.

The Wires Left isa location Name 'Wires Left'
  Description ""
  Exit South to Entry.
  End exit.
  Exit East to Wires Right.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wires Left.

The Elevator2 isa location Name 'Elevator'
  Description ""
  Exit South to L2 room.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator2.

The L2 room isa location Name 'L2 room'
  Description ""
  Exit North to Elevator2.
  End exit.
  Exit East to From Pod.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L2 room.

The From Pod isa location Name 'From Pod'
  Description ""
  Exit West to L2 room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The From Pod.

The GET NEW HAIR isa thing at From Pod.
  IsDisplayedAs GET NEW HAIR.
End The GET NEW HAIR.

The L3 hall isa location Name 'L3 hall'
  Description ""
  Exit North to Elevator3.
  End exit.
  Exit East to L3 hall2.
  End exit.
  Exit West to Guards9.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L3 hall.

The Elevator3 isa location Name 'Elevator'
  Description ""
  Exit South to L3 hall.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator3.

The L3 hall2 isa location Name 'L3 hall'
  Description ""
  Exit North to Lookout.
  End exit.
  Exit East to Guards8.
  End exit.
  Exit West to L3 hall.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L3 hall2.

The Lookout isa location Name 'Lookout'
  Description ""
  Exit South to L3 hall2.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lookout.

The Bar isa location Name 'Bar'
  Description ""
  Exit East to Bar LISTEN.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bar.

The Order Frost isa thing at Bar.
  IsDisplayedAs Order Frost/.
End The Order Frost.

The Tormeenian isa thing at Bar.
  IsDisplayedAs Tormeenian.
End The Tormeenian.

The Bar LISTEN isa location Name 'Bar LISTEN'
  Description ""
  Exit South to L4 Hall.
  End exit.
  Exit East to Bar2.
  End exit.
  Exit West to Bar.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bar LISTEN.

The Bar2 isa location Name 'Bar'
  Description ""
  Exit West to Bar LISTEN.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bar2.

The trade stone isa thing at Bar2.
  IsDisplayedAs trade stone.
End The trade stone.

The for clearance isa thing at Bar2.
  IsDisplayedAs for clearance.
End The for clearance.

The L4 Hall isa location Name 'L4 Hall'
  Description ""
  Exit North to Bar LISTEN.
  End exit.
  Exit East to Guards.
  End exit.
  Exit West to L4 Hall2.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L4 Hall.

The Guards isa location Name 'Guards'
  Description ""
  Exit West to L4 Hall.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards.

The L4 Hall2 isa location Name 'L4 Hall'
  Description ""
  Exit North to Elevator4.
  End exit.
  Exit East to L4 Hall.
  End exit.
  Exit West to Guards2.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L4 Hall2.

The Elevator4 isa location Name 'Elevator'
  Description ""
  Exit South to L4 Hall2.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator4.

The Guards2 isa location Name 'Guards'
  Description ""
  Exit East to L4 Hall2.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards2.

The Elevator5 isa location Name 'Elevator'
  Description ""
  Exit South to L5 hall.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator5.

The L5 hall isa location Name 'L5 hall'
  Description ""
  Exit North to Elevator5.
  End exit.
  Exit East to Guards4.
  End exit.
  Exit West to Guards3.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L5 hall.

The Guards3 isa location Name 'Guards'
  Description ""
  Exit East to L5 hall.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards3.

The Guards4 isa location Name 'Guards'
  Description ""
  Exit West to L5 hall.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards4.

The L6 hall isa location Name 'L6 hall'
  Description ""
  Exit North to Science Room.
  End exit.
  Exit East to L6 hall2.
  End exit.
  Exit West to Guards6.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L6 hall.

The Science Room isa location Name 'Science Room'
  Description ""
  Exit South to L6 hall.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science Room.

The L6 hall2 isa location Name 'L6 hall'
  Description ""
  Exit North to Elevator6.
  End exit.
  Exit East to L6 hall3.
  End exit.
  Exit West to L6 hall.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L6 hall2.

The L6 hall3 isa location Name 'L6 hall'
  Description ""
  Exit North to Elevator6.
  End exit.
  Exit East to Guards7.
  End exit.
  Exit West to L6 hall2.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L6 hall3.

The Elevator6 isa location Name 'Elevator'
  Description ""
  Exit South to L6 hall2.
  End exit.
  Exit East to L6 hall3.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator6.

The Science isa location Name 'Science'
  Description ""
  Exit East to Science2.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science.

The Science2 isa location Name 'Science'
  Description ""
  Exit South to L7 hall.
  End exit.
  Exit West to Science.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science2.

The L7 hall isa location Name 'L7 hall'
  Description ""
  Exit North to Science2.
  End exit.
  Exit East to L7 hall2.
  End exit.
  Exit West to Guards10.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L7 hall.

The L7 hall2 isa location Name 'L7 hall'
  Description ""
  Exit North to Elevator7.
  End exit.
  Exit East to Guards11.
  End exit.
  Exit West to L7 hall.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L7 hall2.

The Elevator7 isa location Name 'Elevator'
  Description ""
  Exit South to L7 hall2.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator7.

The Elevator8 isa location Name 'Elevator'
  Description ""
  Exit South to Hall L10.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator8.

The Hall L10 isa location Name 'Hall L10'
  Description ""
  Exit North to Elevator8.
  End exit.
  Exit East to Hall L102.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall L10.

The Hall L102 isa location Name 'Hall L10'
  Description ""
  Exit North to WAIT  LISTEN.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Guards12.
  End exit.
  Exit West to Hall L10.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall L102.

The WAIT  LISTEN isa location Name 'WAIT & LISTEN'
  Description ""
  Exit South to Hall L102.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The WAIT  LISTEN.

The Elevator9 isa location Name 'Elevator'
  Description ""
  Exit South to Guards L8.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator9.

The Elevator10 isa location Name 'Elevator'
  Description ""
  Exit South to Guards L9.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator10.

The Elevator11 isa location Name 'Elevator'
  Description ""
  Exit South to Hall L11.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator11.

The Hall L11 isa location Name 'Hall L11'
  Description ""
  Exit North to Elevator11.
  End exit.
  Exit East to Hall L112.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall L11.

The Hall L112 isa location Name 'Hall L11'
  Description ""
  Exit North to Science room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Guards13.
  End exit.
  Exit West to Hall L11.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall L112.

The Science room2 isa location Name 'Science room'
  Description ""
  Exit South to Hall L112.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Bomb Room.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science room2.

The Bomb Room isa location Name 'Bomb Room'
  Description ""
  Exit East to Science room2.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Bomb Room.

The Docking Bay isa location Name 'Docking Bay'
  Description ""
  Exit East to Controls L12.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Docking Bay.

The LISTEN isa thing at Docking Bay.
  IsDisplayedAs LISTEN.
End The LISTEN.

The Controls L12 isa location Name 'Controls L12'
  Description ""
  Exit North to Elevator12.
  End exit.
  Exit West to Docking Bay.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Controls L12.

The Elevator12 isa location Name 'Elevator'
  Description ""
  Exit South to Controls L12.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator12.

The Wires Right isa location Name 'Wires Right'
  Description ""
  Exit West to Wires Left.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wires Right.

The POD isa thing at Wires Right.
  IsDisplayedAs POD.
End The POD.

The Lever room isa location Name 'Lever room'
  Description ""
  Exit North to Engine Room.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lever room.

The Hall isa location Name 'Hall'
  Description ""
  Exit East to Fight 2 Aliens.
  End exit.
  Exit Northwest to Pilot Room.
  End exit.

  Exit North South West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall.

The Fight 2 Aliens isa location Name 'Fight 2 Aliens'
  Description ""
  Exit East to Hall2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Hall.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fight 2 Aliens.

The Hall2 isa location Name 'Hall'
  Description ""
  Exit South to Hall3.
  End exit.
  Exit West to Fight 2 Aliens.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall2.

The Hall3 isa location Name 'Hall'
  Description ""
  Exit North to Hall2.
  End exit.
  Exit East to SUIT HELMET.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall3.

The SUIT HELMET isa location Name 'SUIT HELMET'
  Description ""
  Exit West to Hall3.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The SUIT HELMET.

The Gun Compart ment isa location Name 'Gun Compart ment'
  Description ""
  Exit North to Pilot Room.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gun Compart ment.

The GUN isa thing at Gun Compart ment.
  IsDisplayedAs GUN.
End The GUN.

The Guards5 isa location Name 'Guards'
  Description ""
  Exit South to L1 hall.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards5.

The L1 hall isa location Name 'L1 hall'
  Description ""
  Exit East to L1 hall2.
  End exit.
  Exit West to Guards5.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L1 hall.

The L1 hall2 isa location Name 'L1 hall'
  Description ""
  Exit North to Elevator13.
  End exit.
  Exit East to L1 hall3.
  End exit.
  Exit West to L1 hall.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L1 hall2.

The Elevator13 isa location Name 'Elevator'
  Description ""
  Exit South to L1 hall2.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Elevator13.

The L1 hall3 isa location Name 'L1 hall'
  Description ""
  Exit North to Science3.
  End exit.
  Exit West to L1 hall2.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The L1 hall3.

The Science3 isa location Name 'Science'
  Description ""
  Exit South to L1 hall3.
  End exit.
  Exit West to Science4.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science3.

The Science4 isa location Name 'Science'
  Description ""
  Exit East to Science3.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Science4.

The Guards6 isa location Name 'Guards'
  Description ""
  Exit East to L6 hall.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards6.

The Guards7 isa location Name 'Guards'
  Description ""
  Exit West to L6 hall3.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards7.

The Guards8 isa location Name 'Guards'
  Description ""
  Exit West to L3 hall2.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards8.

The Guards9 isa location Name 'Guards'
  Description ""
  Exit East to L3 hall.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards9.

The Guards10 isa location Name 'Guards'
  Description ""
  Exit East to L7 hall.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards10.

The Guards11 isa location Name 'Guards'
  Description ""
  Exit West to L7 hall2.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards11.

The Guards L8 isa location Name 'Guards L8'
  Description ""
  Exit North to Elevator9.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards L8.

The Guards L9 isa location Name 'Guards L9'
  Description ""
  Exit North to Elevator10.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards L9.

The Guards12 isa location Name 'Guards'
  Description ""
  Exit West to Hall L102.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards12.

The Guards13 isa location Name 'Guards'
  Description ""
  Exit West to Hall L112.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Guards13.

