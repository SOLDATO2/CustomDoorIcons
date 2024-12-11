// TabOne.cpp
#include "TabOne.h"
#include "ImageProcessor.h"
#include "CustomFontLoader.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <iostream>


const std::string ROBOTO_BOLD = "Roboto Bold";


TabOne::TabOne(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    wxColour backgroundColor(30, 30, 30);
    wxColour textColor(*wxWHITE);
    wxColour buttonColor(70, 70, 70);
    wxColour textCtrlBg(50, 50, 50);
    wxColour textCtrlFg(*wxWHITE);

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

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

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    bool fontsLoaded = LoadCustomFonts();

    if (!fontsLoaded) {
        
    }

    for (size_t i = 0; i < labels.size(); ++i) {
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label = new wxStaticText(this, wxID_ANY, labels[i]);
        label->SetForegroundColour(textColor);

        if (fontsLoaded) {
            wxFont customFont = GetCustomFont(ROBOTO_BOLD, 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (customFont.IsOk()) {
                label->SetFont(customFont);
            } else {
                //wxLogWarning("Falha ao aplicar a fonte Roboto Bold ao label.");
                label->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            }
        } else {
            label->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        }

        entrySizer->Add(label, 0, wxALIGN_LEFT | wxALL, 5);

        wxBoxSizer* fieldsSizer = new wxBoxSizer(wxHORIZONTAL);

        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
            wxTE_PROCESS_ENTER);
        textField->SetBackgroundColour(textCtrlBg);
        textField->SetForegroundColour(textCtrlFg);

        if (fontsLoaded) {
            wxFont customFont = GetCustomFont(ROBOTO_BOLD, 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (customFont.IsOk()) {
                textField->SetFont(customFont);
            } else {
                
                textField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        } else {
            textField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        fieldsSizer->Add(textField, 1, wxEXPAND | wxRIGHT, 5);

        wxTextCtrl* sizeField = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1),
            wxTE_PROCESS_ENTER);
        sizeField->SetBackgroundColour(textCtrlBg);
        sizeField->SetForegroundColour(textCtrlFg);

        if (fontsLoaded) {
            wxFont customFont = GetCustomFont(ROBOTO_BOLD, 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (customFont.IsOk()) {
                sizeField->SetFont(customFont);
            } else {
                
                sizeField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        } else {
            sizeField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        fieldsSizer->Add(sizeField, 0, wxEXPAND | wxALL, 5);

        wxColourPickerCtrl* colorPicker = new wxColourPickerCtrl(this, wxID_ANY, defaultColors[i]);
        fieldsSizer->Add(colorPicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        wxCheckBox* ignoreSizeCheckBox = new wxCheckBox(this, wxID_ANY, "Ignore size");
        ignoreSizeCheckBox->SetForegroundColour(textColor);
        fieldsSizer->Add(ignoreSizeCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        wxButton* selectImageButton = new wxButton(this, wxID_ANY, "Select Image");
        selectImageButton->SetBackgroundColour(buttonColor);
        selectImageButton->SetForegroundColour(textColor);
        fieldsSizer->Add(selectImageButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        wxButton* resetImageButton = new wxButton(this, wxID_ANY, "Reset");
        resetImageButton->SetBackgroundColour(buttonColor);
        resetImageButton->SetForegroundColour(textColor);
        resetImageButton->Hide();
        fieldsSizer->Add(resetImageButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

        TabOneEntryControls controls = { textField, sizeField, colorPicker, ignoreSizeCheckBox, selectImageButton, resetImageButton, "" };
        entries.push_back(controls);

        entrySizer->Add(fieldsSizer, 0, wxEXPAND | wxALL, 5);

        entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 2);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);

    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i].selectImageButton->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent& event) {
            this->OnSelectImageButtonClicked(i);
        });

        entries[i].resetImageButton->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent& event) {
            this->OnResetImageButtonClicked(i);
        });
    }

    saveButton->Bind(wxEVT_BUTTON, &TabOne::OnSaveButtonClicked, this);

    SetScrollRate(10, 10);
}

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

    entries[index].imagePath = openFileDialog.GetPath().ToStdString();
    entries[index].textField->SetValue("Image Selected");

    entries[index].sizeField->Disable();
    entries[index].colorPicker->Disable();
    entries[index].ignoreSizeCheckBox->Disable();

    entries[index].resetImageButton->Show();

    Layout();
}

void TabOne::OnResetImageButtonClicked(size_t index) {

    entries[index].imagePath = "";

    entries[index].textField->SetValue("");

    entries[index].sizeField->Enable();
    entries[index].colorPicker->Enable();
    entries[index].ignoreSizeCheckBox->Enable();

    entries[index].resetImageButton->Hide();

    Layout();
}


void TabOne::OnSaveButtonClicked(wxCommandEvent& event) {
    // Vamos construir o texto de saída.
    // Para cada entry, se houver imagem, imprime o texto da imagem;
    // caso contrário, imprime o label padrão com a cor padrão.

    wxString output;

    // Pressupondo que `labels` e `defaultColors` correspondem 1 a 1 com entries.
    // Loop sobre todos os entries
    for (size_t i = 0; i < entries.size(); ++i) {
        if (!entries[i].imagePath.empty()) {
            // Há uma imagem selecionada para este label
            int width = 80; // conforme o código original
            wxString imageText = imageToProcessedHTML(entries[i].imagePath, width);
            // Adicionar o texto da imagem no output
            output << imageText << "\n";
        } else {
            // Não há imagem, apenas imprimir o label com cor padrão
            // A cor padrão está em defaultColors[i], o nome do label em labels[i]
            wxString hexColor = defaultColors[i].GetAsString(wxC2S_HTML_SYNTAX);
            wxString labelName = wxString::FromUTF8(labels[i].c_str());
            output << "<color=" << hexColor << ">" << labelName << "</color>\n";
        }
    }

    // Agora salvamos em Doors.txt
    wxFileDialog saveFileDialog(
        this,
        _("Save Doors.txt file"),
        "",
        "Doors.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();
    wxFile file;
    if (!file.Create(filePath, true)) {
        wxMessageBox("Não foi possível criar o arquivo Doors.txt.", "Erro", wxOK | wxICON_ERROR);
        return;
    }

    // Escreve BOM UTF-8
    unsigned char bom[3] = {0xEF, 0xBB, 0xBF};
    file.Write(bom, 3);

    // Converte para UTF-8
    auto utf8Data = output.utf8_str();
    file.Write(utf8Data.data(), utf8Data.length());
    file.Close();

    wxMessageBox("Arquivo Doors.txt criado com sucesso!", "Sucesso", wxOK | wxICON_INFORMATION);
}
