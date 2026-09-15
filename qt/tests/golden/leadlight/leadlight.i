-- Alan currently does not process metadata such as title, author, or description, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "Leadlight Gamma Map" by Andrew Schultz
-- description: Map of Wade Clarke's 2010 game Leadlight (and its Inform port Leadlight Gamma). Orange=dorm grey=library purple=indoor yellow=inner sanctum red=inner inner sanctum.

The Linville Student Library isa location Name 'Linville Student Library'
  Description ""
  Exit South to Amongst the Library Stacks.
  End exit.
  Exit East to In the Library Foyer1.
  End exit.
  Exit West to Library Toilet.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Linville Student Library.

The Charlotte Jeffs isa thing at Linville Student Library.
  IsDisplayedAs Charlotte Jeffs.
End The Charlotte Jeffs.

The Library Toilet isa location Name 'Library Toilet'
  Description ""
  Exit East to Linville Student Library.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Library Toilet.

The Amongst the Library Stacks isa location Name 'Amongst the Library Stacks'
  Description ""
  Exit North to Linville Student Library.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Amongst the Library Stacks.

The In the Library Foyer1 isa location Name 'In the Library Foyer'
  Description ""
  Exit East to In the Quadrangle1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Linville Student Library.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Library Foyer1.

The Narelle Parker isa thing at In the Library Foyer1.
  IsDisplayedAs Narelle Parker.
End The Narelle Parker.

The In the Quadrangle1 isa location Name 'In the Quadrangle'
  Description ""
  Exit North to On the School Steps1.
  End exit.
  Exit South to On the Rose Garden Lawn1.
  End exit.
  Exit East to In the Dormitory Courtyard1.
  End exit.
  Exit West to In the Library Foyer1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Quadrangle1.

The On the Rose Garden Lawn1 isa location Name 'On the Rose Garden Lawn'
  Description ""
  Exit North to In the Quadrangle1.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the Rose Garden Lawn1.

The Alexis Karpa isa thing at On the Rose Garden Lawn1.
  IsDisplayedAs Alexis Karpa.
End The Alexis Karpa.

The In the Dormitory Courtyard1 isa location Name 'In the Dormitory Courtyard'
  Description ""
  Exit South to In the Gym Entry1.
  End exit.
  Exit East to In the Dormitory Entryway1.
  End exit.
  Exit West to In the Quadrangle1.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Dormitory Courtyard1.

The In the Gym Entry1 isa location Name 'In the Gym Entry'
  Description ""
  Exit North to In the Dormitory Courtyard1.
  End exit.
  Exit South to On the Gym Court North1.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Gym Entry1.

The On the Gym Court North1 isa location Name 'On the Gym Court, North'
  Description ""
  Exit North to In the Gym Entry1.
  End exit.
  Exit South to On the Gym Court South1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Up to In the Upstairs Changing Room1.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the Gym Court North1.

The On the Gym Court South1 isa location Name 'On the Gym Court, South'
  Description ""
  Exit North to On the Gym Court North1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The On the Gym Court South1.

The In the Dormitory Entryway1 isa location Name 'In the Dormitory Entryway'
  Description ""
  Exit North to Room 106.
  End exit.
  Exit South to Room 101.
  End exit.
  Exit East to Halfway Along the Dorm Hall.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to In the Dormitory Courtyard1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Dormitory Entryway1.

The Ms PalmerDiane Fallon isa thing at In the Dormitory Entryway1.
  IsDisplayedAs Ms Palmer/Diane Fallon.
End The Ms PalmerDiane Fallon.

The Room 106 isa location Name 'Room 106'
  Description ""
  Exit South to In the Dormitory Entryway1.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 106.

The Room 101 isa location Name 'Room 101'
  Description ""
  Exit North to In the Dormitory Entryway1.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 101.

The Sheryl Bourke isa thing at Room 101.
  IsDisplayedAs Sheryl Bourke.
End The Sheryl Bourke.

The Halfway Along the Dorm Hall isa location Name 'Halfway Along the Dorm Hall'
  Description ""
  Exit North to Room 105.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Room 102.
  End exit.
  Exit East to At the Dorm Halls East End.
  End exit.
  Exit West to In the Dormitory Entryway1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Halfway Along the Dorm Hall.

The Room 105 isa location Name 'Room 105'
  Description ""
  Exit South to Halfway Along the Dorm Hall.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 105.

The Room 102 isa location Name 'Room 102'
  Description ""
  Exit North to Halfway Along the Dorm Hall.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 102.

The Louise isa thing at Room 102.
  IsDisplayedAs Louise.
End The Louise.

The At the Dorm Halls East End isa location Name 'At the Dorm Hall's East End'
  Description ""
  Exit North to Room 104.
  End exit.
  Exit South to Room 103.
  End exit.
  Exit West to Halfway Along the Dorm Hall.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The At the Dorm Halls East End.

The Room 104 isa location Name 'Room 104'
  Description ""
  Exit South to At the Dorm Halls East End.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 104.

The Room 103 isa location Name 'Room 103'
  Description ""
  Exit North to At the Dorm Halls East End.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Room 103.

The On the School Steps1 isa location Name 'On the School Steps'
  Description ""
  Exit North to In the School Foyer1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to In the Quadrangle1.
  End exit.
  Exit East to On the Trellised Patio1.
  End exit.
  Exit West to By the School Fountain.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the School Steps1.

The In the School Foyer1 isa location Name 'In the School Foyer'
  Description ""
  Exit North to In the Marble Stateroom1.
  End exit.
  Exit South to On the School Steps1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to In the Sick Bay1.
  End exit.
  Exit West to In the Receptionists Office1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the School Foyer1.

The On the Trellised Patio1 isa location Name 'On the Trellised Patio'
  Description ""
  Exit West to On the School Steps1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the Trellised Patio1.

The Gardener isa thing at On the Trellised Patio1.
  IsDisplayedAs Gardener.
End The Gardener.

The By the School Fountain isa location Name 'By the School Fountain'
  Description ""
  Exit North to In the Art Cottage1.
  End exit.
  Exit South to In Staff Cottage B1.
  End exit.
  Exit East to On the School Steps1.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The By the School Fountain.

The Mystery Girl isa thing at By the School Fountain.
  IsDisplayedAs Mystery Girl.
End The Mystery Girl.

The In the Art Cottage1 isa location Name 'In the Art Cottage'
  Description ""
  Exit South to By the School Fountain.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Art Cottage1.

The Mr Rhodes isa thing at In the Art Cottage1.
  IsDisplayedAs Mr Rhodes.
End The Mr Rhodes.

The In Staff Cottage B1 isa location Name 'In Staff Cottage B'
  Description ""
  Exit North to By the School Fountain.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Staff Cottage B1.

The File Miss Arden isa thing at In Staff Cottage B1.
  IsDisplayedAs File (Miss Arden).
End The File Miss Arden.

The In the Receptionists Office1 isa location Name 'In the Receptionist's Office'
  Description ""
  Exit East to In the School Foyer1.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Receptionists Office1.

The In the Sick Bay1 isa location Name 'In the Sick Bay'
  Description ""
  Exit West to In the School Foyer1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Sick Bay1.

The Unnamed Girl isa thing at In the Sick Bay1.
  IsDisplayedAs Unnamed Girl.
End The Unnamed Girl.

The In the Marble Stateroom1 isa location Name 'In the Marble Stateroom'
  Description ""
  Exit North to In the South Hall1.
  End exit.
  Exit South to In the School Foyer1.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Marble Stateroom1.

The Maureen Sallis isa thing at In the Marble Stateroom1.
  IsDisplayedAs Maureen Sallis.
End The Maureen Sallis.

The In the South Hall1 isa location Name 'In the South Hall'
  Description ""
  Exit North to In the Grand Hall1.
  End exit.
  Exit South to In the Marble Stateroom1.
  End exit.
  Exit East to In the Sitting Room1.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the South Hall1.

The In the Sitting Room1 isa location Name 'In the Sitting Room'
  Description ""
  Exit North to In the Eastern Hallway1.
  End exit.
  Exit West to In the South Hall1.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Sitting Room1.

The Lucinda Parker isa thing at In the Sitting Room1.
  IsDisplayedAs Lucinda Parker.
End The Lucinda Parker.

The In the Eastern Hallway1 isa location Name 'In the Eastern Hallway'
  Description ""
  Exit North to In the Waiting Room1.
  End exit.
  Exit South to In the Sitting Room1.
  End exit.
  Exit East to In the Dance Room1.
  End exit.
  Exit West to In the Grand Hall1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Eastern Hallway1.

The Mia Fellini isa thing at In the Eastern Hallway1.
  IsDisplayedAs Mia Fellini.
End The Mia Fellini.

The In the Dance Room1 isa location Name 'In the Dance Room'
  Description ""
  Exit West to In the Eastern Hallway1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Dance Room1.

The The Dancer isa thing at In the Dance Room1.
  IsDisplayedAs The Dancer.
End The The Dancer.

The In the Waiting Room1 isa location Name 'In the Waiting Room'
  Description ""
  Exit North to In the Staff Library1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to In the Eastern Hallway1.
  End exit.
  Exit West to In Science Lab 1C1.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Waiting Room1.

The In Science Lab 1C1 isa location Name 'In Science Lab 1C'
  Description ""
  Exit South to In the Grand Hall1.
  End exit.
  Exit East to In the Waiting Room1.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In Science Lab 1C1.

The In the Grand Hall1 isa location Name 'In the Grand Hall'
  Description ""
  Exit North to In Science Lab 1C1.
  End exit.
  Exit South to In the South Hall1.
  End exit.
  Exit East to In the Eastern Hallway1.
  End exit.
  Exit West to In the School Chapel1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Grand Hall1.

The Madison Jones isa thing at In the Grand Hall1.
  IsDisplayedAs Madison Jones.
End The Madison Jones.

The Kate Seung isa thing at In the Grand Hall1.
  IsDisplayedAs Kate, Seung.
End The Kate Seung.

The In the School Chapel1 isa location Name 'In the School Chapel'
  Description ""
  Exit East to In the Grand Hall1.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the School Chapel1.

The In the Staff Library1 isa location Name 'In the Staff Library'
  Description ""
  Exit North to In the Leather Study1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to In the Waiting Room1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Staff Library1.

The In the Leather Study1 isa location Name 'In the Leather Study'
  Description ""
  Exit South to In the Staff Library1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to In the Strange Gallery1.
  End exit.
  Exit West to In a Curtained Hallway1.
  End exit.
  Exit Up to On the Balcony1.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Leather Study1.

The In the Strange Gallery1 isa location Name 'In the Strange Gallery'
  Description ""
  Exit West to In the Leather Study1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Strange Gallery1.

The On the Balcony1 isa location Name 'On the Balcony'
  Description ""
  Exit Down to In the Leather Study1.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the Balcony1.

The Witch isa thing at On the Balcony1.
  IsDisplayedAs Witch.
End The Witch.

The In a Curtained Hallway1 isa location Name 'In a Curtained Hallway'
  Description ""
  Exit East to In the Leather Study1.
  End exit.
  Exit West to In a Curtained Hallway2.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In a Curtained Hallway1.

The In a Curtained Hallway2 isa location Name 'In a Curtained Hallway'
  Description ""
  Exit East to In a Curtained Hallway1.
  End exit.
  Exit West to At the Opulent Corner.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In a Curtained Hallway2.

The At the Opulent Corner isa location Name 'At the Opulent Corner'
  Description ""
  Exit North to Before the Leadlight Door1.
  End exit.
  Exit East to In a Curtained Hallway2.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The At the Opulent Corner.

The Eve Joshi isa thing at At the Opulent Corner.
  IsDisplayedAs Eve Joshi.
End The Eve Joshi.

The Before the Leadlight Door1 isa location Name 'Before the Leadlight Door'
  Description ""
  Exit North to object1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to At the Opulent Corner.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Before the Leadlight Door1.

The object1 isa location Name '???'
  Description ""
  Exit North to In the Founders Room1.
  End exit.
  Exit South to Before the Leadlight Door1.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
  Is Not lit.
end The object1.

The In the Founders Room1 isa location Name 'In the Founder's Room'
  Description ""
  Exit South to object1.
  End exit.
  Exit East to In the Secret Place1.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Founders Room1.

The Founder isa thing at In the Founders Room1.
  IsDisplayedAs Founder.
End The Founder.

The In the Secret Place1 isa location Name 'In the Secret Place'
  Description ""
  Exit West to In the Founders Room1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Secret Place1.

The Shadow isa thing at In the Secret Place1.
  IsDisplayedAs Shadow.
End The Shadow.

The Founder2 isa thing at In the Secret Place1.
  IsDisplayedAs Founder.
End The Founder2.

The In the Upstairs Changing Room1 isa location Name 'In the Upstairs Changing Room'
  Description ""
  Exit Down to On the Gym Court North1.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The In the Upstairs Changing Room1.

The Isabelle Green isa thing at In the Upstairs Changing Room1.
  IsDisplayedAs Isabelle Green.
End The Isabelle Green.

