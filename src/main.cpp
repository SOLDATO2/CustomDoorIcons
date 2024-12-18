// main.cpp
#include <wx/wx.h>
#include "TabOne.h"
#include "TabTwo.h"
#include "TabThree.h"
#include "TabFour.h"
#include "icons/appicon.h"
#include <wx/notebook.h>
#include <wx/mstream.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxInitAllImageHandlers();

    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Custom Door Icons", wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
    frame->SetBackgroundColour(wxColour(30, 30, 30));

    SetTopWindow(frame);

    wxMemoryInputStream memStream(__appicon_png, __appicon_png_len);
    wxImage image(memStream, wxBITMAP_TYPE_PNG);

    if (!image.IsOk())
    {
        //couldnt load icon
    }
    else
    {
        wxIcon icon;
        icon.CopyFromBitmap(wxBitmap(image));
        frame->SetIcon(icon);
    }

    wxNotebook* notebook = new wxNotebook(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    notebook->SetBackgroundColour(wxColour(30, 30, 30));
    notebook->SetForegroundColour(*wxWHITE);

    TabOne* tabOne = new TabOne(notebook);
    TabTwo* tabTwo = new TabTwo(notebook);
    TabThree* tabThree = new TabThree(notebook);
    TabFour* tabFour = new TabFour(notebook);

    notebook->AddPage(tabOne, "Door Settings");
    notebook->AddPage(tabTwo, "Team Names");
    notebook->AddPage(tabThree, "Intercom");
    notebook->AddPage(tabFour, "Items");

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 10);
    frame->SetSizer(mainSizer);

    frame->SetSize(800, 600);

    frame->Show(true);
    return true;
}
