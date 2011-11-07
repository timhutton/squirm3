// SquirmReaction.h

// STL:
#include <string>

class SquirmReaction
{
public:
    char a_type,b_type,c_type;
    int a_state,b_state,c_state;
    int future_a_state,future_b_state,future_c_state;
    bool current_ab_bond,current_bc_bond,current_ac_bond;
    bool future_ab_bond,future_bc_bond,future_ac_bond;

    bool assign_enzyme_of_a;

    unsigned long cases; // 1 in X chance of reaction occurring
    int n_inputs;

    SquirmReaction() {}
    SquirmReaction(const SquirmReaction& r)
    {
        *this = r;
    }

    std::string GetString() const;

    SquirmReaction& operator=(const SquirmReaction& r)
    {
        this->a_type=r.a_type;
        this->a_state=r.a_state;
        this->current_ab_bond=r.current_ab_bond;
        this->b_type=r.b_type;
        this->b_state=r.b_state;
        this->current_bc_bond=r.current_bc_bond;
        this->c_type=r.c_type;
        this->c_state=r.c_state;
        this->current_ac_bond=r.current_ac_bond;
        this->future_a_state=r.future_a_state;
        this->future_ab_bond=r.future_ab_bond;
        this->future_b_state=r.future_b_state;
        this->future_bc_bond=r.future_bc_bond;
        this->future_c_state=r.future_c_state;
        this->future_ac_bond=r.future_ac_bond;
        this->cases = r.cases;
        this->n_inputs=r.n_inputs;
        this->assign_enzyme_of_a = r.assign_enzyme_of_a;

        return *this;
    }

    SquirmReaction(char a_type,int a_state,bool current_ab_bond,
            char b_type,int b_state,bool current_bc_bond,
            char c_type,int c_state,bool current_ac_bond,
            int future_a_state,bool future_ab_bond,
            int future_b_state,bool future_bc_bond,
            int future_c_state,bool future_ac_bond)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->current_bc_bond=current_bc_bond;
        this->c_type=c_type;
        this->c_state=c_state;
        this->current_ac_bond=current_ac_bond;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->future_bc_bond=future_bc_bond;
        this->future_c_state=future_c_state;
        this->future_ac_bond=future_ac_bond;
        this->cases=1; // happens every time 
        this->n_inputs=3;
        this->assign_enzyme_of_a=false; // only applicable to 2-way reactions anyway
    }

    SquirmReaction(char a_type,int a_state,bool current_ab_bond,
            char b_type,int b_state,bool current_bc_bond,
            char c_type,int c_state,bool current_ac_bond,
            int future_a_state,bool future_ab_bond,
            int future_b_state,bool future_bc_bond,
            int future_c_state,bool future_ac_bond,
            unsigned long cases)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->current_bc_bond=current_bc_bond;
        this->c_type=c_type;
        this->c_state=c_state;
        this->current_ac_bond=current_ac_bond;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->future_bc_bond=future_bc_bond;
        this->future_c_state=future_c_state;
        this->future_ac_bond=future_ac_bond;
        this->cases=cases;
        this->n_inputs=3;
        this->assign_enzyme_of_a=false;
    }

    SquirmReaction(char a_type,int a_state,bool current_ab_bond,char b_type,int b_state,
            int future_a_state,bool future_ab_bond,int future_b_state,bool aeoa)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->cases=1; // happens every time 
        this->n_inputs=2;
        this->assign_enzyme_of_a=aeoa;
    }

    SquirmReaction(char a_type,int a_state,bool current_ab_bond,char b_type,int b_state,
            int future_a_state,bool future_ab_bond,int future_b_state,bool aeoa,
            unsigned long cases)
    {
        this->a_type=a_type;
        this->a_state=a_state;
        this->current_ab_bond=current_ab_bond;
        this->b_type=b_type;
        this->b_state=b_state;
        this->future_a_state=future_a_state;
        this->future_ab_bond=future_ab_bond;
        this->future_b_state=future_b_state;
        this->cases=cases;
        this->n_inputs=2;
        this->assign_enzyme_of_a=aeoa;
    }
};

