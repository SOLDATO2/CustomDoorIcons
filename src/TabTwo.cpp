// TabTwo.cpp
#include "TabTwo.h"
#include "CustomFontLoader.h" // Inclua o carregador de fonte personalizada
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <iostream>

const std::string OLIVERS_BARNEY_REGULAR = "OliversBarney-Regular"; // Nome lógico definido no CustomFontLoader

// Construtor da classe TabTwo
TabTwo::TabTwo(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    // Define cores para o tema escuro
    wxColour backgroundColor(30, 30, 30);
    wxColour textColor(*wxWHITE);
    wxColour buttonColor(70, 70, 70);
    wxColour textCtrlBg(50, 50, 50);
    wxColour textCtrlFg(*wxWHITE);

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

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

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    // Carregar a fonte personalizada
    bool fontLoaded = LoadCustomFonts();

    if (!fontLoaded) {
        wxLogWarning("Falha ao carregar fontes personalizadas. Usando fontes padrão.");
    }

    // Criação dos controles de entrada para cada label
    for (size_t i = 0; i < labels.size(); ++i) {
        if (labels[i] == "-") {
            continue; // Ignorar entradas com "-"
        }

        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        // Label acima do campo de texto
        wxStaticText* label = new wxStaticText(this, wxID_ANY, labels[i]);
        label->SetForegroundColour(textColor);

        // Aplicar a fonte OliversBarney-Regular somente se fontLoaded for verdadeiro
        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                label->SetFont(customFont);
            } else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao label.");
                label->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        } else {
            label->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        entrySizer->Add(label, 0, wxALIGN_LEFT | wxALL, 5);

        // Campo de texto
        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
            wxTE_PROCESS_ENTER);
        textField->SetBackgroundColour(textCtrlBg);
        textField->SetForegroundColour(textCtrlFg);

        // Aplicar a fonte OliversBarney-Regular ao campo de texto
        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                textField->SetFont(customFont);
            } else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao campo de texto.");
                textField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        } else {
            textField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

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
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    SetScrollRate(10, 10);

    // Bind do evento do botão "Salvar"
    saveButton->Bind(wxEVT_BUTTON, &TabTwo::OnSaveButtonClicked, this);
}

// Evento para o botão "Salvar"
void TabTwo::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for (size_t i = 0; i < entries.size(); i++) {
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

    // Salvar com codificação UTF-8
    wxFileOutputStream outputStream(filePath);
    if (!outputStream.IsOk()) {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxTextOutputStream textStream(outputStream, wxEOL_NATIVE, wxConvUTF8);
    textStream.WriteString(output);

    wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
}