Live demo: https://timhutton.github.io/squirm3

Squirm3 is a simple kind of artificial chemistry, where 'atoms' have a type (a, b, c, d, ...) and a state (0, 1, 2, 3, ...) and can react with each other, forming bonds. The atoms move around in a world, forming molecules and membranes. With the right reactions in place, molecules can replicate through template-copying.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=VrTM6wYl4Us' target='_blank'><img src='http://img.youtube.com/vi/VrTM6wYl4Us/0.jpg' width='425' height=344 /> video</a>

Build
-----

Using Emscripten:

```emcc src\main.cpp src\C2dVector.cpp src\SquirmCell.cpp src\SquirmCellProperties.cpp src\SquirmCellSlot.cpp src\SquirmChemistry.cpp src\SquirmGrid.cpp src\SquirmReaction.cpp src\SquirmError.cpp src\SquirmGrid_SDL.cpp -s WASM=1 -s USE_SDL=2 -std=c++11 -O3 -o index.js```

Then open index.html in a web browser.
