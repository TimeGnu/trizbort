-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "spiral" by J. Lautzenheiser
The Mountainside isa location Name 'Mountainside'
  Description ""
  Exit South to Foot of the Mountain S.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mountainside.

The Foot of the Mountain S isa location Name 'Foot of the Mountain S'
  Description ""
  Exit North to Mountainside.
  End exit.
  Exit East to Foot of the Mountain E.
  End exit.
  Exit West to Foot of the Mountain W.
  End exit.
  Exit Southeast to Church Isle.
  End exit.

  Exit South Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Foot of the Mountain S.

The Pole isa thing at Foot of the Mountain S.
  IsDisplayedAs Pole.
End The Pole.

The Foot of the Mountain W isa location Name 'Foot of the Mountain W'
  Description ""
  Exit East to Foot of the Mountain S.
  End exit.
  Exit West to Coast.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Foot of the Mountain W.

The Foot of the Mountain E isa location Name 'Foot of the Mountain E'
  Description
  "fsdfds"
  Exit West to Foot of the Mountain S.
  End exit.
  Exit Inside to On the Cloud1.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Foot of the Mountain E.

The On the Cloud1 isa location Name 'On the Cloud'
  Description ""
  Exit Outside to Foot of the Mountain E.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The On the Cloud1.

The Platform isa location Name 'Platform'
  Description ""
  Exit East to Machine Room West.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Platform.

The Machine Room West isa location Name 'Machine Room: West'
  Description ""
  Exit North to Over the Gap.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Machine Room Central.
  End exit.
  Exit West to Platform.
  End exit.
  Exit Up to Beneath the Pipe.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Machine Room West.

The put pole in recess1 isa thing at Machine Room West.
  IsDisplayedAs (put pole in recess).
End The put pole in recess1.

The Fall to Carriage isa location Name 'Fall to Carriage'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fall to Carriage.

The Fall to Carriage2 isa location Name 'Fall to Carriage'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fall to Carriage2.

The Fall to Carriage3 isa location Name 'Fall to Carriage'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fall to Carriage3.

The Machine Room Central isa location Name 'Machine Room: Central'
  Description ""
  Exit East to Machine Room East.
  End exit.
  Exit West to Machine Room West.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Machine Room Central.

The Crystal isa thing at Machine Room Central.
  IsDisplayedAs Crystal.
End The Crystal.

The Machine Room East isa location Name 'Machine Room: East'
  Description ""
  Exit West to Machine Room Central.
  End exit.
  Exit Down to Hospital Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Machine Room East.

The Beneath the Pipe isa location Name 'Beneath the Pipe'
  Description ""
  Exit Up to Fall to Carriage4.
  End exit.
  Exit Down to Machine Room West.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Beneath the Pipe.

The Crystal2 isa thing at Beneath the Pipe.
  IsDisplayedAs Crystal.
End The Crystal2.

The Church Isle isa location Name 'Church Isle'
  Description ""
  Exit Northwest to Foot of the Mountain S.
  End exit.

  Exit North South East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Church Isle.

The Page isa thing at Church Isle.
  IsDisplayedAs Page.
End The Page.

The Fall to Carriage4 isa location Name 'Fall to Carriage'
  Description ""
  Exit Down to Beneath the Pipe.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fall to Carriage4.

The Over the Gap isa location Name 'Over the Gap'
  Description ""
  Exit North to Scrap Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Machine Room West.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Over the Gap.

The Crystal3 isa thing at Over the Gap.
  IsDisplayedAs Crystal.
End The Crystal3.

The Scrap Room isa location Name 'Scrap Room'
  Description ""
  Exit South to Over the Gap.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Reths Den.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scrap Room.

The Phonecrystal isa thing at Scrap Room.
  IsDisplayedAs Phone/crystal.
End The Phonecrystal.

The Coast isa location Name 'Coast'
  Description ""
  Exit East to Foot of the Mountain W.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Up to Cave.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Coast.

The Page2 isa thing at Coast.
  IsDisplayedAs Page.
End The Page2.

The Cave isa location Name 'Cave'
  Description ""
  Exit South to Attic.
  End exit.
  Exit West to Tomb.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Down to Coast.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cave.

The Attic isa location Name 'Attic'
  Description ""
  Exit North to Cave.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Attic.

The Stuff page isa thing at Attic.
  IsDisplayedAs Stuff (page).
End The Stuff page.

The Laptop isa thing at Attic.
  IsDisplayedAs Laptop.
End The Laptop.

The Tomb isa location Name 'Tomb'
  Description ""
  Exit East to Cave.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tomb.

The Coffin page isa thing at Tomb.
  IsDisplayedAs Coffin (page).
End The Coffin page.

The Carriage isa location Name 'Carriage'
  Description ""
  Exit Northeast to Mountainside.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Southeast to Machine Room West.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East West Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Carriage.

The Underground Line isa location Name 'Underground Line'
  Description
  "Made in Twine, Men Don't Play With Dolls is an intimate and painful look into the beginnings of a lifelong struggle with addiction. Memories like these are not easily escaped."
  Exit South to Tomb.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Underground Line.

The Hat isa thing at Underground Line.
  IsDisplayedAs Hat.
End The Hat.

The Cat isa thing at Underground Line.
  IsDisplayedAs Cat.
End The Cat.

The Hospital Room isa location Name 'Hospital Room'
  Description ""
  Exit South to Machine Room East.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hospital Room.

The Reths Den isa location Name 'Reth's Den'
  Description ""
  Exit West to Scrap Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Reths Den.

The Box Soul isa thing at Reths Den.
  IsDisplayedAs Box (Soul).
End The Box Soul.

The Underground Line2 isa location Name 'Underground Line'
  Description ""
  Exit South to Reths Den.
  End exit.
  Exit East to Vat.
  End exit.
  Exit West to Summit.
  End exit.
  Exit Inside to Train Wreck.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Underground Line2.

The Summit isa location Name 'Summit'
  Description ""
  Exit Down to Underground Line2.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Summit.

The Vat isa location Name 'Vat'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Vat.

The Syringe isa thing at Vat.
  IsDisplayedAs Syringe.
End The Syringe.

The Train Wreck isa location Name 'Train Wreck'
  Description ""
  Exit Outside to Underground Line2.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Train Wreck.

