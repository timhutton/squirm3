// SquirmGrid.h

// local:
#include "SquirmCellSlot.h"
#include "SquirmChemistry.h"

// STL:
#include <vector>
#include <string>

class SquirmGrid 
{

public:

    SquirmGrid();
    ~SquirmGrid();

    void Create(float x,float y);

    void InitSimple(int n);

    void DoTimeStep();
    unsigned long GetIterations() { return this->iterations; }

    C2dVector GetSize() { return this->size; }

    void GetAllWithinRadius(C2dVector loc,float r,std::vector<SquirmCell*> &dest);
    bool hasCollisions(SquirmCell* tester,C2dVector test_pos,float r);

    static int RandomN(int n);

    bool Valid(C2dVector p) { return(p.x>=0.0F && p.y>=0.0F && 
        p.x<=this->size.x && p.y<=this->size.y); }

    void PutCell(SquirmCell *cell);
    void RemoveCell(SquirmCell *cell);

    std::string ProbeAt(float x,float y,float scale);
    int GetOffset() { return this->OFFSET; }

    float GetTotalSpeed() { return this->total_speed; }

protected:

    float spring_force(float r2);
    float compute_force(float r2,float range,float magnitude);

    SquirmCellSlot* GetSlot(C2dVector p);

    SquirmCell* CreateNewCell(float x,float y,char type,int state)
    {
        return CreateNewCell(C2dVector(x,y),type,state);
    }

    SquirmCell* CreateNewCell(C2dVector loc,char type,int state);

    SquirmCell* CreateRandomCell();
    SquirmCell* CreateRandomCellOfType(char type);

    float RandomX();
    float RandomY();

    void RecomputeVelocitiesAndReact();
    void MoveCells();

    void DoFlood();

    void InitSearchArea(int cx,int cy,int search_slots,
                        std::vector<SquirmCell*> &central_cells,
                        std::vector<SquirmCell*> &nearby);

protected:

    SquirmCellSlot **cell_grid;
    std::vector<SquirmCell*> cell_list;

    SquirmChemistry chemistry;

    C2dVector size;
    bool created;

    unsigned long iterations;

    bool do_flood;
    int FLOOD_PERIOD;
    int flood_sector;

    int initial_population;

    FILE *output,*output2;
    bool do_output;

    float slot_size;
    int slots_X,slots_Y;

    int OFFSET;

    float lost_speed,total_speed; // things to check that our simple physics will keep working ok

};
