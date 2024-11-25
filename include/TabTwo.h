// include/TabTwo.h
#ifndef TABTWO_H
#define TABTWO_H
#include <wx/notebook.h>
#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <string>

class TabTwo : public wxPanel {
public:
    TabTwo(wxNotebook* parent);

private:
    wxTextCtrl* imagePathCtrl;
    wxTextCtrl* widthCtrl;
    std::string imagePath;

    void OnSelectImageButtonClicked(wxCommandEvent& event);
    void OnProcessButtonClicked(wxCommandEvent& event);
};

#endif // TABTWO_H
