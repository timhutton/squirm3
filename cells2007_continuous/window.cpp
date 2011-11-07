// wxWidgets:
#include <wx/wx.h>
#include <wx/dcbuffer.h>

// local:
#include "SquirmGrid_wx.h"

// STL:
#include <sstream>
using namespace std;

class MyApp: public wxApp
{
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:

    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnPaint(wxPaintEvent& event);
    void OnIdle(wxIdleEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    void OnRun(wxCommandEvent& event);
    void OnUpdateRun(wxUpdateUIEvent& event);

    void OnStop(wxCommandEvent& event);
    void OnUpdateStop(wxUpdateUIEvent& event);

    void OnReallyFast(wxCommandEvent& event);
    void OnUpdateReallyFast(wxUpdateUIEvent& event);

    DECLARE_EVENT_TABLE()

protected:
    
    SquirmGrid_wx grid;
    
    bool running;
    bool really_fast;
    float its_per_second; // how fast are we running?

    clock_t old_time,new_time;

    float scale;

    bool save_video;
};

namespace ID {
    enum
    {
        Quit = 1,
        About,
        Run,
        Stop,
        ReallyFast,
    };
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)

    EVT_PAINT(MyFrame::OnPaint)
    EVT_IDLE(MyFrame::OnIdle)
    EVT_SIZE(MyFrame::OnSize)
    EVT_MOTION(MyFrame::OnMouseMove)

    EVT_MENU(ID::Quit, MyFrame::OnQuit)
    EVT_MENU(ID::About, MyFrame::OnAbout)

    EVT_MENU(ID::Run, MyFrame::OnRun)
    EVT_UPDATE_UI(ID::Run, MyFrame::OnUpdateRun)
    EVT_MENU(ID::Stop, MyFrame::OnStop)
    EVT_UPDATE_UI(ID::Stop, MyFrame::OnUpdateStop)
    EVT_MENU(ID::ReallyFast, MyFrame::OnReallyFast)
    EVT_UPDATE_UI(ID::ReallyFast, MyFrame::OnUpdateReallyFast)

END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( _("cells2007"), wxDefaultPosition,
                                  wxSize(600,500) );
    frame->Show(true);
    SetTopWindow(frame);
    return true;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, -1, title, pos, size )
{
    wxMenuBar *menuBar = new wxMenuBar;
    {
        wxMenu *file_menu = new wxMenu;
        file_menu->Append( ID::Quit, _("E&xit") );
        menuBar->Append( file_menu, _("&File") );
    }
    {
        wxMenu *actions_menu = new wxMenu;
        actions_menu->Append( ID::Run, _("&Run\tF5") );
        actions_menu->AppendCheckItem( ID::ReallyFast, _("Really &fast\tF7") );
        actions_menu->Append( ID::Stop, _("&Stop\tF6") );
        menuBar->Append( actions_menu, _("&Actions") );
    }
    {
        wxMenu *help_menu = new wxMenu;
        help_menu->Append( ID::About, _("&About...") );
        menuBar->Append( help_menu, _("&Help") );
    }
    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _("Ready") );
    
    // initialise our world
    running = false;
    really_fast = false;
    its_per_second = 0.0F;
    save_video = false;
    const float SIDE_X = 300.0F;
    const float SIDE_Y = 200.0F;
    this->grid.Create(SIDE_X,SIDE_Y);
    const float DENSITY = 200.0F/(200.0F*200.0F);
    this->grid.InitSimple((int)(SIDE_X*SIDE_Y*DENSITY));
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("An implementation of the continuous-space model from:\n\nHutton T.J. (2007) \"Evolvable Self-Reproducing Cells in a Two-Dimensional Artificial Chemistry\" Artificial Life 13(1): 11-30.\n\nSee: http://www.sq3.org.uk"),
                  _("About cells2007"),
                  wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnIdle(wxIdleEvent& event)
{
    // we drive our game loop by onIdle events
    if(!this->running) return;

    this->grid.DoTimeStep();

    // report the number of iterations so far
    const int REPORT_ITERATIONS_EVERY=1000;
    if(!this->really_fast || this->grid.GetIterations()%REPORT_ITERATIONS_EVERY==0)
    {
        ostringstream oss;
        oss << "Iterations: " << this->grid.GetIterations() << " (" << its_per_second 
            << " its/s)";
        //oss << "    [Total speed: " << this->grid.GetTotalSpeed() << "]";
        this->SetStatusText(wxString(oss.str().c_str(),wxConvUTF8));
        this->Refresh(false);
    }

    const int N_ITS=10;
    if(this->grid.GetIterations()%N_ITS==0)
    {
        this->old_time = this->new_time;
        this->new_time = clock();
        float elapsed = (float)(this->new_time-this->old_time) / CLOCKS_PER_SEC;
        this->its_per_second = N_ITS/elapsed;
    }

    if(!this->really_fast)
        wxMilliSleep(30);

    event.RequestMore(); // trigger another onIdle event
}

void MyFrame::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc( this );        
    wxGraphicsContext* gdc = wxGraphicsContext::Create( dc );
    if(!gdc) SquirmError("Failed to create wxGraphicsContext");

    bool save_this_frame = (this->save_video && this->grid.GetIterations()%2==0);

    wxBitmap bmp;
    if(save_this_frame)
    {
        int sx = this->grid.GetSize().x*scale;
        int sy = this->grid.GetSize().y*scale;
        // round up to multiple of 4 to comply with video encoding
        while(sx%4) sx++;
        while(sy%4) sy++;
        bmp.Create(sx,sy);
        dc.SelectObject(bmp);
    }

    this->grid.Draw(gdc,this->scale);

    if(save_this_frame)
    {
        ostringstream oss;
        oss << "frame_" << this->grid.GetIterations()/2 << ".bmp";
        bmp.SaveFile(wxString(oss.str().c_str(),wxConvUTF8),wxBITMAP_TYPE_BMP);
    }

    delete gdc;
}

void MyFrame::OnRun(wxCommandEvent& event) 
{
    this->running = true;
    this->Refresh();
}

void MyFrame::OnUpdateRun(wxUpdateUIEvent &event) 
{
    event.Enable(!this->running);
}

void MyFrame::OnStop(wxCommandEvent& event) 
{
    this->running = false;
}

void MyFrame::OnUpdateStop(wxUpdateUIEvent &event) 
{
    event.Enable(this->running);
}

void MyFrame::OnReallyFast(wxCommandEvent& event) 
{
    this->really_fast = !this->really_fast;
}

void MyFrame::OnUpdateReallyFast(wxUpdateUIEvent &event) 
{
    event.Enable(this->running);
    event.Check(this->really_fast);
}

void MyFrame::OnMouseMove(wxMouseEvent& event) 
{
    // if stopped and mouse-moving over the window then report the status of the cell being pointed at
    if(!running)
        this->SetStatusText(wxString(this->grid.ProbeAt(event.GetX(),event.GetY(),scale).c_str(),wxConvUTF8));
    
    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    float scale_x = 0.98F*this->GetClientSize().GetWidth()/this->grid.GetSize().x;
    float scale_y = 0.98F*this->GetClientSize().GetHeight()/this->grid.GetSize().y;
    this->scale = min(scale_x,scale_y);

    this->Refresh(true);
}
