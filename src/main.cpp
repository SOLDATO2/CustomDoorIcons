// main.cpp
#include <wx/wx.h>
#include "TabOne.h"
#include "TabTwo.h"
#include "TabThree.h"
#include "TabFour.h"
#include <wx/notebook.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

// main.cpp
bool MyApp::OnInit() {
    wxInitAllImageHandlers();

    // create window with dark background
    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Custom Door Icons", wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
    frame->SetBackgroundColour(wxColour(30, 30, 30)); // Fundo escuro

    SetTopWindow(frame);

    //notebook
    wxNotebook* notebook = new wxNotebook(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
    notebook->SetBackgroundColour(wxColour(30, 30, 30));
    notebook->SetForegroundColour(*wxWHITE);

    // windows
    TabOne* tabOne = new TabOne(notebook);
    TabTwo* tabTwo = new TabTwo(notebook);
    TabThree* tabThree = new TabThree(notebook);
    TabFour* tabFour = new TabFour(notebook);

    // adding windows to notebook
    notebook->AddPage(tabOne, "Door Settings");
    notebook->AddPage(tabTwo, "Team Names");
    notebook->AddPage(tabThree, "Intercom");
    notebook->AddPage(tabFour, "Items");

    // sizer config
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 10);
    frame->SetSizer(mainSizer);

    // default window size
    frame->SetSize(800, 600); // 800px, 600px

    frame->Show(true);
    return true; //loop
}
