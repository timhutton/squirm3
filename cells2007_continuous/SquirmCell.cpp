// SquirmCell.cpp

// local:
#include "SquirmCell.h"
#include "SquirmCellSlot.h"
#include "SquirmError.h"

// stdlib:
#include <math.h>
#include <stdlib.h>

// STL:
#include <algorithm>
using namespace std;

SquirmCell::SquirmCell(C2dVector loc,char type,int state)
     : SquirmCellProperties(type,state)
{
    this->location = loc;
    AssignRandomVelocity();
}

void SquirmCell::AssignRandomVelocity()
{
    // initialize with a random velocity
    float angle = rand()*2.0F*3.1415926535F/RAND_MAX;
    this->velocity.x = MAX_VELOCITY * (float)cos(angle);
    this->velocity.y = MAX_VELOCITY * (float)sin(angle);
}

SquirmCell* SquirmCell::BondTo(SquirmCell* cell) 
{
    this->MakeBondWith(cell);
    cell->MakeBondWith(this);
    return this;
}

void SquirmCell::Debond(SquirmCell* cell)
{
    vector<SquirmCell*>::iterator pos;

    pos = find(this->bonded_cells.begin(),this->bonded_cells.end(),cell);
    if(pos!=this->bonded_cells.end()) 
        this->bonded_cells.erase(pos);
    else 
        SquirmError("Debond : not bonded to this cell!");

    pos = find(cell->bonded_cells.begin(),cell->bonded_cells.end(),this);
    if(pos!=cell->bonded_cells.end()) 
        cell->bonded_cells.erase(pos);
    else 
        SquirmError("Debond : cell not bonded to us!");
}

bool SquirmCell::HasBondWith(SquirmCell *cell)
{
    return find(this->bonded_cells.begin(),this->bonded_cells.end(),cell) != this->bonded_cells.end();
}

void SquirmCell::BreakAllBonds()
{
    while(!bonded_cells.empty())
    {
        SquirmCell *cell=bonded_cells.front();
        Debond(cell);
    }
}

void SquirmCell::MakeBondWith(SquirmCell *cell) 
{ 
    if(this->HasBondWith(cell))
        SquirmError("MakeBondWith - already bonded");
    this->bonded_cells.push_back(cell); 
}

