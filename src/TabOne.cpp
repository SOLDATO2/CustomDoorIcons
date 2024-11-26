// src/TabOne.cpp
#include "TabOne.h"
#include "ImageProcessor.h" // Incluir o cabeçalho centralizado
#include <wx/filedlg.h>
#include <wx/file.h>       // Para manipulação de arquivos
#include <iostream>


// Construtor da classe TabOne
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

        // Campo de tamanho (valor padrão 100)
        wxTextCtrl* sizeField = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
        fieldsSizer->Add(sizeField, 0, wxEXPAND | wxALL, 5);

        // Selecionador de cor com valor padrão
        wxColourPickerCtrl* colorPicker = new wxColourPickerCtrl(this, wxID_ANY, defaultColors[i]);
        fieldsSizer->Add(colorPicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Checkbox para indicar se é um texto
        wxCheckBox* isTextCheckBox = new wxCheckBox(this, wxID_ANY, "Ignore size");
        fieldsSizer->Add(isTextCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Botão para selecionar a imagem
        wxButton* selectImageButton = new wxButton(this, wxID_ANY, "Select Image");
        fieldsSizer->Add(selectImageButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Botão para resetar a imagem (inicialmente escondido)
        wxButton* resetImageButton = new wxButton(this, wxID_ANY, "Reset");
        resetImageButton->Hide();
        fieldsSizer->Add(resetImageButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        // Armazenar os controles
        TabOneEntryControls controls = { textField, sizeField, colorPicker, isTextCheckBox, selectImageButton, resetImageButton, "" };
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

    // Bind dos eventos do botão "Salvar"
    saveButton->Bind(wxEVT_BUTTON, &TabOne::OnSaveButtonClicked, this);

    // Bind para os botões de selecionar e resetar imagem usando lambdas para capturar o índice
    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i].selectImageButton->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent& event) {
            this->OnSelectImageButtonClicked(i);
        });

        entries[i].resetImageButton->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent& event) {
            this->OnResetImageButtonClicked(i);
        });
    }
}

// Evento para o botão "Selecionar Imagem"
void TabOne::OnSelectImageButtonClicked(size_t index) {
    wxFileDialog openFileDialog(
        this,
        _("Select an image"),
        "",
        "",
        "Images (*.png;*.jpg;*.jpeg;*.bmp;*.gif)|*.png;*.jpg;*.jpeg;*.bmp;*.gif",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        wxMessageBox("You didn't select an image.", "Warning", wxOK | wxICON_INFORMATION);
        return;
    }

    // Armazenar o caminho da imagem
    entries[index].imagePath = openFileDialog.GetPath().ToStdString();
    // Atualizar o campo de texto para mostrar que uma imagem foi selecionada
    entries[index].textField->SetValue("Image Selected");

    // Desabilitar outros campos
    entries[index].sizeField->Disable();
    entries[index].colorPicker->Disable();
    entries[index].isTextCheckBox->Disable();

    // Mostrar o botão de reset
    entries[index].resetImageButton->Show();

    // Atualizar o layout
    Layout();
}

// Evento para o botão "Resetar Imagem"
void TabOne::OnResetImageButtonClicked(size_t index) {
    // Limpar o caminho da imagem
    entries[index].imagePath = "";
    // Resetar o campo de texto
    entries[index].textField->SetValue("");

    // Reabilitar outros campos
    entries[index].sizeField->Enable();
    entries[index].colorPicker->Enable();
    entries[index].isTextCheckBox->Enable();

    // Esconder o botão de reset
    entries[index].resetImageButton->Hide();

    // Atualizar o layout
    Layout();
}

// Evento para o botão "Salvar"
void TabOne::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for (size_t i = 0; i < entries.size(); ++i) {
        wxString entryContent;

        if (!entries[i].imagePath.empty()) {
            // Processar como imagem
            std::string imagePath = entries[i].imagePath;
            int width = 80; // Valor padrão de largura

            try {
                width = std::stoi(entries[i].sizeField->GetValue().ToStdString());
            }
            catch (const std::exception&) {
                wxMessageBox("Invalid width provided. Please enter a valid number.", "Error", wxOK | wxICON_ERROR);
                return;
            }

            if (width < 10 || width > 300) {
                wxMessageBox("Width must be between 10 and 300 for image processing.", "Error", wxOK | wxICON_ERROR);
                return;
            }

            // Gerar HTML processado da imagem
            entryContent = imageToProcessedHTML(imagePath, width);
        } else {
            // Processar como texto
            wxString text = entries[i].textField->GetValue();
            wxString sizeStr = entries[i].sizeField->GetValue();
            wxColour color = entries[i].colorPicker->GetColour();
            bool isText = entries[i].isTextCheckBox->GetValue();

            if (text.IsEmpty()) {
                text = wxString::FromUTF8(labels[i].c_str());
                isText = true;
            }

            if (isText) {
                entryContent = "<color=" + color.GetAsString(wxC2S_HTML_SYNTAX) + ">" + text + "</color>";
            } else {
                long sizeValue = 90;
                try {
                    sizeValue = std::stol(sizeStr.ToStdString());
                }
                catch (const std::exception&) {
                    sizeValue = 90;
                }
                entryContent = "<color=" + color.GetAsString(wxC2S_HTML_SYNTAX) + "><size=" + std::to_string(sizeValue) + ">" + text + "</size></color>";
            }
        }

        output += entryContent + "\n";
    }

    // Exibir o diálogo de salvamento
    wxFileDialog saveFileDialog(
        this,
        _("Save Doors.txt file"),
        "",
        "Doors.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        // O usuário cancelou a operação de salvamento
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    // Salvar o conteúdo no arquivo especificado
    wxFile file;
    if (file.Open(filePath, wxFile::write)) {
        file.Write(output);
        file.Close();
        wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
    }
}
