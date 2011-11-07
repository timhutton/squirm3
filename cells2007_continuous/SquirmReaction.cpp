
// local:
#include "SquirmReaction.h"

// STL:
#include <sstream>
using namespace std;

string SquirmReaction::GetString() const
{
    if(this->n_inputs==2)
    {
        ostringstream oss;
        oss << this->a_type << this->a_state 
            << string(this->current_ab_bond?"-":" + ")
            << this->b_type 
            << this->b_state 
            << " -> "
            << this->a_type << this->future_a_state 
            << string(this->future_ab_bond?"-":" + ")
            << this->b_type
            << this->future_b_state;
        return oss.str();
    }
    else return "not yet implemented";
}
