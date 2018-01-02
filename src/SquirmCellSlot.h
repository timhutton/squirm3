// SquirmCellSlot.h

// STL:
#include <vector>

class SquirmCell;

class SquirmCellSlot {

public:

    void addOccupant(SquirmCell* occ);

    void removeOccupant(SquirmCell* occ);

    bool queryEmpty();

    std::vector<SquirmCell*> occupants;
};
