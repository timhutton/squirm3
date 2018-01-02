// local:
#include "SquirmGrid_SDL.h"

// STL:
#include <vector>
#include <sstream>
using namespace std;

SDL_Color GetColor(char type)
{
    switch(type)
    {
        default:
        case 'e': return {255,0,0}; // red
        case 'f': return {0,255,0}; // green
        case 'a': return {220,220,0}; // yellow
        case 'b': return {128,128,128}; // gray
        case 'c': return {0,255,255}; // cyan
        case 'd': return {0,0,255}; // blue
    }
}

void SquirmGrid_SDL::Draw(SDL_Renderer* renderer,float scale)
{
    // clear the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // draw the atoms
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for(int pass=0;pass<2;pass++) // we want unbonded atoms to appear faint and behind (as if 2.5D)
    {
        for(vector<SquirmCell*>::const_iterator it = this->cell_list.begin();it!=this->cell_list.end();it++)
        {
            SquirmCell *cell = *it;
            // draw the cell
            SDL_Color color = GetColor(cell->GetType());
            int alpha = 0;
            if(cell->GetState()==0)
            {
                // background atom
                // skip these in the second pass
                if(pass==1) continue;
                // make mostly transparent
                alpha = 10;
            }
            else 
            {
                // foreground atom
                if(pass==0) continue;
                // make slightly transparent
                alpha = 200;
            }
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
            float s = scale*RADIUS*0.8;
            SDL_Rect r;
            r.x = cell->location.x*scale-s+OFFSET;
            r.y = cell->location.y*scale-s+OFFSET;
            r.w = s*2;
            r.h = s*2;
            SDL_RenderFillRect(renderer, &r);
        }
    }

    // draw the bonds
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    for(vector<SquirmCell*>::const_iterator it = this->cell_list.begin();it!=this->cell_list.end();it++)
    {
        SquirmCell *cell = *it;
        // draw the cell's bonds as lines
        for(vector<SquirmCell*>::const_iterator it2=cell->GetBondedCells().begin();it2!=cell->GetBondedCells().end();it2++)
        {
            SquirmCell *other = *it2;
            if(other<cell) continue; // (avoid drawing the same line more than once)

            SDL_RenderDrawLine(renderer, 
                int(cell->location.x*scale)+OFFSET,
                int(cell->location.y*scale)+OFFSET,
                int(other->location.x*scale)+OFFSET,
                int(other->location.y*scale)+OFFSET);
        }
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
