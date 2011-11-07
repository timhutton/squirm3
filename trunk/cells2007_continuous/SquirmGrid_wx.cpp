// local:
#include "SquirmGrid_wx.h"

// STL:
#include <vector>
#include <sstream>
using namespace std;

wxColor GetColor(char type)
{
    switch(type)
    {
        default:
        case 'e': return wxColor(255,0,0); // red
        case 'f': return wxColor(0,255,0); // green
        case 'a': return wxColor(220,220,0); // yellow
        case 'b': return wxColor(128,128,128); // grey
        case 'c': return wxColor(0,255,255); // cyan
        case 'd': return wxColor(0,0,255); // blue
    }
}

void SquirmGrid_wx::Draw(wxGraphicsContext* pGC,float scale)
{
    // draw the background rectangle
    pGC->SetPen(*wxBLACK_PEN);
    pGC->SetBrush(*wxWHITE_BRUSH);
    pGC->DrawRectangle(OFFSET,OFFSET,int(this->size.x*scale)-1,int(this->size.y*scale)-1);

    // draw the atoms
    for(int pass=0;pass<2;pass++) // we want unbonded atoms to appear faint and behind (as if 2.5D)
    {
        for(vector<SquirmCell*>::const_iterator it = this->cell_list.begin();it!=this->cell_list.end();it++)
        {
            SquirmCell *cell = *it;
            // draw the cell
            wxColor color = GetColor(cell->GetType());
            if(cell->GetState()==0)
            {
                // background atom
                // skip these in the second pass
                if(pass==1) continue;
                // make mostly transparent
                color = wxColor(color.Red(),color.Green(),color.Blue(),10);
            }
            else 
            {
                // foreground atom
                if(pass==0) continue;
                // make slightly transparent
                color = wxColor(color.Red(),color.Green(),color.Blue(),200);
            }
            pGC->SetBrush(wxBrush(color));
            pGC->SetPen(wxPen(color));
            float s = scale*RADIUS*0.8;
            pGC->DrawEllipse(cell->location.x*scale-s+OFFSET,cell->location.y*scale-s+OFFSET,s*2,s*2);
            if(false && cell->GetState()>0)
            {
                // draw the cell's state
                ostringstream oss;
                oss << cell->GetState();
                wxString t(oss.str().c_str(),wxConvUTF8);
                double w,h;
                pGC->SetFont(*wxNORMAL_FONT,*wxBLACK);
                pGC->GetTextExtent(t,&w,&h); // will use rect size to center
                pGC->DrawText(t,(int)(cell->location.x*scale)-w/2+OFFSET,
                    (int)(cell->location.y*scale)-h/2+OFFSET);
            }
        }
    }

    // draw the bonds
    pGC->SetPen(wxPen(wxColor(0,0,0,50),3));
    for(vector<SquirmCell*>::const_iterator it = this->cell_list.begin();it!=this->cell_list.end();it++)
    {
        SquirmCell *cell = *it;
        // draw the cell's bonds as lines
        for(vector<SquirmCell*>::const_iterator it2=cell->GetBondedCells().begin();it2!=cell->GetBondedCells().end();it2++)
        {
            SquirmCell *other = *it2;
            if(other<cell) continue; // (avoid drawing the same line more than once)

            pGC->StrokeLine(int(cell->location.x*scale)+OFFSET,
                int(cell->location.y*scale)+OFFSET,
                int(other->location.x*scale)+OFFSET,
                int(other->location.y*scale)+OFFSET);
        }
    }

}

