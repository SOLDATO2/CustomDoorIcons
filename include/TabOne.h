// include/TabOne.h
#ifndef TABONE_H
#define TABONE_H

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <vector>
#include <string>
#include <wx/notebook.h>


struct EntryControls {
    wxTextCtrl* textField;
    wxTextCtrl* sizeField;
    wxColourPickerCtrl* colorPicker;
    wxCheckBox* isTextCheckBox;
};

class TabOne : public wxPanel {
public:
    TabOne(wxNotebook* parent);

private:
    std::vector<EntryControls> entries;
    std::vector<std::string> labels;
    std::vector<wxColour> defaultColors;

    void OnSaveButtonClicked(wxCommandEvent& event);
};

#endif // TABONE_H
