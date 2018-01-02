// SquirmChemistry.h

// local:
#include "SquirmReaction.h"
#include "SquirmCell.h"
#include "SquirmError.h"

// stdlib:
#include <stdlib.h>

// STL:
#include <vector>

inline bool TestProb(unsigned long cases)
{
    if(cases==0)
        SquirmError("Zero passed to TestProb!");
    return((rand()+(unsigned long)rand()*RAND_MAX)%cases==0);
}

class SquirmChemistry
{

public:
    
    void removeAllReactions()
    {
        reactions.clear();
    }

    void addReaction(SquirmReaction r)
    {
        reactions.push_back(r);
    }

    int react(SquirmCell *cell,const std::vector<SquirmCell*> &nearby_cells,float REACTION_RANGE2);

private:

    bool tryReaction(SquirmCell *cell,
        const std::vector<SquirmCell*>& nearby_cells,
        SquirmReaction r,float REACTION_RANGE2);

private:
    std::vector<SquirmReaction> reactions;

};

