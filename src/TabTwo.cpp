// src/TabTwo.cpp
#include "TabTwo.h"
#include "ImageProcessor.h" // Incluir o cabeçalho centralizado
#include <wx/filedlg.h>
#include <wx/clipbrd.h>
#include <iostream>

// Construtor da classe TabTwo
TabTwo::TabTwo(wxNotebook* parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Botão para selecionar a imagem
    wxButton* selectImageButton = new wxButton(this, wxID_ANY, "Select an image");
    mainSizer->Add(selectImageButton, 0, wxALIGN_CENTER | wxALL, 10);

    // Campo para exibir o caminho da imagem selecionada
    imagePathCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    mainSizer->Add(imagePathCtrl, 0, wxEXPAND | wxALL, 10);

    // Campo para inserir a largura
    wxBoxSizer* widthSizer = new wxBoxSizer(wxHORIZONTAL);
    widthSizer->Add(new wxStaticText(this, wxID_ANY, "Width:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    widthCtrl = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
    widthSizer->Add(widthCtrl, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(widthSizer, 0, wxALIGN_CENTER | wxALL, 5);

    // Botão "Processar"
    wxButton* processButton = new wxButton(this, wxID_ANY, "Process image");
    mainSizer->Add(processButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);

    // Bind dos eventos
    selectImageButton->Bind(wxEVT_BUTTON, &TabTwo::OnSelectImageButtonClicked, this);
    processButton->Bind(wxEVT_BUTTON, &TabTwo::OnProcessButtonClicked, this);
}

// Evento para o botão "Selecionar Imagem"
void TabTwo::OnSelectImageButtonClicked(wxCommandEvent& event) {
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

    imagePath = openFileDialog.GetPath().ToStdString();
    imagePathCtrl->SetValue(openFileDialog.GetPath());
}

// Evento para o botão "Processar"
void TabTwo::OnProcessButtonClicked(wxCommandEvent& event) {
    if (imagePath.empty()) {
        wxMessageBox("Please select an image first.", "Warning", wxOK | wxICON_WARNING);
        return;
    }

    int width = 0;
    try {
        width = std::stoi(widthCtrl->GetValue().ToStdString());
    }
    catch (const std::exception&) {
        wxMessageBox("Invalid width provided. Please enter a valid number.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (width < 10 || width > 300) {
        wxMessageBox("Width must be between 10 and 300.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Gerar o HTML processado a partir da imagem
    wxString processedHTML = imageToProcessedHTML(imagePath, width);

    // Encapsular o conteúdo dentro das tags <size=2> e </size>
    wxString finalOutput = "<size=2>" + processedHTML + "</size>";

    // Copiar a saída para o clipboard
    if (wxTheClipboard->Open()) {
        // Criar um objeto de dados de texto com a string Unicode
        wxTextDataObject* data = new wxTextDataObject(finalOutput);
        wxTheClipboard->SetData(data);
        wxTheClipboard->Close();
        wxMessageBox("Copied to clipboard.", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Could not access clipboard.", "Error", wxOK | wxICON_ERROR);
    }
}
