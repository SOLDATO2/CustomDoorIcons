// TabTwo.cpp
#include "TabTwo.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <iostream>

// Construtor da classe TabTwo
TabTwo::TabTwo(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    SetScrollRate(5, 5);

    // Inicializar labels
    labels = {
        "SCP-173",
        "Class-D Personnel",
        "Spectator",
        "SCP-106",
        "Nine-Tailed Fox Specialist",
        "SCP-049",
        "Scientist",
        "SCP-079",
        "Chaos Insurgency Conscript",
        "SCP-096",
        "SCP-049-2",
        "Nine-Tailed Fox Sergeant",
        "Nine-Tailed Fox Captain",
        "Nine-Tailed Fox Private",
        "Tutorial",
        "Facility Guard",
        "SCP-939",
        "-",
        "Chaos Insurgency Rifleman",
        "Chaos Insurgency Marauder",
        "Chaos Insurgency Repressor",
        "Overwatch",
        "Filmmaker",
        "SCP-3114",
        "SCP-1507",
        "SCP-1507-Alpha",
        "SCP-1507-049"
    };

    // Criar um sizer vertical para organizar as entradas
    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    for (const auto& labelText : labels) {
        if(labelText == "-") {
            continue;
        }
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        // Label acima do campo de texto
        wxStaticText* label = new wxStaticText(this, wxID_ANY, labelText);
        entrySizer->Add(label, 0, wxALIGN_LEFT | wxALL, 5);

        // Campo de texto
        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "");
        entrySizer->Add(textField, 0, wxEXPAND | wxALL, 5);

        // Armazenar o controle
        TabTwoEntryControls controls = { textField };
        entries.push_back(controls);

        // Adicionar a entrada ao sizer principal
        entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 2);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    // Botão "Salvar"
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);

    // Bind do evento do botão "Salvar"
    saveButton->Bind(wxEVT_BUTTON, &TabTwo::OnSaveButtonClicked, this);
}

// Evento para o botão "Salvar"
void TabTwo::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for (size_t i = 0; i < entries.size(); ++i) {
        wxString text = entries[i].textField->GetValue();

        // Se o campo estiver vazio, usamos o rótulo padrão
        if (text.IsEmpty()) {
            text = wxString::FromUTF8(labels[i].c_str());
        }

        output += text + "\n";
    }

    // Diálogo para salvar o arquivo
    wxFileDialog saveFileDialog(
        this,
        _("Save Classes_Names.txt file"),
        "",
        "Classes_Names.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return; // O usuário cancelou a operação
    }

    wxString filePath = saveFileDialog.GetPath();

    // Salvar o output no arquivo
    wxFile file;
    if (file.Open(filePath, wxFile::write)) {
        file.Write(output);
        file.Close();
        wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
    }
}
