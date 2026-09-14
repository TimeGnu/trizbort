-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "further" by A Trizbort User
The Foggy Room isa location Name 'Foggy Room'
  Description ""
  Exit North to Prairie.
  End exit.
  Exit South to Cave.
  End exit.
  Exit East to Hearth.
  End exit.
  Exit West to Sandstorm.
  End exit.
  Exit Northeast to Tundra.
  End exit.

  Exit Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Foggy Room.

The Ruddy Bell isa thing at Foggy Room.
  IsDisplayedAs Ruddy Bell.
End The Ruddy Bell.

The Prairie isa location Name 'Prairie'
  Description ""
  Exit South to Foggy Room.
  End exit.
  Exit Northwest to Manic Sky.
  End exit.

  Exit North East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Prairie.

The Cave isa location Name 'Cave'
  Description ""
  Exit North to Foggy Room.
  End exit.
  Exit Down to Shifting Grotto.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cave.

The Hearth isa location Name 'Hearth'
  Description ""
  Exit West to Foggy Room.
  End exit.
  Exit Down to Low Room.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hearth.

The Sandstorm isa location Name 'Sandstorm'
  Description ""
  Exit East to Foggy Room.
  End exit.
  Exit Down to Countless Dunes.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sandstorm.

The Tundra isa location Name 'Tundra'
  Description ""
  Exit Northeast to Infinite Ice Floes.
  End exit.
  Exit Southwest to Foggy Room.
  End exit.

  Exit North South East West Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tundra.

The Manic Sky isa location Name 'Manic Sky'
  Description ""
  Exit West to Grassland.
  End exit.
  Exit Southeast to Prairie.
  End exit.

  Exit North South East Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Manic Sky.

The Becomes isa thing at Manic Sky.
  IsDisplayedAs Becomes.
End The Becomes.

The Pet Shop isa thing at Manic Sky.
  IsDisplayedAs Pet Shop.
End The Pet Shop.

The Grassland isa location Name 'Grassland'
  Description ""
  Exit East to Manic Sky.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Grassland.

The Ashen Robe isa thing at Grassland.
  IsDisplayedAs Ashen Robe.
End The Ashen Robe.

The Infinite Ice Floes isa location Name 'Infinite Ice Floes'
  Description ""
  Exit Southwest to Tundra.
  End exit.
  Exit Up to Cement Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East West Northeast Southeast Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Infinite Ice Floes.

The Becomes Frozen Pond isa thing at Infinite Ice Floes.
  IsDisplayedAs Becomes Frozen Pond.
End The Becomes Frozen Pond.

The Low Room isa location Name 'Low Room'
  Description ""
  Exit Southeast to Alcove.
  End exit.
  Exit Up to Hearth.
  End exit.

  Exit North South East West Northeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Low Room.

The Becomes2 isa thing at Low Room.
  IsDisplayedAs Becomes.
End The Becomes2.

The Your Basement isa thing at Low Room.
  IsDisplayedAs Your Basement.
End The Your Basement.

The Alcove isa location Name 'Alcove'
  Description ""
  Exit Northwest to Low Room.
  End exit.

  Exit North South East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Alcove.

The Jaundiced Penny isa thing at Alcove.
  IsDisplayedAs Jaundiced Penny.
End The Jaundiced Penny.

The Countless Dunes isa location Name 'Countless Dunes'
  Description ""
  Exit Southwest to Oasis.
  End exit.
  Exit Up to Sandstorm.
  End exit.

  Exit North South East West Northeast Southeast Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Countless Dunes.

The Becomes Alans Den isa thing at Countless Dunes.
  IsDisplayedAs Becomes Alan's Den.
End The Becomes Alans Den.

The Oasis isa location Name 'Oasis'
  Description ""
  Exit Northeast to Countless Dunes.
  End exit.

  Exit North South East West Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Oasis.

The Cobalt Ribbon isa thing at Oasis.
  IsDisplayedAs Cobalt Ribbon.
End The Cobalt Ribbon.

The Shifting Grotto isa location Name 'Shifting Grotto'
  Description ""
  Exit Southeast to Crumbling Chimney.
  End exit.
  Exit Up to Cave.
  End exit.

  Exit North South East West Northeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Shifting Grotto.

The Becomes3 isa thing at Shifting Grotto.
  IsDisplayedAs Becomes.
End The Becomes3.

The Lindas Room isa thing at Shifting Grotto.
  IsDisplayedAs Linda's Room.
End The Lindas Room.

The Crumbling Chimney isa location Name 'Crumbling Chimney'
  Description ""
  Exit Northwest to Shifting Grotto.
  End exit.

  Exit North South East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Crumbling Chimney.

The Olive Envelope isa thing at Crumbling Chimney.
  IsDisplayedAs Olive Envelope.
End The Olive Envelope.

The Cement Room isa location Name 'Cement Room'
  Description ""
  Exit North to Brick-Lined Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Down to Infinite Ice Floes.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cement Room.

The Brick-Lined Room isa location Name 'Brick-Lined Room'
  Description ""
  Exit North to Sandy Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Cement Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Brick-Lined Room.

The Sandy Room isa location Name 'Sandy Room'
  Description ""
  Exit North to Water Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Brick-Lined Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sandy Room.

The Water Room isa location Name 'Water Room'
  Description ""
  Exit North to Writing Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Sandy Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water Room.

The Writing Room isa location Name 'Writing Room'
  Description ""
  Exit North to Spinning Expanse.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Water Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Writing Room.

The Spinning Expanse isa location Name 'Spinning Expanse'
  Description ""
  Exit South to Writing Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Spinning Expanse.

