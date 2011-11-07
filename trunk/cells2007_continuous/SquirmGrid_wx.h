// local:
#include "SquirmGrid.h"

// wxWidgets:
#include <wx/graphics.h>

class SquirmGrid_wx : public SquirmGrid
{
public:

    void Draw(wxGraphicsContext* pDC,float scale);

};
