#ifndef TABTWO_H
#define TABTWO_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/richtext/richtextctrl.h>
#include <vector>
#include <string>
#include <wx/scrolwin.h>

// Estrutura para armazenar os controles de cada entrada
struct TabTwoEntryControls {
    wxRichTextCtrl* richTextCtrl;
};

class TabTwo : public wxScrolledWindow {
public:
    TabTwo(wxNotebook* parent);

private:
    void OnSaveButtonClicked(wxCommandEvent& event);
    std::string EscapeXML(const std::string& text);
    std::string GetFormattedText(wxRichTextCtrl* ctrl, const wxColour& defaultColor);
    std::string ColourToHex(const wxColour& color);

    std::vector<TabTwoEntryControls> entries;
    std::vector<std::string> labels;

    wxColour backgroundColor;
    wxColour textColor;
    wxColour buttonColor;
    wxColour textCtrlBg;
    wxColour textCtrlFg;
};

#endif // TABTWO_H
