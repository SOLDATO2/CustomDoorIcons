// src/TabOne.cpp
#include "TabOne.h"
#include <wx/notebook.h>
#include <wx/clipbrd.h>

// Classe para a primeira aba
TabOne::TabOne(wxNotebook* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Inicializar labels
    labels = {
        "OPEN",
        "CLOSED",
        "MOVING",
        "LOCKDOWN",
        "ERROR",
        "ACCESS DENIED",
        "ACCESS GRANTED",
        "KEYCARD REQUIRED",
        "PLEASE PASS",
        "CLOSING"
    };

    // Cores padrão para cada entrada
    defaultColors = {
        wxColour(0, 255, 0),       // green
        wxColour(7, 162, 255),     // #07A2FF
        wxColour(255, 165, 0),     // orange
        wxColour(255, 0, 0),       // red
        wxColour(255, 0, 0),       // red
        wxColour(255, 0, 0),       // red
        wxColour(0, 255, 0),       // green
        wxColour(7, 162, 255),     // #07A2FF
        wxColour(255, 165, 0),     // orange
        wxColour(255, 165, 0)      // orange
    };

    // Criar um sizer vertical para organizar as entradas
    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    for (size_t i = 0; i < labels.size(); ++i) {
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        // Label acima dos campos
        wxStaticText* label = new wxStaticText(this, wxID_ANY, labels[i]);
        entrySizer->Add(label, 0, wxALIGN_LEFT | wxALL, 5);

        // Sizer horizontal para os campos
        wxBoxSizer* fieldsSizer = new wxBoxSizer(wxHORIZONTAL);

        // Campo de texto/emoji
        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "");
        fieldsSizer->Add(textField, 1, wxEXPAND | wxALL, 5);

        // Campo de tamanho (valor padrão 90)
        wxTextCtrl* sizeField = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
        fieldsSizer->Add(sizeField, 0, wxEXPAND | wxALL, 5);

        // Selecionador de cor com valor padrão
        wxColourPickerCtrl* colorPicker = new wxColourPickerCtrl(this, wxID_ANY, defaultColors[i]);
        fieldsSizer->Add(colorPicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Checkbox para indicar se é um texto
        wxCheckBox* isTextCheckBox = new wxCheckBox(this, wxID_ANY, "Text");
        fieldsSizer->Add(isTextCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Armazenar os controles
        EntryControls controls = { textField, sizeField, colorPicker, isTextCheckBox };
        entries.push_back(controls);

        // Adicionar os campos ao sizer horizontal
        entrySizer->Add(fieldsSizer, 0, wxEXPAND | wxALL, 2);

        // Adicionar a entrada ao sizer principal
        entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 2);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    // Botão "Salvar"
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);

    // Bind do evento de clique no botão "Salvar"
    saveButton->Bind(wxEVT_BUTTON, &TabOne::OnSaveButtonClicked, this);
}

void TabOne::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;
    for (size_t i = 0; i < entries.size(); ++i) {
        wxString text = entries[i].textField->GetValue();
        wxString sizeStr = entries[i].sizeField->GetValue();
        wxColour color = entries[i].colorPicker->GetColour();
        bool isText = entries[i].isTextCheckBox->GetValue();

        // Se o campo de texto estiver vazio, usar o label correspondente e tratar como texto
        if (text.IsEmpty()) {
            text = wxString::FromUTF8(labels[i]);
            isText = true; // Forçar tratamento como texto
        }

        // Formatar a saída conforme as especificações
        if (isText) {
            // Se for texto, não incluir o parâmetro <size=...>
            output += "<color=" + color.GetAsString(wxC2S_HTML_SYNTAX) + ">" + text + "</color>\n";
        }
        else {
            // Se não for texto, incluir o parâmetro <size=...>
            long sizeValue = 90; // Valor padrão
            if (!sizeStr.ToLong(&sizeValue)) {
                sizeValue = 90; // Valor padrão em caso de erro
            }
            output += "<color=" + color.GetAsString(wxC2S_HTML_SYNTAX) + "><size=" + wxString::Format("%ld", sizeValue) + ">" + text + "</size></color>\n";
        }
    }

    // Remover a última quebra de linha
    if (!output.IsEmpty() && output.Last() == '\n') {
        output.RemoveLast();
    }

    // Copiar a saída para o clipboard
    if (wxTheClipboard->Open()) {
        // Criar um objeto de dados de texto com a string Unicode
        wxTextDataObject* data = new wxTextDataObject(output);
        wxTheClipboard->SetData(data);
        wxTheClipboard->Close();
        wxMessageBox("Copied to clipboard", "Sucess", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Could not access clipboard.", "Error", wxOK | wxICON_ERROR);
    }
}
