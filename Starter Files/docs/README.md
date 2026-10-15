Week 2 Changes

- WASD movement with wall collision
- Treasure (`$`) and potion (`!`) tiles that can be picked up
- Win condition at the exit (`E`)
- Status line showing HP, score, potions, and steps

- `Tile.h`: added `Treasure` and `Potion`
- `Player.h`: added score, potions, steps, and `reset()`
- `Game`: added `tryMove()` for collision and `handleTile()` for pickups and the exit

- Invalid movement input shows an error and re-prompts
- Feedback message under the map (e.g. "A wall blocks your way.")
- Instructions screen explains the controls and symbols

- Collision is checked in `Game` because `Grid` knows what is walkable, which keeps `Player` simple.
- Pickup is separate from movement so each function does one job and is easier to extend in Week 3.
- Collected items turn back into `Floor` so they can't be collected twice.








