#ifndef TABFOUR_H
#define TABFOUR_H

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <string>
#include <wx/scrolwin.h>

// Estrutura para armazenar cada item com ID, nome e descrição
struct Item {
    int id;
    std::string name;
    std::string description;
};

// Estrutura para armazenar os controles de entrada
struct TabFourEntryControls {
    wxTextCtrl* nameField;
    wxTextCtrl* descriptionField; // Pode ser nullptr se não houver descrição
};

class TabFour : public wxScrolledWindow {
public:
    TabFour(wxNotebook* parent);
private:
    void OnSaveButtonClicked(wxCommandEvent& event);
    std::vector<Item> items; // Vetor de itens
    std::vector<TabFourEntryControls> entries; // Vetor de controles de entrada
};

#endif // TABFOUR_H
