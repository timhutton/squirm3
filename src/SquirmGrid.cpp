// SquirmGrid.cpp

//local
#include "SquirmGrid.h"
#include "SquirmError.h"

// stdlib:
#include <math.h>
#include <stdio.h>
#include <time.h>

// STL:
#include <sstream>
using namespace std;

SquirmGrid::SquirmGrid()
{
    this->created = false;

    // -- create the reactions --

    srand((unsigned int)time(NULL));

    this->do_flood = false;
    this->FLOOD_PERIOD = 50000;
    this->flood_sector = 2; // which quarter to flood next

    this->do_output=false; // unless can open outputs
    if(false) // set to true and enter a filename below to get a log of reactions
              // (see RecomputeVelocitiesAndReact() )
    {
        this->output = fopen("log.csv","wt");
        if(this->output)
        {
            this->do_output = true;
            fprintf(this->output,"iterations,reaction,molecule\n");
        }
    }

    this->OFFSET = 0;

    this->lost_speed = 0.0F;
    this->total_speed = 0.0F;
}

void SquirmGrid::Create(float x,float y)
{
    if(this->created)
    {
        this->~SquirmGrid();
    }

    this->size.x = x;
    this->size.y = y;

    // how many slots do we need?
    this->slot_size = RADIUS*5.0F;
    this->slots_X = (int)ceil(this->size.x / this->slot_size);
    this->slots_Y = (int)ceil(this->size.y / this->slot_size);

    // create a 2D array of SquirmCellSlot's NR-stylee
    SquirmCellSlot *matrix = new SquirmCellSlot[this->slots_X*this->slots_Y];
    this->cell_grid = new SquirmCellSlot*[this->slots_X];
    for(int i = 0; i < this->slots_X; i++)
        this->cell_grid[i] = &matrix[i*this->slots_Y];

    this->created=true;

    this->iterations=0;

}

SquirmCellSlot* SquirmGrid::GetSlot(C2dVector p)
{
    int x = (int)floor(p.x/slot_size);
    int y = (int)floor(p.y/slot_size);
    if(x>=0 && y>=0 && x<this->slots_X && y<this->slots_Y)
        return &this->cell_grid[x][y];
    else
    {
        SquirmError("Error!");
        //ASSERT(false);
        return NULL;
    }
}

SquirmGrid::~SquirmGrid()
{
  delete [] *this->cell_grid;
  delete [] this->cell_grid;

  // delete all the cells
  for(vector<SquirmCell*>::const_iterator it=this->cell_list.begin();it!=this->cell_list.end();it++)
    delete *it;

  this->created=false;

  if(this->do_output)
    fclose(this->output);
}

/* old reaction scheme (neat, works well in continuous space)

        // duplication
        chemistry.addReaction(SquirmReaction('e',1,false,'e',0,4,true,3));
        chemistry.addReaction(SquirmReaction('x',3,true,'x',4,true,'y',1,false,4,true,4,true,2,true));
        chemistry.addReaction(SquirmReaction('x',2,false,'x',0,3,true,5));
        chemistry.addReaction(SquirmReaction('x',3,true,'x',5,false,'y',4,true,4,true,3,true,4,false));

        // splitting
        chemistry.addReaction(SquirmReaction('f',3,true,'f',4,6,false,6));
        chemistry.addReaction(SquirmReaction('x',6,true,'y',4,1,true,7));
        chemistry.addReaction(SquirmReaction('x',7,true,'y',7,6,false,6));

        // protein read-off (uses same 'zippering' mechanism as replication)
        chemistry.addReaction(SquirmReaction('e',6,true,'x',1,false,'x',0,false,5,true,4,true,3,true));
        chemistry.addReaction(SquirmReaction('x',6,true,'e',5,true,'x',6,false,chemistry.GetFirstEnzymeState(),false,1,true,1,false));

*/

void SquirmGrid::InitSimple(int n)
{
    const int S=39,T=40,E=41;

    // membranes are now elastic
    if(true)
    {
        // crumpled membranes lose atoms spontaneously
        chemistry.addReaction(SquirmReaction('a',S,true,'a',S,true,'a',S,false,S,false,0,false,S,true,100));
        // stretched membranes acquire atoms spontaneously
        chemistry.addReaction(SquirmReaction('a',S,true,'a',S,false,'a',0,false,S,false,S,true,S,true,100));

        // some extra ones to allow membrane growth at T places too
        chemistry.addReaction(SquirmReaction('a',S,true,'a',T,false,'a',0,false,S,false,T,true,S,true));
        chemistry.addReaction(SquirmReaction('a',T,true,'a',T,false,'a',0,false,T,false,T,true,S,true));
    }

    // initialise a replicator with the rules it needs
    if(true)
    {
        // the new set of base duplication reactions:
        SquirmReaction r1('x',2,true,'y',1,7,true,4,false);
        SquirmReaction r2('x',4,false,'y',3,5,true,7,false);
        SquirmReaction r3('x',5,false,'x',0,6,true,6,false);
        SquirmReaction r4('x',6,false,'y',7,3,true,4,false);
        SquirmReaction r5('x',6,true,'y',4,1,false,2,false);
        SquirmReaction r6('x',7,true,'y',1,2,true,2,false);

        // the standard splitting reactions
        SquirmReaction r7('x',2,true,'y',8,9,true,1,false);
        SquirmReaction r8('x',9,true,'y',9,8,false,8,false);

        // start the duplication
        SquirmReaction r9('a',T,true,'e',1,10,true,5,false);
        SquirmReaction r10('a',10,false,'e',6,T,true,3,false);
        SquirmReaction r11('e',6,true,'e',3,2,true,3,false);

        // start the splitting
        SquirmReaction r12('f',2,true,'a',T,9,true,11,false);
        SquirmReaction r13('a',11,false,'f',3,11,true,9,false);

        // start the cell division
        SquirmReaction r14('a',11,true,'a',S,11,true,12,false);
        SquirmReaction r15('f',1,false,'a',12,13,true,T,false);

        // the pulling sequence
        SquirmReaction r16('x',13,true,'y',1,14,true,15,false);
        SquirmReaction r17('a',11,false,'x',15,11,true,16,false);
        SquirmReaction r18('x',14,true,'y',16,29,true,16,false); // state 29 borrowed from later
        SquirmReaction r19('x',29,true,'a',11,17,false,11,false);
        SquirmReaction r20('x',17,true,'y',16,17,true,13,false);
        // plus
        SquirmReaction r21('x',13,true,'e',8,14,true,15,false);

        // finish the cell division
        SquirmReaction r22('e',13,true,'a',T,18,true,19,false);
        SquirmReaction r23('e',13,true,'a',19,18,true,20,false);
        SquirmReaction r24('a',20,false,'a',11,21,true,22,false);
        SquirmReaction r25('e',18,true,'a',22,35,false,23,false); // 35 borrowed from later
        SquirmReaction r26('e',18,true,'a',21,35,false,24,false);
        SquirmReaction r27('a',23,true,'a',T,25,true,26,false);
        SquirmReaction r28('a',25,true,'a',T,27,true,26,false);
        SquirmReaction r29('a',24,false,'a',26,28,true,29,false);
        SquirmReaction r30('a',29,true,'a',27,T,false,30,false);
        SquirmReaction r31('a',30,true,'a',26,31,true,T,false);
        SquirmReaction r32('a',28,true,'a',S,32,true,33,false);
        SquirmReaction r33('a',32,true,'a',S,34,true,33,false);
        SquirmReaction r34('a',33,false,'a',31,S,true,36,false);
        SquirmReaction r35('a',34,true,'a',S,35,false,S,false);
        SquirmReaction r36('a',35,true,'a',33,37,true,S,false);
        SquirmReaction r37('a',37,true,'a',36,T,false,T,false);

        if(false)
        {
            // bypass the enzyme-production sequence
            chemistry.addReaction(SquirmReaction('x',35,true,'y',17,1,true,35,false));
            chemistry.addReaction(SquirmReaction('f',35,true,'a',T,1,true,T,false));
        }

        // start the enzyme-production
        if(true)
        {
            SquirmReaction e1('x',35,true,'y',17,false,'d',0,false,1,true,38,true,E,false);
            SquirmReaction e2a('d',E/*-4*/,true,'a',38,true,'y',17,false,E/*-4*/,false,1,true,38,true);
            SquirmReaction e2b('d',E/*-4*/,true,'b',38,true,'y',17,false,E/*-4*/,false,1,true,38,true);
            SquirmReaction e2c('d',E/*-4*/,true,'c',38,true,'y',17,false,E/*-4*/,false,1,true,38,true);
            SquirmReaction e3('d',E/*-5*/,true,'d',38,false,'d',0,false,E/*-5*/,false,35,false,E/*-5*/,true);
            SquirmReaction e4('d',E/*-6*/,true,'f',38,0,false,1,false);
            chemistry.addReaction(e1);
            chemistry.addReaction(e2a);
            chemistry.addReaction(e2b);
            chemistry.addReaction(e2c);
            chemistry.addReaction(e3);
            chemistry.addReaction(e4);
        }

        chemistry.addReaction(r1);
        chemistry.addReaction(r2);
        chemistry.addReaction(r3);
        chemistry.addReaction(r4);
        chemistry.addReaction(r5);
        chemistry.addReaction(r6);
        chemistry.addReaction(r7);
        chemistry.addReaction(r8);
        chemistry.addReaction(r9);
        chemistry.addReaction(r10);
        chemistry.addReaction(r11);
        chemistry.addReaction(r12);
        chemistry.addReaction(r13);
        chemistry.addReaction(r14);
        chemistry.addReaction(r15);
        chemistry.addReaction(r16);
        chemistry.addReaction(r17);
        chemistry.addReaction(r18);
        chemistry.addReaction(r19);
        chemistry.addReaction(r20);
        chemistry.addReaction(r21);
        chemistry.addReaction(r22);
        chemistry.addReaction(r23);
        chemistry.addReaction(r24);
        chemistry.addReaction(r25);
        chemistry.addReaction(r26);
        chemistry.addReaction(r27);
        chemistry.addReaction(r28);
        chemistry.addReaction(r29);
        chemistry.addReaction(r30);
        chemistry.addReaction(r31);
        chemistry.addReaction(r32);
        chemistry.addReaction(r33);
        chemistry.addReaction(r34);
        chemistry.addReaction(r35);
        chemistry.addReaction(r36);
        chemistry.addReaction(r37);


        // ---------------------

        SquirmCell *e_end,*f_end;

        // pick a sensible place to start making a cell
        int SX=(int)(this->GetSize().x/2-9*RADIUS);
        int SY=(int)(this->GetSize().y/2-9*RADIUS+50);

        //bbacaabaccbbbab, bbccccbaaacbacb
        (e_end=CreateNewCell(SX+1*RADIUS*2.0F,SY,'e',1))->BondTo(
        CreateNewCell(SX+2*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+3*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+4*RADIUS*2.0F,SY,'a',1)->BondTo(
        CreateNewCell(SX+5*RADIUS*2.0F,SY,'c',1)->BondTo(
        /*CreateNewCell(SX+6*RADIUS*2.0F,SY,'a',1)->BondTo(
        CreateNewCell(SX+7*RADIUS*2.0F,SY,'a',1)->BondTo(
        CreateNewCell(SX+8*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+9*RADIUS*2.0F,SY,'a',1)->BondTo(
        CreateNewCell(SX+10*RADIUS*2.0F,SY,'c',1)->BondTo(
        CreateNewCell(SX+11*RADIUS*2.0F,SY,'c',1)->BondTo(
        CreateNewCell(SX+12*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+13*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+14*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+15*RADIUS*2.0F,SY,'a',1)->BondTo(*/
        CreateNewCell(SX+6*RADIUS*2.0F,SY,'b',1)->BondTo(
        CreateNewCell(SX+7*RADIUS*2.0F,SY,'d',1)->BondTo(
        (f_end=CreateNewCell(SX+8*RADIUS*2.0F,SY,'f',1)) ))))))/*))))))))))*/);

        // ------------------------------------------------------------

        int length=2; // was 25
        int width=8; // was 25
        float start_x=SX,start_y=SY-RADIUS*2;
        SquirmCell *start =  CreateNewCell(start_x,start_y,'a',T);
        start->BondTo(e_end);
        SquirmCell *a=start,*b;
        int i;
        // a string down the left
        for(i=0;i<length;i++)
        {
            b = CreateNewCell(start_x,start_y+(i+1)*RADIUS*2.0F,'a',S);
            b->BondTo(a);
            a=b;
        }
        // along the bottom
        for(i=0;i<width;i++)
        {
            b = CreateNewCell(start_x+(i+1)*RADIUS*2.0F,start_y+length*RADIUS*2.0F,'a',S);
            if(i==width-1)
            {
                b->SetState(T);
                b->BondTo(f_end);
            }
            b->BondTo(a);
            a=b;
        }
        // up the right
        for(i=length-1;i>=0;i--)
        {
            b = CreateNewCell(start_x+(width+1)*RADIUS*2.0F,start_y+(i+1)*RADIUS*2.0F,'a',S);
            b->BondTo(a);
            a=b;
        }
        // along the top
        for(i=width;i>=0;i--)
        {
            b = CreateNewCell(start_x+(i+1)*RADIUS*2.0F,start_y,'a',S);
            b->BondTo(a);
            a=b;
        }
        // link to the start of the loop
        a->BondTo(start);

    }

    // create some random cells around the place
    for(int i=0;i<n;i++)
        CreateRandomCell();

    this->initial_population = n;
}

float sqr(float x) { return x*x; }

void SquirmGrid::DoTimeStep()
{
    RecomputeVelocitiesAndReact();
    MoveCells();
    if( this->do_flood && (this->iterations%this->FLOOD_PERIOD)==0 && this->iterations>0 )
        DoFlood();
    this->iterations++;
}

SquirmCell* SquirmGrid::CreateRandomCell()
{
    // pick a random position
    C2dVector loc;
    loc.x = RandomX();
    loc.y = RandomY();
    return CreateNewCell(loc,"aaaaabcdef"[RandomN(10)],0);//RandomN(4)!=0?'c':(RandomN(2)==0?'a':'b'),0);
}

SquirmCell* SquirmGrid::CreateRandomCellOfType(char type)
{
    // pick a random  position
    C2dVector loc;
    loc.x = RandomX();
    loc.y = RandomY();
    return CreateNewCell(loc.x,loc.y,type,0);
}

SquirmCell* SquirmGrid::CreateNewCell(C2dVector loc,char type,int state)
{
    // only proceed if this location is empty
    if(Valid(loc))
    {
        SquirmCell *cell = new SquirmCell(loc,type,state);

        this->PutCell(cell);
        this->cell_list.push_back(cell);

        return cell;
    }
    else
    {
        // this position is taken!
        SquirmError("CreateNewCell : slot taken!");
        return NULL;
    }
}

void SquirmGrid::PutCell(SquirmCell *cell)
{
    // put the cell into a slot
    int cx = (int)floor(cell->location.x/slot_size);
    int cy = (int)floor(cell->location.y/slot_size);
    cx = max(0,cx);
    cx = min(cx,this->slots_X);
    cy = max(0,cy);
    cy = min(cy,this->slots_Y);
    cell_grid[cx][cy].addOccupant(cell);
}

void SquirmGrid::RemoveCell(SquirmCell *cell)
{
    // clear the slot it is in
    int cx = (int)floor(cell->location.x/slot_size);
    int cy = (int)floor(cell->location.y/slot_size);
    cx = max(0,cx);
    cx = min(cx,this->slots_X);
    cy = max(0,cy);
    cy = min(cy,this->slots_Y);
    cell_grid[cx][cy].removeOccupant(cell);
}

void SquirmGrid::DoFlood()
{
    // for all the atoms in one part of the area:
    // - remove all their bonds
    // - set their state to 0
    float left[4]={0,this->GetSize().x/2.0F,this->GetSize().x/2.0F,0};
    float top[4]={0,0,this->GetSize().y/2.0F,this->GetSize().y/2.0F};

    for(vector<SquirmCell*>::const_iterator it = this->cell_list.begin();it!=this->cell_list.end();it++)
    {
        SquirmCell *cell = *it;
        if( cell->location.x>=left[this->flood_sector] && 
            cell->location.x<=left[this->flood_sector]+this->GetSize().x/2.0F &&
            cell->location.y>=top[this->flood_sector] && 
            cell->location.y<=top[this->flood_sector]+this->GetSize().y/2.0F )
        {
            // break all of its bonds
            cell->BreakAllBonds();
            // set its state to 0
            cell->SetState(0);
            // assign a random velocity
            cell->AssignRandomVelocity();
        }
    }

    // do the flood on the other side next time
    this->flood_sector = (this->flood_sector+1)%4;
}


float SquirmGrid::RandomX()
{
    return rand()*size.x/RAND_MAX;
}

float SquirmGrid::RandomY()
{
    return rand()*size.y/RAND_MAX;
}

int SquirmGrid::RandomN(int n)
{
    if(n==0)
        SquirmError("Zero passed to RandomN");
    return (rand()*100+rand()) % n;
}

bool SquirmGrid::hasCollisions(SquirmCell* tester,C2dVector test_pos,float r)
{
    // which slot contains the test_pos?
    int cx = (int)floor(test_pos.x/slot_size);
    int cy = (int)floor(test_pos.y/slot_size);
    float r2 = r*r;
    if(cx>=0 && cy>=0 && cx<this->slots_X && cy<this->slots_Y)
    {
        int search_slots = (int)ceil(r / slot_size);
        int x,y;
        for(x=max(0,cx-search_slots);x<=min(this->slots_X-1,cx+search_slots);x++)
        {
            for(y=max(0,cy-search_slots);y<=min(this->slots_Y-1,cy+search_slots);y++)
            {
                SquirmCellSlot *slot = &cell_grid[x][y];
                if(!slot->queryEmpty())
                {
                    // now do a more expensive test
                    for(vector<SquirmCell*>::const_iterator it=slot->occupants.begin();it!=slot->occupants.end();it++)
                    {
                        SquirmCell *cell = *it;
                        if(cell!=tester && cell->GetState()!=0 && 
                            C2dVector::Dist2(cell->location,test_pos)<r2)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

void SquirmGrid::GetAllWithinRadius(C2dVector loc,float r,
                                    vector<SquirmCell*> &dest)
{
    dest.clear();
    // which slot is at the centre?
    int cx = (int)floor(loc.x/slot_size);
    int cy = (int)floor(loc.y/slot_size);
    float r2 = r*r;
    if(cx>=0 && cy>=0 && cx<this->slots_X && cy<this->slots_Y)
    {
        int search_slots = (int)ceil(r / slot_size);
        int x,y;
        for(x=max(0,cx-search_slots);x<=min(this->slots_X-1,cx+search_slots);x++)
        {
            for(y=max(0,cy-search_slots);y<=min(this->slots_Y-1,cy+search_slots);y++)
            {
                SquirmCellSlot *slot = &cell_grid[x][y];
                if(!slot->queryEmpty())
                {
                    // now do a more expensive test
                    for(vector<SquirmCell*>::const_iterator it=slot->occupants.begin();it!=slot->occupants.end();it++)
                    {
                        SquirmCell *cell = *it;
                        if(C2dVector::Dist2(cell->location,loc)<r2)
                            dest.push_back(cell);
                    }
                }
            }
        }
    }
}

void SquirmGrid::InitSearchArea(int cx,int cy,int search_slots,
                                vector<SquirmCell*> &central_cells,
                                vector<SquirmCell*> &nearby_cells)
{
    // assuming the cx,cy is valid
    central_cells = this->cell_grid[cx][cy].occupants;
    nearby_cells.clear();
    int i,j;
    for(i=max(0,cx-search_slots);i<=min(this->slots_X-1,cx+search_slots);i++)
    {
        for(j=max(0,cy-search_slots);j<=min(this->slots_Y-1,cy+search_slots);j++)
        {
            for(vector<SquirmCell*>::const_iterator it = this->cell_grid[i][j].occupants.begin();it!=this->cell_grid[i][j].occupants.end();it++)
                nearby_cells.push_back(*it);
        }
    }
}

void SquirmGrid::RecomputeVelocitiesAndReact()
{
    this->total_speed = 0.0F; // will recompute in here

    const float PHYS_RANGE = RADIUS*2.0F; // how far out do the physical forces extend?
    const float PHYS_RANGE2 = PHYS_RANGE*PHYS_RANGE;

    const float REACTION_RANGE = RADIUS*2.5F; // how far out can reactions extend?
    const float REACTION_RANGE2 = REACTION_RANGE*REACTION_RANGE;

    const float MAX_RANGE = max(PHYS_RANGE2,REACTION_RANGE2); // no effect acts beyond this distance

    // find the search radius distance in terms of the number of slots
    int search_slots = (int) ceil( MAX_RANGE / this->slot_size);

    // for each slot in the grid, get the list of cells within the neighbourhood
    // use this list to recompute the velocities of the cells in the central slot

    int cx,cy; // central square 
    vector<SquirmCell*> central_cells; // the cells in the central square
    vector<SquirmCell*> nearby_cells; // the cells in the search area (including central_cells)

    vector<SquirmCell*> reaction_candidates;

    C2dVector force,to;
    C2dVector test_location;
    float to_len2;
    float wall_range;

    const float XY_DIST = 2.0F*(float)sqrt(MAX_RANGE*MAX_RANGE/2.0F);

    for(cx=0;cx<this->slots_X;cx++)
    {
        for(cy=0;cy<this->slots_Y;cy++)
        {
            InitSearchArea(cx,cy,search_slots,central_cells,nearby_cells);
            // central_cells are the cells in the current slot (the ones being affected by the forces)
            // nearby_cells include these but also include other nearby (these together effect the forces)
            for(vector<SquirmCell*>::const_iterator it = central_cells.begin();it!=central_cells.end();it++)
            {
                SquirmCell *cell = *it;
                reaction_candidates.clear();

                // -- sum the forces acting on this cell --

                force.x = 0.0F;
                force.y = 0.0F;

                // a special case: unbonded atoms don't have any physical forces on them
                if(cell->GetBondedCells().empty())
                {
                    // no forces
                }
                else 
                {
                    // the repulsive forces between nearby cells
                    for(vector<SquirmCell*>::const_iterator it2 = nearby_cells.begin();it2!=nearby_cells.end();it2++)
                    {
                        SquirmCell *other = *it2;
                        if(other==cell) continue; // no interaction with self!

                        to.x = cell->location.x;
                        to.y = cell->location.y;
                        to.Subtract(other->location);

                        // for speed (minor saving), we first check if this cell is within 
                        // an octagon before going for the full circle test
                        if(fabs(to.x)+fabs(to.y) > XY_DIST) continue;

                        to_len2 = Length2(to);

                        // should we add this other cell to the list of possible reactants?
                        if(to_len2<REACTION_RANGE2)
                        {
                            // we don't react if the atom is too close. this was introduced
                            // when we turned off the volume exclusion force for atoms in 
                            // state 0

                            // check whether anything is too near to this atom (slow)
                            vector<SquirmCell*> too_close;
                            this->GetAllWithinRadius(other->location,RADIUS*1.5F,too_close);
                            if(too_close.size()<=1)
                                reaction_candidates.push_back(other);
                        }

                        if((cell->GetBondedCells().empty() && cell->GetState()==0) ||
                            (other->GetBondedCells().empty() && other->GetState()==0))
                        {
                            // no forces
                        }
                        else {
                            // atoms that are too close get repelled
                            to.Mult(compute_force(to_len2,PHYS_RANGE,0.4F));
                            force.Add(to);
                        }
                    }
                }

                // pull forces from bonded cells
                {
                    for(vector<SquirmCell*>::const_iterator it2 = cell->GetBondedCells().begin();it2!=cell->GetBondedCells().end();it2++)
                    {
                        SquirmCell *other = *it2;
                        if(other!=cell)
                        {
                            to.x = other->location.x;
                            to.y = other->location.y;
                            to.Subtract(cell->location);
                            to.Mult(spring_force(Length2(to)));
                            force.Add(to);
                        }
                        else
                            SquirmError("Cell bonded with self!?");
                    }
                }

                // forces from walls
                wall_range = 1.0F*RADIUS;
                if(cell->location.x<wall_range) // left wall
                {
                    to.x = cell->location.x;
                    to.y = 0.0F;
                    // length of to vector is easy to compute (sign must be positive)
                    to.Mult(wall_range/cell->location.x-1.0F);
                    force.Add(to);
                }
                if(cell->location.x>size.x-wall_range) // right wall
                {
                    to.x = cell->location.x-size.x;
                    to.y = 0.0F;
                    // length of to vector is easy to compute (sign must be positive)
                    to.Mult(wall_range/(size.x-cell->location.x)-1.0F);
                    force.Add(to);
                }
                if(cell->location.y<wall_range) // top wall
                {
                    to.x = 0.0F;
                    to.y = cell->location.y;
                    // length of to vector is easy to compute (sign must be positive)
                    to.Mult(wall_range/cell->location.y-1.0F);
                    force.Add(to);
                }
                if(cell->location.y>size.y-wall_range) // bottom wall
                {
                    to.x = 0.0F;
                    to.y = cell->location.y-size.y;
                    // length of to vector is easy to compute (sign must be positive)
                    to.Mult(wall_range/(size.y-cell->location.y)-1.0F);
                    force.Add(to);
                }

                cell->velocity.Add(force);

                // we must limit the velocity else atoms can pass through each other
                {
                    float speed = Length(cell->velocity);
                    if(speed>MAX_VELOCITY)
                    {
                        this->lost_speed += speed-MAX_VELOCITY;
                        cell->velocity.Mult(MAX_VELOCITY/speed);
                    }
                    // we must also not allow the overall momentum in the system to drop, else
                    // everything will grind to a halt
                    // (numerical hack that makes life easy)
                    else if(speed<MAX_VELOCITY-1.0F && this->lost_speed>1.0F && speed!=0.0F)
                    {
                        // add one pixel per iteration of speed to this atom
                        cell->velocity.Mult((speed+1.0F)/speed);
                        this->lost_speed -= 1.0F;
                        // um, this isn't working at the moment
                    }
                }

                // optional monitoring:
                this->total_speed += Length(cell->velocity);

                // apply any reactions that are possible
                reaction_candidates.push_back(cell);
                int ret = this->chemistry.react(cell,reaction_candidates,REACTION_RANGE2);
           }
        }
    }
}

void SquirmGrid::MoveCells()
{
    // usual code that ties in with RecomputeVelocitiesAndReact

    // then, update the position of each cell using its velocity
    C2dVector new_loc;

    for(vector<SquirmCell*>::const_iterator it=this->cell_list.begin();it!=this->cell_list.end();it++)
    {
        SquirmCell *cell = *it;

        int old_slot_x = (int)floor(cell->location.x/slot_size);
        int old_slot_y = (int)floor(cell->location.y/slot_size);

        new_loc = cell->location + cell->velocity;
        if(Valid(new_loc))
            cell->location = new_loc;
        else
        {
            if(cell->location.x<0.0F)
                cell->location.x = 0.0F;
            else if(cell->location.x>this->size.x)
                cell->location.x = this->size.x;
            if(cell->location.y<0.0F)
                cell->location.y = 0.0F;
            else if(cell->location.y>this->size.y)
                cell->location.y = this->size.y;
            cell->velocity = C2dVector(0,0);
        }

        int new_slot_x = (int)floor(cell->location.x/slot_size);
        int new_slot_y = (int)floor(cell->location.y/slot_size);

        if(old_slot_x!=new_slot_x || old_slot_y!=new_slot_y)
        {
            this->cell_grid[old_slot_x][old_slot_y].removeOccupant(cell);
            this->cell_grid[new_slot_x][new_slot_y].addOccupant(cell);
        }
    }
}

string SquirmGrid::ProbeAt(float x,float y,float scale)
{ 
    vector<SquirmCell*> dest;
    this->GetAllWithinRadius(C2dVector((x-OFFSET)/scale,(y-OFFSET)/scale),RADIUS,dest);
    string text;
    if(!dest.empty())
    {
        SquirmCell *cell = dest.front();
        ostringstream oss;
        oss << cell->GetType() << cell->GetState();
        return oss.str();
    }
    else
        return "-";
}

float SquirmGrid::compute_force(float r2,float range,float magnitude)
{
    if(r2 < range*range)
        return magnitude * (range/(float)sqrt(r2) - 1.0F); // our force/distance curve
    else
        return 0.0F;
}

float SquirmGrid::spring_force(float r2)
{
    static const float RANGE = 2.0F*RADIUS;
    static const float RANGE2 = RANGE*RANGE;
    if(r2 > RANGE2) 
        return 0.4F * ( (float)sqrt(r2)/RANGE - 1.0F );
    else
        return 0.0F;
}

