// SquirmChemistry.cpp

//local
#include "SquirmChemistry.h"

// stdlib:
#include <math.h>

using namespace std;

int SquirmChemistry::react(SquirmCell *cell,const vector<SquirmCell*> &nearby_cells,float REACTION_RANGE2)
{
    // try the in-built reactions
    for(int i=0;i<(int)reactions.size();i++)
    {
        if(tryReaction(cell,nearby_cells,reactions[i],REACTION_RANGE2))
            return i;
        // only one reaction should apply per iteration
        // (also, unbonded_neighbours may now be wrong)
    }

    return -1; // no reaction could be applied
}

bool SquirmChemistry::tryReaction(SquirmCell *cell,
    const vector<SquirmCell*>& nearby_cells,
    SquirmReaction r,float REACTION_RANGE2)
{
    // can this reaction apply to this cell based on the cells it is 
    // bonded to and its unbonded neighbours?

    // we are checking this for every atom, therefore it suffices here to
    // check whether this reaction applies to this atom as the 'a' atom

    // --------------

    // reject cell if it does not match the specification for a

    if(r.a_type!='x' && r.a_type!='y' && r.a_type!='z' && r.a_type!=cell->GetType())
        return false;

    if(r.a_state!=cell->GetState())
        return false;

    {   
        // work through the list of cells to find a candidate for reactant b

        // for each b candidate (if any) can we find a suitable c?
        // apply the reaction and return if we find one
        for(vector<SquirmCell*>::const_iterator b_it = nearby_cells.begin();b_it!=nearby_cells.end();b_it++)
        {
            SquirmCell *b_candidate = *b_it;
        
            // b_candidate cannot be the same cell as a
            if(b_candidate == cell)
                continue;

            // its state must be correct
            if(r.b_state != b_candidate->GetState())
                continue;

            // its type, if specified, must be correct
            if(r.b_type!='x' && r.b_type!='y' && r.b_type!='z' && r.b_type!=b_candidate->GetType())
                continue;

            // if the type is specified as being equal to that of a, then reject if it isn't
            if( (r.b_type=='x' || r.b_type=='y' || r.b_type=='z') && r.b_type==r.a_type &&
                b_candidate->GetType()!=cell->GetType())
                continue;

            // reject this b_candidate if its bond/not with cell is wrong
            if( (r.current_ab_bond && !cell->HasBondWith(b_candidate)) || (!r.current_ab_bond && cell->HasBondWith(b_candidate)) )
                continue;

            if(r.n_inputs==3)
            {
                // likewise, consider each candidate c in turn
                for(vector<SquirmCell*>::const_iterator c_it=nearby_cells.begin();c_it!=nearby_cells.end();c_it++)
                {
                    SquirmCell *c_candidate = *c_it;
                    
                    // cannot be the same candidate cell as b or a
                    if(c_candidate==b_candidate || c_candidate==cell)
                        continue;

                    // its state must be correct
                    if(r.c_state != c_candidate->GetState())
                        continue;

                    // its type, if specified, must be correct
                    if(r.c_type!='x' && r.c_type!='y' && r.c_type!='z' && r.c_type!=c_candidate->GetType())
                        continue;

                    // if its type is specified as being equal to that of a and isn't then reject
                    if( (r.c_type=='x' || r.c_type=='y' || r.c_type=='z') && r.c_type==r.a_type &&
                        c_candidate->GetType() != cell->GetType())
                        continue;

                    // if its type is specified as being equal to that of b and isn't then reject
                    if( (r.c_type=='x' || r.c_type=='y' || r.c_type=='z') && r.c_type==r.b_type &&
                        c_candidate->GetType() != b_candidate->GetType())
                        continue;

                    // reject this c_candidate if its bond/not with cell is wrong
                    if( (r.current_ac_bond && !cell->HasBondWith(c_candidate)) || (!r.current_ac_bond && cell->HasBondWith(c_candidate)) )
                        continue;

                    // also the bond/not between b and c must be correct
                    if( (r.current_bc_bond && !b_candidate->HasBondWith(c_candidate)) || (!r.current_bc_bond && b_candidate->HasBondWith(c_candidate)) )
                        continue;

                    // all the players must be within range
                    if(!(Length2(c_candidate->location-b_candidate->location)<REACTION_RANGE2))
                        continue;

                    // one last check before applying the reaction
                    if(!TestProb(r.cases))
                        continue;

                    // we can now apply the reaction
                    cell->SetState(r.future_a_state);
                    b_candidate->SetState(r.future_b_state);
                    c_candidate->SetState(r.future_c_state);
                    if(r.current_ab_bond && !r.future_ab_bond)
                        cell->Debond(b_candidate);
                    else if(!r.current_ab_bond && r.future_ab_bond)
                        cell->BondTo(b_candidate);
                    if(r.current_bc_bond && !r.future_bc_bond)
                        b_candidate->Debond(c_candidate);
                    else if(!r.current_bc_bond && r.future_bc_bond)
                        b_candidate->BondTo(c_candidate);
                    if(r.current_ac_bond && !r.future_ac_bond)
                        cell->Debond(c_candidate);
                    else if(!r.current_ac_bond && r.future_ac_bond)
                        cell->BondTo(c_candidate);

                    return true;
                }
            }
            else // n_inputs==2
            {
                if(!TestProb(r.cases))
                    continue;

                // apply the reaction and return
                cell->SetState(r.future_a_state);
                b_candidate->SetState(r.future_b_state);
                if(cell->HasBondWith(b_candidate) && !r.future_ab_bond)
                    cell->Debond(b_candidate);
                else if(!cell->HasBondWith(b_candidate) && r.future_ab_bond)
                    cell->BondTo(b_candidate);
                return true;
            }
        }
    }

    return false; // this reaction was not possible

}

