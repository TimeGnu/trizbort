-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "space-quest-coral-maze" by A Trizbort User
The Maze Start isa location Name 'Maze Start'
  Description ""
  Exit South to T right.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Maze Start.

The T right isa location Name 'T right'
  Description ""
  Exit North to Maze Start.
  End exit.
  Exit South to Up Down.
  End exit.
  Exit East to Left Right.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T right.

The Left Right isa location Name 'Left Right'
  Description ""
  Exit East to T down.
  End exit.
  Exit West to T right.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Left Right.

The Up Down isa location Name 'Up Down'
  Description ""
  Exit North to T right.
  End exit.
  Exit South to T left.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up Down.

The T left isa location Name 'T left'
  Description ""
  Exit North to Up Down.
  End exit.
  Exit West to Dead End.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T left.

The Dead End isa location Name 'Dead End'
  Description ""
  Exit East to T left.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dead End.

The T down isa location Name 'T down'
  Description ""
  Exit South to Up down2.
  End exit.
  Exit West to Left Right.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T down.

The Up down2 isa location Name 'Up down'
  Description ""
  Exit North to T down.
  End exit.
  Exit South to Up Right.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up down2.

The Up Right isa location Name 'Up Right'
  Description ""
  Exit North to Up down2.
  End exit.
  Exit East to Left Right2.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up Right.

The Left Right2 isa location Name 'Left Right'
  Description ""
  Exit East to Left Right3.
  End exit.
  Exit West to Up Right.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Left Right2.

The Left Right3 isa location Name 'Left Right'
  Description ""
  Exit East to T down2.
  End exit.
  Exit West to Left Right2.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Left Right3.

The T down2 isa location Name 'T down'
  Description ""
  Exit South to T right2.
  End exit.
  Exit West to Left Right3.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T down2.

The T right2 isa location Name 'T right'
  Description ""
  Exit North to T down2.
  End exit.
  Exit South to Cross Roads.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T right2.

The Cross Roads isa location Name 'Cross Roads'
  Description ""
  Exit North to T right2.
  End exit.
  Exit South to Up Down3.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cross Roads.

The Up Down3 isa location Name 'Up Down'
  Description ""
  Exit North to Cross Roads.
  End exit.
  Exit West to Up Left.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up Down3.

The Up Left isa location Name 'Up Left'
  Description ""
  Exit East to Up Down3.
  End exit.
  Exit West to T down3.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up Left.

The T down3 isa location Name 'T down'
  Description ""
  Exit South to T left2.
  End exit.
  Exit East to Up Left.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T down3.

The T left2 isa location Name 'T left'
  Description ""
  Exit North to T down3.
  End exit.
  Exit South to Up right2.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The T left2.

The Up right2 isa location Name 'Up right'
  Description ""
  Exit North to T left2.
  End exit.
  Exit East to Panel.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Up right2.

The Panel isa location Name 'Panel'
  Description ""
  Exit East to Green Button.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Up right2.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Panel.

The Need alien paper isa thing at Panel.
  IsDisplayedAs Need alien paper.
End The Need alien paper.

The From behind shark isa thing at Panel.
  IsDisplayedAs From behind shark.
End The From behind shark.

The Green Button isa location Name 'Green Button'
  Description ""
  Exit South to Hall.
  End exit.
  Exit East to Nice scene.
  End exit.
  Exit West to Panel.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Green Button.

The Use panel  3 isa thing at Green Button.
  IsDisplayedAs Use panel * 3.
End The Use panel  3.

The Nice scene isa location Name 'Nice scene'
  Description ""
  Exit West to Green Button.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Nice scene.

The Hall isa location Name 'Hall'
  Description ""
  Exit North to Green Button.
  End exit.
  Exit South to Hall2.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall.

The Hall2 isa location Name 'Hall'
  Description ""
  Exit North to Hall.
  End exit.
  Exit East to Ship.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall2.

The Ship isa location Name 'Ship'
  Description ""
  Exit West to Hall2.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ship.

The Craft Start isa location Name 'Craft Start'
  Description ""
  Exit West to R00.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Craft Start.

The R00 isa location Name 'R00'
  Description ""
  Exit East to Craft Start.
  End exit.
  Exit West to R002.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The R00.

The R002 isa location Name 'R00'
  Description ""
  Exit East to R00.
  End exit.
  Exit West to R003.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The R002.

The R003 isa location Name 'R00'
  Description ""
  Exit East to R002.
  End exit.
  Exit West to Other Ship.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The R003.

The Other Ship isa location Name 'Other Ship'
  Description ""
  Exit East to R003.
  End exit.
  Exit West to R004.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Other Ship.

The Box isa thing at Other Ship.
  IsDisplayedAs Box.
End The Box.

The Need pressure suit isa thing at Other Ship.
  IsDisplayedAs Need pressure suit.
End The Need pressure suit.

The R004 isa location Name 'R00'
  Description ""
  Exit North to City Entry.
  End exit.
  Exit East to Other Ship.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The R004.

The City Entry isa location Name 'City Entry'
  Description ""
  Exit South to R004.
  End exit.
  Exit Northwest to Dock Bay.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The City Entry.

The USE BOX isa thing at City Entry.
  IsDisplayedAs USE BOX.
End The USE BOX.

The EXIT CITY to leave isa thing at City Entry.
  IsDisplayedAs EXIT CITY to leave.
End The EXIT CITY to leave.

The Dock Bay isa location Name 'Dock Bay'
  Description ""
  Exit East to Big Door1.
  End exit.
  Exit Southeast to City Entry.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dock Bay.

The Big Door1 isa location Name 'Big Door'
  Description ""
  Exit East to Hall3.
  End exit.
  Exit West to Dock Bay.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Big Door1.

The USE PANEL isa thing at Big Door1.
  IsDisplayedAs USE PANEL.
End The USE PANEL.

The Hall3 isa location Name 'Hall'
  Description ""
  Exit South to Big Door1.
  End exit.
  Exit East to Hall4.
  End exit.
  Exit West to Aliens.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall3.

The Aliens isa location Name 'Aliens'
  Description ""
  Exit East to Hall3.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Aliens.

The Hall4 isa location Name 'Hall'
  Description ""
  Exit South to Hall5.
  End exit.
  Exit West to Hall3.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall4.

The Hall5 isa location Name 'Hall'
  Description ""
  Exit North to Hall4.
  End exit.
  Exit East to Stay North.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall5.

The Stay North isa location Name 'Stay North'
  Description ""
  Exit East to Hall6.
  End exit.
  Exit West to Hall5.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Stay North.

The Hall6 isa location Name 'Hall'
  Description ""
  Exit North to Hall7.
  End exit.
  Exit East to Aliens2.
  End exit.
  Exit West to Stay North.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall6.

The Aliens2 isa location Name 'Aliens'
  Description ""
  Exit West to Hall6.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Aliens2.

The Extra bad isa thing at Aliens2.
  IsDisplayedAs Extra bad.
End The Extra bad.

The Theyll follow isa thing at Aliens2.
  IsDisplayedAs They'll follow.
End The Theyll follow.

The Hall7 isa location Name 'Hall'
  Description ""
  Exit North to Hall8.
  End exit.
  Exit South to Hall6.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall7.

The Hall8 isa location Name 'Hall'
  Description ""
  Exit South to Hall7.
  End exit.
  Exit East to Hall9.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall8.

The Hall9 isa location Name 'Hall'
  Description ""
  Exit East to Mus-eum.
  End exit.
  Exit West to Hall8.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hall9.

The Mus-eum isa location Name 'Mus-eum'
  Description ""
  Exit West to Hall9.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mus-eum.

The MASK isa thing at Mus-eum.
  IsDisplayedAs MASK.
End The MASK.

The OUTFIT isa thing at Mus-eum.
  IsDisplayedAs OUTFIT.
End The OUTFIT.

The Item isa location Name 'Item'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Item.

The Use-less isa location Name 'Use-less'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Use-less.

The Death isa location Name 'Death'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Death.

The Walk Thru isa location Name 'Walk Thru'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Walk Thru.

