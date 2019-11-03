# [PicrossEngine v0.1](https://github.com/deqyra/PicrossEngine/releases/tag/v0.1)

Functional groundwork to build upon in future releases.

**Main additions:**

- Fully featured representation of a game state
- Flexible CLI menu/command architecture to easily build apps around
- XML serialization
  
Includes an **interactive CLI app** capable of the following:  

- Create a new grid
- Load a grid from an XML file
- Save a grid to an XML file

**Planned for next release:**

- Print grid on screen
- Modify grid through CLI app
- Tests (and maybe CI)
- Windows support (it currently builds but doesn't run)

# [PicrossEngine v0.2](https://github.com/deqyra/PicrossEngine/releases/tag/v0.2)

Fully working "game" / simulator.

**Main additions:**

- Modular build architecture
- Modify grid command, a sub-shell parsing small commands to modify a grid's state
- Comments / documentation
- Tests! (Plenty of them!) (And they're modular too!)
- Small fixes
  
The **interactive CLI app** is now capable of the following:  

- Create a new grid
- Load a grid from an XML file
- Save a grid to an XML file
- Manipulate a grid's state
- Display a grid

**Planned for next release:**

- Iterative solver
