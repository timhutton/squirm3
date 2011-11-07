// SquirmCellProperties.cpp

// local:
#include "SquirmCellProperties.h"
#include "SquirmError.h"

// stdlib:
#include <stdlib.h>

SquirmCellProperties::SquirmCellProperties(char t,int s)
{
    this->type = t;
    this->state = s;
}

void SquirmCellProperties::EIGHT(int i,int &x,int &y)
{
    switch(i)
    {
    case 0: x=-1; y=-1; break;
    case 1: x=0; y=-1; break;
    case 2: x=1; y=-1; break;
    case 3: x=1; y=0; break;
    case 4: x=1; y=1; break;
    case 5: x=0; y=1; break;
    case 6: x=-1; y=1; break;
    case 7: x=-1; y=0; break;
    default: SquirmError("EIGHT: i out of range!"); break;
    }
    return;
}

void SquirmCellProperties::FOUR(int i,int &x,int &y)
{
    switch(i)
    {
    case 0: x=0; y=-1; break;
    case 1: x=1; y=0; break;
    case 2: x=0; y=1; break;
    case 3: x=-1; y=0; break;
    default: SquirmError("FOUR: i out of range!"); break;
    }
    return;
}

char SquirmCellProperties::GetRandomType()
{
    return "abcdef"[rand()%6];
}

