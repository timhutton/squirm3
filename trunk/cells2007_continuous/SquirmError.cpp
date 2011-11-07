// SquirmError.cpp

// local:
#include "SquirmError.h"

// wxWidgets:
#include <wx/wx.h>
#include <wx/string.h>

void SquirmError(std::string text) 
{ 
    wxMessageBox(wxString(text.c_str(),wxConvUTF8),_T("Fatal error"));
    wxExit();
}
