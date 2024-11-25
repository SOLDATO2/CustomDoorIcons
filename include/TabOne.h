// include/TabOne.h
#ifndef TABONE_H
#define TABONE_H

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <vector>
#include <string>
#include <wx/notebook.h>

// Estrutura para armazenar os controles de cada entrada
struct EntryControls {
    wxTextCtrl* textField;
    wxTextCtrl* sizeField;
    wxColourPickerCtrl* colorPicker;
    wxCheckBox* isTextCheckBox;
    wxButton* selectImageButton;
    wxButton* resetImageButton;
    std::string imagePath;
};

class TabOne : public wxPanel {
public:
    TabOne(wxNotebook* parent);

private:
    std::vector<EntryControls> entries;
    std::vector<std::string> labels;
    std::vector<wxColour> defaultColors;

    void OnSaveButtonClicked(wxCommandEvent& event);
    void OnSelectImageButtonClicked(size_t index);
    void OnResetImageButtonClicked(size_t index);
};

#endif // TABONE_H
