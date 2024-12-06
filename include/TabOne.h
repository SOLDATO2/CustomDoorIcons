// TabOne.h
#ifndef TABONE_H
#define TABONE_H

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <vector>
#include <string>
#include <wx/notebook.h>

struct TabOneEntryControls {
    wxTextCtrl* textField;
    wxTextCtrl* sizeField;
    wxColourPickerCtrl* colorPicker;
    wxCheckBox* ignoreSizeCheckBox;
    wxButton* selectImageButton;
    wxButton* resetImageButton;
    std::string imagePath;
};

class TabOne : public wxScrolledWindow {
public:
    TabOne(wxNotebook* parent);

private:
    void OnSelectImageButtonClicked(size_t index);
    void OnResetImageButtonClicked(size_t index);
    void OnSaveButtonClicked(wxCommandEvent& event);

    std::vector<TabOneEntryControls> entries;
    std::vector<std::string> labels;
    std::vector<wxColour> defaultColors;
};

#endif // TABONE_H
