-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "properties-in-brackets-basic" by A Trizbort User
The Cave isa location Name 'Cave'
  Description ""
  Exit East to Error Room.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cave.

The Fred isa actor at Cave.
  IsDisplayedAs Fred.
End The Fred.

The Wilma isa actor at Cave.
  IsDisplayedAs Wilma.
End The Wilma.

The Pat isa actor at Cave.
  IsDisplayedAs Pat.
End The Pat.

The Mice isa thing at Cave.
  IsDisplayedAs Mice.
End The Mice.

The Mess isa thing at Cave.
  IsDisplayedAs Mess.
End The Mess.

The Oven isa thing at Cave.
  IsDisplayedAs Oven.
End The Oven.

The Table isa thing at Cave.
  IsDisplayedAs Table.
End The Table.

The Window isa thing at Cave.
  IsDisplayedAs Window.
End The Window.

The Error Room isa location Name 'Error Room'
  Description ""
  Exit West to Cave.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Error Room.

The Genders isa actor at Error Room.
  IsDisplayedAs Genders.
End The Genders.

The Genders2 isa actor at Error Room.
  IsDisplayedAs Genders.
End The Genders2.

The Genders3 isa actor at Error Room.
  IsDisplayedAs Genders.
End The Genders3.

The Oddperson isa actor at Error Room.
  IsDisplayedAs Oddperson.
End The Oddperson.

The Oddperson2 isa actor at Error Room.
  IsDisplayedAs Oddperson.
End The Oddperson2.

The Oddperson3 isa actor at Error Room.
  IsDisplayedAs Oddperson.
End The Oddperson3.

The Singplur isa thing at Error Room.
  IsDisplayedAs Singplur.
End The Singplur.

