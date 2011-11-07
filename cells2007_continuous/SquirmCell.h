// SquirmCell.h

// local
#include "SquirmCellProperties.h"
#include "C2dVector.h"

// STL:
#include <vector>

const float RADIUS = 6.0F;
const float MAX_VELOCITY = RADIUS*0.4F;

class SquirmCell : public SquirmCellProperties
{
public:

    C2dVector location;

    C2dVector velocity;

    SquirmCell(C2dVector loc,char type,int state);

    std::vector<SquirmCell*>& GetBondedCells() { return this->bonded_cells; }

    SquirmCell* BondTo(SquirmCell* cell);
    void Debond(SquirmCell* cell);
    bool HasBondWith(SquirmCell *cell);
    void BreakAllBonds();
    void AssignRandomVelocity();

protected:

    void MakeBondWith(SquirmCell *cell);

protected:

    std::vector<SquirmCell*> bonded_cells;
};
