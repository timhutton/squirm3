// SquirmCellSlot.cpp

//local
#include "SquirmCellSlot.h"
#include "SquirmCell.h"
#include "SquirmError.h"

// STL:
#include <algorithm>

using namespace std;

void SquirmCellSlot::addOccupant(SquirmCell *occ)
{
    // mustn't already be in this slot
    vector<SquirmCell*>::iterator where = find(this->occupants.begin(),this->occupants.end(),occ);
    if(where!=this->occupants.end())
        SquirmError("SquirmCellSlot::addOccupant - occupant already present");
    this->occupants.push_back(occ);
}
 
void SquirmCellSlot::removeOccupant(SquirmCell* occ)
{
    // must already be in this slot
    vector<SquirmCell*>::iterator where = find(this->occupants.begin(),this->occupants.end(),occ);
    if(where==this->occupants.end())
        SquirmError("SquirmCellSlot::removeOccupant - occupant not present");
    this->occupants.erase(where);
}

bool SquirmCellSlot::queryEmpty() 
{ 
    return this->occupants.empty(); 
}

