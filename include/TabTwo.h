// TabTwo.h
#ifndef TABTWO_H
#define TABTWO_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <string>
#include <wx/scrolwin.h>

struct TabTwoEntryControls {
    wxTextCtrl* textField;
};

class TabTwo : public wxScrolledWindow {
public:
    TabTwo(wxNotebook* parent);

private:
    void OnSaveButtonClicked(wxCommandEvent& event);

    std::vector<TabTwoEntryControls> entries;
    std::vector<std::string> labels;
};

#endif // TABTWO_H
