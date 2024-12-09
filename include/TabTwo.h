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

// Subclasse de wxRichTextCtrl para interceptar eventos de scroll e teclas
class MyRichTextCtrl : public wxRichTextCtrl {
public:
    MyRichTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxRE_MULTILINE | wxBORDER_SUNKEN)
        : wxRichTextCtrl(parent, id, value, pos, size, style) {
        Bind(wxEVT_MOUSEWHEEL, &MyRichTextCtrl::OnMouseWheel, this);
        Bind(wxEVT_CHAR, &MyRichTextCtrl::OnChar, this);
    }

private:
    void OnMouseWheel(wxMouseEvent& event) {
        wxScrolledWindow* scrolledParent = dynamic_cast<wxScrolledWindow*>(GetParent());
        if (scrolledParent) {
            // Passa o evento como referência sem cloná-lo
            scrolledParent->ProcessWindowEvent(event);
        }
        // Não chama event.Skip() para impedir que o campo de texto processe o evento
    }

    void OnChar(wxKeyEvent& event) {
        if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
            // Ignora a tecla Enter para impedir quebras de linha
            return;
        }
        event.Skip(); // Permite que outras teclas sejam processadas normalmente
    }
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
