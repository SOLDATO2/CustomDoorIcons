#ifndef TABTHREE_H
#define TABTHREE_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <string>

struct TabThreeEntryControls {
    wxTextCtrl* textField;
    wxCheckBox* isTextCheckBox;
};

class TabThree : public wxPanel {
public:
    TabThree(wxNotebook* parent);
private:
    void OnSaveButtonClicked(wxCommandEvent& event);
    std::vector<std::string> labels;
    std::vector<TabThreeEntryControls> entries;
};

#endif // TABTHREE_H