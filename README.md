# Project & Portfolio 1

Hello my name is William Alfonso. I am a student from FullsailUniveristy. The purpose of this repository is to practice development using version control. This work will help me begin to build a portfolio of skills and accomplishment that can be shared in the future.

<br>

### Week 1

Replace this paragraph with your stand up for this week. Use the prompts above to summarize your most recent milestone activity and work.

### Week 2

My next stand up will go here...

### Week 3

🔎 Overview

This week I added enemies and combat to my dungeon crawler. The player can now fight goblins by walking into them, heal with potions, and lose the game if HP reaches 0. I built each feature in small steps, tested it, and committed it on my dev branch.

Improvements
Created an Enemy class with its own .h and .cpp files
Refactored Game::play() into handleCommand(), move(), and checkEndConditions() so each function does one job
Replaced the duplicated “invalid command” message with a single shared one
Added a MAX_HEALTH constant so healing can’t go over the maximum
Updated the README and added planning and change documents in /docs

Challenges
Visual Studio errors after adding new files, fixed by adding Enemy.cpp to the project and including the missing getTilePtr in Grid
Git permission errors
Balancing combat 

Accomplishments
I got more comfortable with pointers. getTilePtr() returns a Tile* (or nullptr) so I can edit a tile in place, and Game owns the Grid through a unique_ptr.
I learned to find enemies by index instead of pointer, because erasing from a vector can leave pointers dangling.
I improved my Git workflow with small, frequent commits on dev, and a pull request to main.

Next Steps
Add a second level or room]
Add different enemy types]
Test edge cases and fix bugs
Final cleanup, comments, and documentation

### Week 4

This week I finished the final features of my dungeon crawler. The player can fight goblins, heal with potions, win by reaching the exit, or lose when HP reaches 0. I also completed the main menu, instructions, and replay system.

Improvements
- Refactored `Game::play()` into smaller functions.
- Added `Enemy` class files and improved pointer usage with `Enemy*` and `Tile*`.
- Added constants for health, healing, and other values.
- Improved input validation, status updates, and combat messages.

Challenges
- Preventing dangling pointers when removing defeated enemies.
- Fixing invalid input and out-of-bounds errors.
- Making sure player stats and the dungeon reset between runs.

Accomplishments
I improved my understanding of pointers, references, `unique_ptr`, and safe enemy removal. I also made the game more reliable by testing edge cases and fixing bugs.
