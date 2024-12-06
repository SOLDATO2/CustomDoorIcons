// TabFour.h
#ifndef TABFOUR_H
#define TABFOUR_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <string>
#include <wx/scrolwin.h>

struct Item {
    int id;
    std::string name;
    std::string description;
};

struct TabFourEntryControls {
    wxTextCtrl* nameField;
    wxTextCtrl* descriptionField;
};

class TabFour : public wxScrolledWindow {
public:
    TabFour(wxNotebook* parent);
private:
    void OnSaveButtonClicked(wxCommandEvent& event);
    std::vector<Item> items;
    std::vector<TabFourEntryControls> entries;
};

#endif // TABFOUR_H
