-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "Zork_770614" by A Trizbort User
The West of House isa location Name 'West of House'
  Description ""
  Exit North to North of House.
  End exit.
  Exit South to South of House.
  End exit.
  Exit West to Forest 2.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The West of House.

The South of House isa location Name 'South of House'
  Description ""
  Exit South to Forest 1.
  End exit.
  Exit East to Behind House.
  End exit.
  Exit West to West of House.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The South of House.

The Behind House isa location Name 'Behind House'
  Description ""
  Exit North to North of House.
  End exit.
  Exit South to South of House.
  End exit.
  Exit East to Clearing.
  End exit.
  Exit West to Kitchen.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Behind House.

The Small window isa thing at Behind House.
  IsDisplayedAs Small window.
End The Small window.

The North of House isa location Name 'North of House'
  Description ""
  Exit North to Forest 3.
  End exit.
  Exit East to Behind House.
  End exit.
  Exit West to West of House.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The North of House.

The Clearing isa location Name 'Clearing'
  Description ""
  Exit South to Forest 1.
  End exit.
  Exit West to Forest 3.
  End exit.
  Exit Southwest to Behind House.
  End exit.

  Exit North East Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clearing.

The Pile of leaves isa thing at Clearing.
  IsDisplayedAs Pile of leaves.
End The Pile of leaves.

The Kitchen isa location Name 'Kitchen'
  Description ""
  Exit East to Behind House.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to Living Room.
  End exit.
  Exit Up to Attic.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Kitchen.

The Bottle isa thing at Kitchen.
  IsDisplayedAs Bottle.
End The Bottle.

The Sack isa thing at Kitchen.
  IsDisplayedAs Sack.
End The Sack.

The Living Room isa location Name 'Living Room'
  Description ""
  Exit East to Kitchen.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Living Room.

The Tropgy case isa thing at Living Room.
  IsDisplayedAs Tropgy case.
End The Tropgy case.

The Lantern isa thing at Living Room.
  IsDisplayedAs Lantern.
End The Lantern.

The Rug isa thing at Living Room.
  IsDisplayedAs Rug.
End The Rug.

The Attic isa location Name 'Attic'
  Description ""
  Exit Down to Kitchen.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Attic.

The Rope isa thing at Attic.
  IsDisplayedAs Rope.
End The Rope.

The Knife isa thing at Attic.
  IsDisplayedAs Knife.
End The Knife.

The Forest 1 isa location Name 'Forest 1'
  Description ""
  Exit North to South of House.
  End exit.
  Exit East to Clearing.
  End exit.
  Exit West to Forest 2.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest 1.

The Forest 2 isa location Name 'Forest 2'
  Description ""
  Exit East to Forest 3.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest 2.

The Forest 3 isa location Name 'Forest 3'
  Description ""
  Exit South to Clearing.
  End exit.
  Exit East to Clearing.
  End exit.
  Exit West to North of House.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Forest 3.

The Cellar isa location Name 'Cellar'
  Description ""
  Exit South to West of Chasm.
  End exit.
  Exit East to Troll Room.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cellar.

The West of Chasm isa location Name 'West of Chasm'
  Description ""
  Exit North to North-South Crawlway.
  End exit.
  Exit South to Gallery.
  End exit.
  Exit West to Cellar.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The West of Chasm.

The Gallery isa location Name 'Gallery'
  Description ""
  Exit North to West of Chasm.
  End exit.
  Exit South to Studio.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gallery.

The Painting isa thing at Gallery.
  IsDisplayedAs Painting.
End The Painting.

The Studio isa location Name 'Studio'
  Description ""
  Exit North to North-South Crawlway.
  End exit.
  Exit Northwest to Gallery.
  End exit.

  Exit South East West Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Studio.

The Chimney isa thing at Studio.
  IsDisplayedAs Chimney.
End The Chimney.

The North-South Crawlway isa location Name 'North-South Crawlway'
  Description ""
  Exit North to West of Chasm.
  End exit.
  Exit South to Studio.
  End exit.
  Exit East to Troll Room.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The North-South Crawlway.

The Troll Room isa location Name 'Troll Room'
  Description ""
  Exit North to East-West Passage.
  End exit.
  Exit East to North-South Crawlway.
  End exit.
  Exit West to Cellar.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Troll Room.

The Troll isa thing at Troll Room.
  IsDisplayedAs Troll.
End The Troll.

The East-West Passage isa location Name 'East-West Passage'
  Description ""
  Exit North to Deep Ravine.
  End exit.
  Exit East to Round Room.
  End exit.
  Exit West to Troll Room.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The East-West Passage.

The Cyclops Room isa location Name 'Cyclops Room'
  Description ""
  Exit Up to Treasure Room.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cyclops Room.

The Cyclop isa thing at Cyclops Room.
  IsDisplayedAs Cyclop.
End The Cyclop.

The Strange Passage isa location Name 'Strange Passage'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Strange Passage.

The Treasure Room isa location Name 'Treasure Room'
  Description ""
  Exit Down to Cyclops Room.
  End exit.

  Exit North South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Treasure Room.

The Thief isa thing at Treasure Room.
  IsDisplayedAs Thief.
End The Thief.

The Chalice isa thing at Treasure Room.
  IsDisplayedAs Chalice.
End The Chalice.

The Dome Room isa location Name 'Dome Room'
  Description ""
  Exit East to Rocky Crawl.
  End exit.
  Exit Down to Torch Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dome Room.

The Railing isa thing at Dome Room.
  IsDisplayedAs Railing.
End The Railing.

The Rocky Crawl isa location Name 'Rocky Crawl'
  Description ""
  Exit East to Dome Room.
  End exit.
  Exit West to Deep Ravine.
  End exit.
  Exit Northwest to Egyptian Room.
  End exit.

  Exit North South Northeast Southeast Southwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rocky Crawl.

The Deep Ravine isa location Name 'Deep Ravine'
  Description ""
  Exit South to East-West Passage.
  End exit.
  Exit East to Chasm.
  End exit.
  Exit West to Rocky Crawl.
  End exit.
  Exit Down to Reservoir South.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Deep Ravine.

The Chasm isa location Name 'Chasm'
  Description ""
  Exit South to Deep Ravine.
  End exit.
  Exit East to North-South Passage.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Chasm.

The North-South Passage isa location Name 'North-South Passage'
  Description ""
  Exit North to Chasm.
  End exit.
  Exit South to Round Room.
  End exit.
  Exit Northeast to Loud Room.
  End exit.

  Exit East West Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The North-South Passage.

The Loud Room isa location Name 'Loud Room'
  Description ""
  Exit East to Ancient Chasm.
  End exit.
  Exit West to North-South Passage.
  End exit.
  Exit Up to  Damp Cave.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Loud Room.

The Platinum Bar isa thing at Loud Room.
  IsDisplayedAs Platinum Bar.
End The Platinum Bar.

The Round Room isa location Name 'Round Room'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Round Room.

The Egyptian Room isa location Name 'Egyptian Room'
  Description ""
  Exit East to Rocky Crawl.
  End exit.
  Exit West to Glacier Room.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Egyptian Room.

The Gold Coffin isa thing at Egyptian Room.
  IsDisplayedAs Gold Coffin.
End The Gold Coffin.

The Glacier Room isa location Name 'Glacier Room'
  Description ""
  Exit North to Stream View.
  End exit.
  Exit East to Egyptian Room.
  End exit.
  Exit West to Ruby Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Glacier Room.

The Ice isa thing at Glacier Room.
  IsDisplayedAs Ice.
End The Ice.

The Stream View isa location Name 'Stream View'
  Description ""
  Exit North to Glacier Room.
  End exit.
  Exit East to Reservoir South.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Stream View.

The Reservoir South isa location Name 'Reservoir South'
  Description ""
  Exit North to Reservoir North.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Deep Ravine.
  End exit.
  Exit West to Stream View.
  End exit.
  Exit Up to Deep Canyon.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Reservoir South.

The Rope2 isa thing at Reservoir South.
  IsDisplayedAs Rope.
End The Rope2.

The Trunk with Jewels isa thing at Reservoir South.
  IsDisplayedAs Trunk with Jewels.
End The Trunk with Jewels.

The Deep Canyon isa location Name 'Deep Canyon'
  Description ""
  Exit East to Dam.
  End exit.
  Exit Southeast to Reservoir South.
  End exit.

  Exit North South West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Deep Canyon.

The Torch Room isa location Name 'Torch Room'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Torch Room.

The Torch isa thing at Torch Room.
  IsDisplayedAs Torch.
End The Torch.

The Dam isa location Name 'Dam'
  Description ""
  Exit North to Dam Lobby.
  End exit.
  Exit South to Deep Canyon.
  End exit.
  Exit East to  Damp Cave.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dam.

The  Damp Cave isa location Name ' Damp Cave'
  Description ""
  Exit South to Loud Room.
  End exit.
  Exit East to Dam.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The  Damp Cave.

The Small Crack isa thing at  Damp Cave.
  IsDisplayedAs Small Crack.
End The Small Crack.

The Dam Lobby isa location Name 'Dam Lobby'
  Description ""
  Exit North to Maintenance Room.
  End exit.
  Exit South to Dam.
  End exit.
  Exit East to Maintenance Room.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dam Lobby.

The Matchbook isa thing at Dam Lobby.
  IsDisplayedAs Matchbook.
End The Matchbook.

The Guidebooks isa thing at Dam Lobby.
  IsDisplayedAs Guidebooks.
End The Guidebooks.

The Maintenance Room isa location Name 'Maintenance Room'
  Description ""
  Exit South to Dam Lobby.
  End exit.
  Exit West to Dam Lobby.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Maintenance Room.

The Tube of toothpaste isa thing at Maintenance Room.
  IsDisplayedAs Tube of toothpaste.
End The Tube of toothpaste.

The Wrench isa thing at Maintenance Room.
  IsDisplayedAs Wrench.
End The Wrench.

The Screwdriver isa thing at Maintenance Room.
  IsDisplayedAs Screwdriver.
End The Screwdriver.

The Ancient Chasm isa location Name 'Ancient Chasm'
  Description ""
  Exit North to Dead End2.
  End exit.
  Exit South to Loud Room.
  End exit.
  Exit West to Dead End.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ancient Chasm.

The Reservoir North isa location Name 'Reservoir North'
  Description ""
  Exit North to Atlantis Room.
  End exit.
  Exit South to Reservoir South.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Reservoir North.

The Atlantis Room isa location Name 'Atlantis Room'
  Description ""
  Exit Southeast to Reservoir North.
  End exit.
  Exit Up to Cave.
  End exit.

  Exit North South East West Northeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Atlantis Room.

The Trident isa thing at Atlantis Room.
  IsDisplayedAs Trident.
End The Trident.

The Cave isa location Name 'Cave'
  Description ""
  Exit North to Mirror Room.
  End exit.
  Exit Down to Atlantis Room.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cave.

The Mirror Room isa location Name 'Mirror Room'
  Description ""
  Exit North to Steep Crawlway.
  End exit.
  Exit East to Cave.
  End exit.
  Exit West to Cold Passage.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mirror Room.

The Cold Passage isa location Name 'Cold Passage'
  Description ""
  Exit North to Steep Crawlway.
  End exit.
  Exit East to Mirror Room.
  End exit.
  Exit West to Slide Room.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cold Passage.

The Steep Crawlway isa location Name 'Steep Crawlway'
  Description ""
  Exit South to Mirror Room.
  End exit.
  Exit Southwest to Cold Passage.
  End exit.

  Exit North East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Steep Crawlway.

The Slide Room isa location Name 'Slide Room'
  Description ""
  Exit East to Cold Passage.
  End exit.
  Exit Down to Cellar.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Slide Room.

The Entrance to Hades isa location Name '*Entrance to Hades'
  Description ""
  Exit East to Land of the Living Dead.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Up to Cave2.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Entrance to Hades.

The Evil Spirits isa thing at Entrance to Hades.
  IsDisplayedAs Evil Spirits.
End The Evil Spirits.

The Narrow Crawlway isa location Name 'Narrow Crawlway'
  Description ""
  Exit North to Grail Room.
  End exit.
  Exit South to Cave2.
  End exit.
  Exit Southwest to Mirror Room2.
  End exit.

  Exit East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Narrow Crawlway.

The Grail Room isa location Name 'Grail Room'
  Description ""
  Exit East to Narrow Crawlway.
  End exit.
  Exit West to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Up to Temple.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Grail Room.

The Grail isa thing at Grail Room.
  IsDisplayedAs Grail.
End The Grail.

The Round Room2 isa location Name 'Round Room'
  Description ""
  Exit North to Deep Canyon2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit South to Winding Passage.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Grail Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit West to East-West Passage2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Northeast to Maze.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Southwest to Engravings Cave.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Northwest to North-South Passage2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit Southeast Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Round Room2.

The East-West Passage2 isa location Name 'East-West Passage'
  Description ""
  Exit East to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The East-West Passage2.

The North-South Passage2 isa location Name 'North-South Passage'
  Description ""
  Exit Southeast to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The North-South Passage2.

The Winding Passage isa location Name 'Winding Passage'
  Description ""
  Exit North to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit East to Mirror Room2.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Winding Passage.

The Mirror Room2 isa location Name 'Mirror Room'
  Description ""
  Exit North to Narrow Crawlway.
  End exit.
  Exit East to Cave2.
  End exit.
  Exit West to Winding Passage.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mirror Room2.

The Mirror isa thing at Mirror Room2.
  IsDisplayedAs Mirror.
End The Mirror.

The Cave2 isa location Name 'Cave'
  Description ""
  Exit North to Narrow Crawlway.
  End exit.
  Exit West to Mirror Room2.
  End exit.
  Exit Down to Entrance to Hades.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cave2.

The Deep Canyon2 isa location Name 'Deep Canyon'
  Description ""
  Exit South to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Deep Canyon2.

The Maze isa location Name 'Maze'
  Description ""
  Exit Southwest to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Maze.

The Engravings Cave isa location Name 'Engravings Cave'
  Description ""
  Exit North to Round Room2.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Southeast to Riddle Room.
  End exit.

  Exit South East West Northeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Engravings Cave.

The Engravings isa thing at Engravings Cave.
  IsDisplayedAs Engravings.
End The Engravings.

The Riddle Room isa location Name 'Riddle Room'
  Description ""
  Exit East to Pearl Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.
  Exit Down to Engravings Cave.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Riddle Room.

The Pearl Room isa location Name 'Pearl Room'
  Description ""
  Exit West to Riddle Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pearl Room.

The Pearl Necklace isa thing at Pearl Room.
  IsDisplayedAs Pearl Necklace.
End The Pearl Necklace.

The Temple isa location Name 'Temple'
  Description ""
  Exit East to Altar.
  End exit.
  Exit West to Grail Room.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Temple.

The Brass bell isa thing at Temple.
  IsDisplayedAs Brass bell.
End The Brass bell.

The Altar isa location Name 'Altar'
  Description ""
  Exit West to Temple.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Altar.

The Black Book isa thing at Altar.
  IsDisplayedAs Black Book.
End The Black Book.

The Candles isa thing at Altar.
  IsDisplayedAs Candles.
End The Candles.

The Dead End isa location Name 'Dead End'
  Description ""
  Exit East to Ancient Chasm.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dead End.

The Dead End2 isa location Name 'Dead End'
  Description ""
  Exit Southwest to Ancient Chasm.
  End exit.

  Exit North South East West Northeast Southeast Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Dead End2.

The Land of the Living Dead isa location Name 'Land of the Living Dead'
  Description ""
  Exit West to Entrance to Hades.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Land of the Living Dead.

The Ruby Room isa location Name 'Ruby Room'
  Description ""
  Exit South to Glacier Room.
    Check
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ruby Room.

The Ruby isa thing at Ruby Room.
  IsDisplayedAs Ruby.
End The Ruby.

