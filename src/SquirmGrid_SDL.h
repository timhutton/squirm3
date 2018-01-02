// local:
#include "SquirmGrid.h"

// SDL2:
#include <SDL2/SDL.h>

class SquirmGrid_SDL : public SquirmGrid
{
public:

    void Draw(SDL_Renderer* renderer,float scale);

};
