// TabFour.h
#ifndef TABFOUR_H
#define TABFOUR_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/scrolwin.h>
#include <wx/richtext/richtextctrl.h>
#include <vector>
#include <string>

// Estrutura representando um item
struct Item {
    int id;
    std::string name;
    std::string description;
};

// Estrutura para armazenar os controles de cada entrada de item
struct TabFourEntryControls {
    wxRichTextCtrl* nameField;
    wxButton* nameColorButton;
    wxRichTextCtrl* descriptionField; // Pode ser nullptr se não houver descrição
    wxButton* descriptionColorButton; // Pode ser nullptr se não houver descrição
};

// Subclasse de wxRichTextCtrl para interceptar eventos (similar ao MyRichTextCtrl em TabTwo)
class MyRichTextCtrlTabFour : public wxRichTextCtrl {
public:
    MyRichTextCtrlTabFour(wxWindow* parent, wxWindowID id, const wxString& value,
                          const wxPoint& pos = wxDefaultPosition,
                          const wxSize& size = wxDefaultSize,
                          long style = wxRE_MULTILINE | wxBORDER_SUNKEN)
        : wxRichTextCtrl(parent, id, value, pos, size, style) {
        Bind(wxEVT_MOUSEWHEEL, &MyRichTextCtrlTabFour::OnMouseWheel, this);
        Bind(wxEVT_CHAR, &MyRichTextCtrlTabFour::OnChar, this);
    }

private:
    void OnMouseWheel(wxMouseEvent& event) {
        wxScrolledWindow* scrolledParent = dynamic_cast<wxScrolledWindow*>(GetParent());
        if (scrolledParent) {
            scrolledParent->ProcessWindowEvent(event);
        }
        // Não chama event.Skip() para impedir que o controle de texto processe o evento
    }

    void OnChar(wxKeyEvent& event) {
        if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
            // Ignora a tecla Enter para impedir quebras de linha
            return;
        }
        event.Skip(); // Permite que outras teclas sejam processadas normalmente
    }
};

class TabFour : public wxScrolledWindow {
public:
    TabFour(wxNotebook* parent);

private:
    void OnSaveButtonClicked(wxCommandEvent& event);
    std::string EscapeXML(const std::string& text);
    std::string GetFormattedText(wxRichTextCtrl* ctrl, const wxColour& defaultColor);
    std::string ColourToHex(const wxColour& color);

    std::vector<TabFourEntryControls> entries;
    std::vector<Item> items;

    wxColour backgroundColor;
    wxColour textColor;
    wxColour buttonColor;
    wxColour textCtrlBg;
    wxColour textCtrlFg;
};

#endif // TABFOUR_H
