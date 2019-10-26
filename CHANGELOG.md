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

**Attached files:**

- `picross`: Linux executable built with `g++`.
- `libtinyxml2.so`: [**C++ XML parsing library**](https://github.com/leethomason/tinyxml2), compiled for Linux using `g++`.
- `example_grid1.xml`: XML serialized grid showing the expected structure of such files.

Refer to build instructions on [`README.md`](https://github.com/deqyra/PicrossEngine/blob/release-v0.1/README.md#building-the-project) in [branch `release-v0.1`](https://github.com/deqyra/PicrossEngine/tree/release-v0.1).