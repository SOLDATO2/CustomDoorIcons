// TabTwo.h
#ifndef TAB_TWO_H
#define TAB_TWO_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <string>
#include <wx/scrolwin.h>

// Estrutura para armazenar os controles de cada entrada
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

#endif // TAB_TWO_H
