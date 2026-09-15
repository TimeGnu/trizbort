-- Alan currently does not process metadata such as title and author, so those will be in the metadata below.
-- Trizbort exports History to the 'about' verb.
-- All other metadata will be in comments below.

-- "space-quest-lost-chapter-surface" by A Trizbort User
The Rock Tree isa location Name 'Rock Tree'
  Description ""
  Exit North to Rock With Vines.
  End exit.
  Exit South to End of Path.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rock Tree.

The Rock With Vines isa location Name 'Rock With Vines'
  Description ""
  Exit South to Rock Tree.
  End exit.
  Exit East to Rock Path.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rock With Vines.

The Rock Path isa location Name 'Rock Path'
  Description ""
  Exit East to Rock Bridge.
  End exit.
  Exit West to Rock With Vines.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rock Path.

The Rock Bridge isa location Name 'Rock Bridge'
  Description ""
  Exit East to NW Rock Path 2.
  End exit.
  Exit West to Rock Path.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Rock Bridge.

The NW Rock Path 2 isa location Name 'N/W Rock Path 2'
  Description ""
  Exit East to NW Rock Path.
  End exit.
  Exit West to Rock Bridge.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The NW Rock Path 2.

The Be careful can fall isa thing at NW Rock Path 2.
  IsDisplayedAs Be careful, can fall.
End The Be careful can fall.

The NW Rock Path isa location Name 'N/W Rock Path'
  Description ""
  Exit South to Shore.
  End exit.
  Exit West to NW Rock Path 2.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The NW Rock Path.

The Shore isa location Name 'Shore'
  Description ""
  Exit North to NW Rock Path.
  End exit.
  Exit East to Ship Front.
  End exit.
  Exit West to Water fall.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Shore.

The Water fall isa location Name 'Water fall'
  Description ""
  Exit East to Shore.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water fall.

The Ship Front isa location Name 'Ship Front'
  Description ""
  Exit South to Monster chases you2.
  End exit.
  Exit East to Ship Body.
  End exit.
  Exit West to Shore.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ship Front.

The Ship Body isa location Name 'Ship Body'
  Description ""
  Exit South to Monster chases you2.
  End exit.
  Exit East to Ship Tail.
  End exit.
  Exit West to Ship Front.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ship Body.

The Ship Tail isa location Name 'Ship Tail'
  Description ""
  Exit South to Monster chases you2.
  End exit.
  Exit East to Lake.
  End exit.
  Exit West to Ship Body.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ship Tail.

The NAIL isa thing at Ship Tail.
  IsDisplayedAs NAIL.
End The NAIL.

The Lake isa location Name 'Lake'
  Description ""
  Exit East to Lagoon CLIMB.
  End exit.
  Exit West to Ship Tail.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake.

The Lagoon CLIMB isa location Name 'Lagoon CLIMB'
  Description ""
  Exit North to Tree.
  End exit.
  Exit East to Water W to lagoon.
  End exit.
  Exit West to Lake.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lagoon CLIMB.

The Go SWW to next lake isa thing at Lagoon CLIMB.
  IsDisplayedAs Go SW/W to next lake.
End The Go SWW to next lake.

The Tree isa location Name 'Tree'
  Description ""
  Exit South to Lagoon CLIMB.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree.

The Tree2 isa location Name 'Tree'
  Description ""
  Exit East to FRUIT.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree2.

The FRUIT isa location Name 'FRUIT'
  Description ""
  Exit West to Tree2.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The FRUIT.

The Water W to lagoon isa location Name 'Water W to lagoon'
  Description ""
  Exit South to Monster chases you.
  End exit.
  Exit East to Lake on S1.
  End exit.
  Exit West to Lagoon CLIMB.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water W to lagoon.

The Lake on S1 isa location Name 'Lake on S'
  Description ""
  Exit South to Monster chases you.
  End exit.
  Exit East to Lake in SW1.
  End exit.
  Exit West to Water W to lagoon.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake on S1.

The Lake in SW1 isa location Name 'Lake in SW'
  Description ""
  Exit North to jungle.
  End exit.
  Exit South to Death in S1.
  End exit.
  Exit East to Grey Rock.
  End exit.
  Exit West to Lake on S1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lake in SW1.

The Monster chases you isa location Name 'Monster chases you'
  Description ""
  Exit North to Lake on S1.
  End exit.
  Exit East to Death in S1.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Monster chases you.

The jungle isa location Name 'jungle'
  Description ""
  Exit South to Lake in SW1.
  End exit.
  Exit East to MOULD N.
  End exit.
  Exit West to Lake in SW1.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The jungle.

The MOULD N isa location Name 'MOULD (N)'
  Description ""
  Exit East to Flower N.
  End exit.
  Exit West to jungle.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The MOULD N.

The Flower N isa location Name 'Flower (N)'
  Description ""
  Exit South to Grey Tree.
  End exit.
  Exit East to jungle2.
  End exit.
  Exit West to MOULD N.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Flower N.

The Mould isa thing at Flower N.
  IsDisplayedAs Mould.
End The Mould.

The jungle2 isa location Name 'jungle'
  Description ""
  Exit South to jungle3.
  End exit.
  Exit West to Flower N.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The jungle2.

The jungle3 isa location Name 'jungle'
  Description ""
  Exit North to jungle2.
  End exit.
  Exit South to jungle5.
  End exit.
  Exit East to Your Ship.
  End exit.
  Exit West to Grey Tree.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The jungle3.

The Your Ship isa location Name 'Your Ship'
  Description ""
  Exit South to jungle3.
  End exit.
  Exit East to jungle4.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Your Ship.

The jungle4 isa location Name 'jungle'
  Description ""
  Exit North to Your Ship.
  End exit.
  Exit West to jungle5.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The jungle4.

The jungle5 isa location Name 'jungle'
  Description ""
  Exit North to jungle3.
  End exit.
  Exit South to CODE CARD.
  End exit.
  Exit East to jungle4.
  End exit.
  Exit West to Black Tree.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The jungle5.

The Black Tree isa location Name 'Black Tree'
  Description ""
  Exit North to Grey Tree.
  End exit.
  Exit East to jungle5.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Black Tree.

The Grey Tree isa location Name 'Grey Tree'
  Description ""
  Exit North to Flower N.
  End exit.
  Exit South to Black Tree.
  End exit.
  Exit East to jungle3.
  End exit.
  Exit West to Grey Rock.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Grey Tree.

The Grey Rock isa location Name 'Grey Rock'
  Description ""
  Exit East to Grey Tree.
  End exit.
  Exit West to Lake in SW1.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Grey Rock.

The CODE CARD isa location Name 'CODE CARD'
  Description ""
  Exit North to jungle5.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The CODE CARD.

The Death in S1 isa location Name 'Death in S'
  Description ""
  Exit North to Lake in SW1.
  End exit.
  Exit West to Monster chases you.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Death in S1.

The End of Path isa location Name 'End of Path'
  Description ""
  Exit North to Rock Tree.
  End exit.
  Exit Up to Mountain.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The End of Path.

The Mountain isa location Name 'Mountain'
  Description ""
  Exit North to Mountain2.
  End exit.
  Exit Down to End of Path.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mountain.

The Mountain2 isa location Name 'Mountain'
  Description ""
  Exit North to CAVE.
  End exit.
  Exit South to Mountain.
  End exit.

  Exit East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Mountain2.

The CAVE isa location Name 'CAVE'
  Description ""
  Exit South to Mountain2.
  End exit.

  Exit North East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The CAVE.

The GET STONE WITH BONE isa thing at CAVE.
  IsDisplayedAs GET STONE WITH BONE.
End The GET STONE WITH BONE.

The Under ship body isa location Name 'Under ship body'
  Description ""
  Exit South to BAG.
  End exit.
  Exit East to Under ship tail.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Under ship body.

The BAG isa location Name 'BAG'
  Description ""
  Exit North to Under ship body.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The BAG.

The Under ship tail isa location Name 'Under ship tail'
  Description ""
  Exit South to Wall W E open.
  End exit.
  Exit East to Scenic.
  End exit.
  Exit West to Under ship body.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Under ship tail.

The Scenic isa location Name 'Scenic'
  Description ""
  Exit South to Tree Root NE.
  End exit.
  Exit East to Scenic2.
  End exit.
  Exit West to Under ship tail.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scenic.

The Scenic2 isa location Name 'Scenic'
  Description ""
  Exit South to Tree Root NW.
  End exit.
  Exit East to Scenic3.
  End exit.
  Exit West to Scenic.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scenic2.

The Scenic3 isa location Name 'Scenic'
  Description ""
  Exit South to Big Vine W.
  End exit.
  Exit East to Hidden Psg West.
  End exit.
  Exit West to Scenic2.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scenic3.

The Hidden Psg West isa location Name 'Hidden Psg West'
  Description ""
  Exit South to Wall E.
  End exit.
  Exit East to Hidden Psg East.
  End exit.
  Exit West to Scenic3.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hidden Psg West.

The Hidden Psg East isa location Name 'Hidden Psg East'
  Description ""
  Exit South to Black Wall West.
  End exit.
  Exit East to Scenic4.
  End exit.
  Exit West to Hidden Psg West.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hidden Psg East.

The Scenic4 isa location Name 'Scenic'
  Description ""
  Exit South to Sea Weed.
  End exit.
  Exit East to Fork North.
  End exit.
  Exit West to Hidden Psg East.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Scenic4.

The Fork North isa location Name 'Fork North'
  Description ""
  Exit North to TENT ACLE.
  End exit.
  Exit South to Brown Tree.
  End exit.
  Exit East to Tiny Tooth.
  End exit.
  Exit West to Scenic4.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Fork North.

The Tiny Tooth isa location Name 'Tiny Tooth'
  Description ""
  Exit South to Gel Blob.
  End exit.
  Exit East to Turtle.
  End exit.
  Exit West to Fork North.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tiny Tooth.

The Turtle isa location Name 'Turtle'
  Description ""
  Exit East to Jelly Fish.
  End exit.
  Exit West to Tiny Tooth.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Turtle.

The Jelly Fish isa location Name 'Jelly Fish'
  Description ""
  Exit East to METAL ROD.
  End exit.
  Exit West to Turtle.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Jelly Fish.

The METAL ROD isa location Name 'METAL ROD'
  Description ""
  Exit East to Cavern.
  End exit.
  Exit West to Jelly Fish.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The METAL ROD.

The Cavern isa location Name 'Cavern'
  Description ""
  Exit East to Cavern2.
  End exit.
  Exit West to METAL ROD.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cavern.

The Cavern2 isa location Name 'Cavern'
  Description ""
  Exit North to Pool.
  End exit.
  Exit West to Cavern.
  End exit.

  Exit South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Cavern2.

The Pool isa location Name 'Pool'
  Description ""
  Exit South to Cavern2.
  End exit.
  Exit West to Puffy Fish.
  End exit.

  Exit North East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Pool.

The Puffy Fish isa location Name 'Puffy Fish'
  Description ""
  Exit East to Pool.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Puffy Fish.

The Gel Blob isa location Name 'Gel Blob'
  Description ""
  Exit North to Tiny Tooth.
  End exit.
  Exit South to Clams.
  End exit.
  Exit West to Brown Tree.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Gel Blob.

The Brown Tree isa location Name 'Brown Tree'
  Description ""
  Exit North to Fork North.
  End exit.
  Exit South to Clams.
  End exit.
  Exit East to Gel Blob.
  End exit.
  Exit West to Sea Weed.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Brown Tree.

The Clams isa location Name 'Clams'
  Description ""
  Exit North to Brown Tree.
  End exit.
  Exit South to Trunk Algae.
  End exit.
  Exit West to Clams2.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams.

The Clams2 isa location Name 'Clams'
  Description ""
  Exit North to Sea Weed.
  End exit.
  Exit South to TREE E.
  End exit.
  Exit East to Clams.
  End exit.
  Exit West to Clams3.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams2.

The Clams3 isa location Name 'Clams'
  Description ""
  Exit North to Black Wall West.
  End exit.
  Exit East to Clams2.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams3.

The Black Wall West isa location Name 'Black Wall West'
  Description ""
  Exit North to Hidden Psg East.
  End exit.
  Exit South to Clams3.
  End exit.
  Exit East to Sea Weed.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Black Wall West.

The Sea Weed isa location Name 'Sea Weed'
  Description ""
  Exit North to Scenic4.
  End exit.
  Exit South to Clams2.
  End exit.
  Exit East to Brown Tree.
  End exit.
  Exit West to Black Wall West.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Sea Weed.

The TREE E isa location Name 'TREE (E)'
  Description ""
  Exit North to Clams2.
  End exit.
  Exit South to Clams5.
  End exit.
  Exit East to Trunk Algae.
  End exit.
  Exit West to TREE Search.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The TREE E.

The Trunk Algae isa location Name 'Trunk/ Algae'
  Description ""
  Exit North to Clams.
  End exit.
  Exit South to Clams5.
  End exit.
  Exit East to Struc tures.
  End exit.
  Exit West to TREE E.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Trunk Algae.

The Struc tures isa location Name 'Struc tures'
  Description ""
  Exit North to Clams6.
  End exit.
  Exit East to OCTO PUS.
  End exit.
  Exit West to Trunk Algae.
  End exit.

  Exit South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Struc tures.

The OCTO PUS isa location Name 'OCTO PUS'
  Description ""
  Exit West to Struc tures.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The OCTO PUS.

The BONE isa thing at OCTO PUS.
  IsDisplayedAs BONE.
End The BONE.

The TREE Search isa location Name 'TREE (Search)'
  Description ""
  Exit South to Clams5.
  End exit.
  Exit East to TREE E.
  End exit.
  Exit West to Wall E  Algae.
  End exit.

  Exit North Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The TREE Search.

The SEARCH HOLE isa thing at TREE Search.
  IsDisplayedAs SEARCH HOLE.
End The SEARCH HOLE.

The Wall E  Algae isa location Name 'Wall E + Algae'
  Description ""
  Exit North to Tree W  Wall.
  End exit.
  Exit South to Eels.
  End exit.
  Exit East to TREE Search.
  End exit.
  Exit West to Coral PULL.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wall E  Algae.

The S to algae isa thing at Wall E  Algae.
  IsDisplayedAs S to algae.
End The S to algae.

The Coral PULL isa location Name 'Coral (PULL)'
  Description ""
  Exit North to NW to treas ure.
  End exit.
  Exit South to Eels.
  End exit.
  Exit East to Wall E  Algae.
  End exit.
  Exit West to Vine and Trunk1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Coral PULL.

The Vine and Trunk1 isa location Name 'Vine and Trunk'
  Description ""
  Exit North to Trunk wNE Hole.
  End exit.
  Exit South to Eels.
  End exit.
  Exit East to Coral PULL.
  End exit.
  Exit West to Vine in East1.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Vine and Trunk1.

The Vine in East1 isa location Name 'Vine in East'
  Description ""
  Exit North to 2 root edges.
  End exit.
  Exit South to Clams4.
  End exit.
  Exit East to Vine and Trunk1.
  End exit.
  Exit West to Turtle2.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Vine in East1.

The Turtle2 isa location Name 'Turtle'
  Description ""
  Exit North to Wall W E Trunk.
  End exit.
  Exit South to Clams4.
  End exit.
  Exit East to Vine in East1.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Turtle2.

The Clams4 isa location Name 'Clams'
  Description ""
  Exit North to Turtle2.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams4.

The Clams5 isa location Name 'Clams'
  Description ""
  Exit North to TREE E.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams5.

The Wall W E Trunk isa location Name 'Wall W, E Trunk'
  Description ""
  Exit North to Wall W E open.
  End exit.
  Exit South to Turtle2.
  End exit.
  Exit East to 2 root edges.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wall W E Trunk.

The Wall W E open isa location Name 'Wall W, E open'
  Description ""
  Exit North to Under ship tail.
  End exit.
  Exit South to Wall W E Trunk.
  End exit.
  Exit East to Tree Root NE.
  End exit.

  Exit West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wall W E open.

The Tree Root NE isa location Name 'Tree Root NE'
  Description ""
  Exit North to Scenic.
  End exit.
  Exit South to 2 root edges.
  End exit.
  Exit East to Tree Root NW.
  End exit.
  Exit West to Wall W E open.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree Root NE.

The Tree Root NW isa location Name 'Tree Root NW'
  Description ""
  Exit North to Scenic2.
  End exit.
  Exit South to Trunk wNE Hole.
  End exit.
  Exit East to Big Vine W.
  End exit.
  Exit West to Tree Root NE.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree Root NW.

The Big Vine W isa location Name 'Big Vine W'
  Description ""
  Exit North to Scenic3.
  End exit.
  Exit South to NW to treas ure.
  End exit.
  Exit East to Wall E.
  End exit.
  Exit West to Tree Root NW.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Big Vine W.

The Wall E isa location Name 'Wall E'
  Description ""
  Exit North to Hidden Psg West.
  End exit.
  Exit South to Tree W  Wall.
  End exit.
  Exit West to Big Vine W.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Wall E.

The 2 root edges isa location Name '2 root edges'
  Description ""
  Exit North to Tree Root NE.
  End exit.
  Exit South to Vine in East1.
  End exit.
  Exit East to Trunk wNE Hole.
  End exit.
  Exit West to Wall W E Trunk.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The 2 root edges.

The Trunk wNE Hole isa location Name 'Trunk w/NE Hole'
  Description ""
  Exit North to Tree Root NW.
  End exit.
  Exit South to Vine and Trunk1.
  End exit.
  Exit East to NW to treas ure.
  End exit.
  Exit West to 2 root edges.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Trunk wNE Hole.

The NW to treas ure isa location Name 'NW to treas ure'
  Description ""
  Exit North to Big Vine W.
  End exit.
  Exit South to Coral PULL.
  End exit.
  Exit East to Tree W  Wall.
  End exit.
  Exit West to Trunk wNE Hole.
  End exit.

  Exit Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The NW to treas ure.

The Tree W  Wall isa location Name 'Tree W + Wall'
  Description ""
  Exit North to Wall E.
  End exit.
  Exit South to Wall E  Algae.
  End exit.
  Exit West to NW to treas ure.
  End exit.

  Exit East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Tree W  Wall.

The Water Cave isa location Name 'Water Cave'
  Description ""
  Exit South to Water Cave3.
  End exit.
  Exit East to Water Cave2.
  End exit.

  Exit North West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water Cave.

The Water Cave2 isa location Name 'Water Cave'
  Description ""
  Exit East to updown psgs.
  End exit.
  Exit West to Water Cave.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water Cave2.

The Water Cave3 isa location Name 'Water Cave'
  Description ""
  Exit North to Water Cave.
  End exit.
  Exit East to Water Cave4.
  End exit.

  Exit South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water Cave3.

The Water Cave4 isa location Name 'Water Cave'
  Description ""
  Exit East to Ship wcard.
  End exit.
  Exit West to Water Cave3.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Water Cave4.

The Ship wcard isa location Name 'Ship w/card'
  Description ""
  Exit West to Water Cave4.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ship wcard.

The updown psgs isa location Name 'up/down psgs'
  Description ""
  Exit East to Upper Psg.
  End exit.
  Exit West to Water Cave2.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The updown psgs.

The Ten Tacle isa location Name 'Ten Tacle'
  Description ""
  Exit East to updown psgs.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Ten Tacle.

The Hid Psg East isa location Name 'Hid Psg East'
  Description ""
  Exit East to Oracle.
  End exit.
  Exit West to Ten Tacle.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Hid Psg East.

The Oracle isa location Name 'Oracle'
  Description ""
  Exit West to Hid Psg East.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Oracle.

The TouchExamine isa thing at Oracle.
  IsDisplayedAs Touch/Examine.
End The TouchExamine.

The Shark isa location Name 'Shark'
  Description ""
  Exit East to Lower Psg.
  End exit.

  Exit North South West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Shark.

The Use fruit isa thing at Shark.
  IsDisplayedAs Use fruit.
End The Use fruit.

The Get paper isa thing at Shark.
  IsDisplayedAs Get paper.
End The Get paper.

The Upper Psg isa location Name 'Upper Psg'
  Description ""
  Exit East to Upper Psg2.
  End exit.
  Exit West to updown psgs.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Upper Psg.

The Upper Psg2 isa location Name 'Upper Psg'
  Description ""
  Exit East to UD Hole.
  End exit.
  Exit West to Upper Psg.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Upper Psg2.

The UD Hole isa location Name 'U/D Hole'
  Description ""
  Exit West to Upper Psg2.
  End exit.

  Exit North South East Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The UD Hole.

The Monster chases you2 isa location Name 'Monster chases you'
  Description ""
  Exit North to Ship Body.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Monster chases you2.

The TENT ACLE isa location Name 'TENT ACLE'
  Description ""
  Exit East to Fork North.
  End exit.
  Exit West to updown psgs.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The TENT ACLE.

The From N of Coral isa location Name 'From N of Coral'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The From N of Coral.

The Line Jar isa thing at From N of Coral.
  IsDisplayedAs Line, Jar.
End The Line Jar.

The Clams6 isa location Name 'Clams'
  Description ""

  Exit North South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Clams6.

The Eels isa location Name 'Eels'
  Description ""
  Exit North to Coral PULL.
  End exit.

  Exit South East West Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Eels.

The Lower Psg isa location Name 'Lower Psg'
  Description ""
  Exit East to Lower Psg2.
  End exit.
  Exit West to Shark.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lower Psg.

The Lower Psg2 isa location Name 'Lower Psg'
  Description ""
  Exit East to UD Hole.
  End exit.
  Exit West to Lower Psg.
  End exit.

  Exit North South Northeast Southeast Southwest Northwest Up Down Inside Outside  to nowhere
    Check
      "You can't go that way."
  End exit.
end The Lower Psg2.

