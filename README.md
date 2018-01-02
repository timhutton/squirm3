Live demo: https://timhutton.github.io/squirm3

This is one of the systems described in a 2007 paper:

Hutton T.J. (2007) Evolvable Self-Reproducing Cells in a Two-Dimensional Artificial Chemistry. Artificial Life 13(1): 11-30. [PDF](http://www.sq3.org.uk/papers/cells2007.pdf)

Squirm3 is a simple kind of artificial chemistry, where 'atoms' have a type (a, b, c, d, ...) and a state (0, 1, 2, 3, ...) and can react with each other, forming bonds. The atoms move around in a world, forming molecules and membranes. With the right reactions in place, molecules can replicate through template-copying.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=VrTM6wYl4Us' target='_blank'><img src='http://img.youtube.com/vi/VrTM6wYl4Us/0.jpg' width='425' height=344 /> video</a>

Build instructions:
-------------------

Build with Emscripten:

```emcc src\main.cpp src\C2dVector.cpp src\SquirmCell.cpp src\SquirmCellProperties.cpp src\SquirmCellSlot.cpp src\SquirmChemistry.cpp src\SquirmGrid.cpp src\SquirmReaction.cpp src\SquirmError.cpp src\SquirmGrid_SDL.cpp -s WASM=1 -s USE_SDL=2 -std=c++11 -O3 -o index.js```

Then open index.html in a web browser.
