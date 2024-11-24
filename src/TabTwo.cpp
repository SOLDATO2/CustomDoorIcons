// src/TabTwo.cpp
#include "TabTwo.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <wx/notebook.h>
#include <wx/clipbrd.h>

// Função para processar a imagem e gerar a string HTML
std::string imageToProcessedHTML(const std::string& imagePath, int width) {
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cerr << "Error, could not load image!" << std::endl;
        return "";
    }
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);  // Converter para RGB

    // Calcular o novo tamanho mantendo a proporção
    double aspectRatio = static_cast<double>(img.rows) / img.cols;
    int newHeight = static_cast<int>(aspectRatio * width * 0.55);
    cv::resize(img, img, cv::Size(width, newHeight));

    // Gerar a string HTML
    std::string htmlOutput;
    const std::string blockChar = "█"; // Caractere de bloco completo

    htmlOutput += "<size=2>";
    for (int y = 0; y < img.rows; y++) {
        std::string currentColor = "";
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
            int r = pixel[0], g = pixel[1], b = pixel[2];
            char hexColor[8];
            snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", r, g, b);

            std::string hexColorStr(hexColor);

            // Verificar se a cor atual é diferente da anterior para agrupar caracteres
            if (currentColor != hexColorStr) {
                if (!currentColor.empty()) {
                    htmlOutput += "</color>";
                }
                currentColor = hexColorStr;
                htmlOutput += "<color=" + currentColor + ">";
            }
            htmlOutput += blockChar;
        }
        if (!currentColor.empty()) {
            htmlOutput += "</color>";
        }
        htmlOutput += "\\n"; // Inserir a sequência de caracteres \n
    }
    htmlOutput += "</size>";

    return htmlOutput;
}

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

    // Binds dos eventos
    selectImageButton->Bind(wxEVT_BUTTON, &TabTwo::OnSelectImageButtonClicked, this);
    processButton->Bind(wxEVT_BUTTON, &TabTwo::OnProcessButtonClicked, this);
}

void TabTwo::OnSelectImageButtonClicked(wxCommandEvent& event) {
    wxFileDialog openFileDialog(
        this,
        _("Selecione uma imagem"),
        "",
        "",
        "Imagens (*.png;*.jpg;*.jpeg;*.bmp;*.gif)|*.png;*.jpg;*.jpeg;*.bmp;*.gif",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        wxMessageBox("Nenhuma imagem foi selecionada.", "Aviso", wxOK | wxICON_INFORMATION);
        return;
    }

    imagePath = openFileDialog.GetPath().ToStdString();
    imagePathCtrl->SetValue(openFileDialog.GetPath());
}

void TabTwo::OnProcessButtonClicked(wxCommandEvent& event) {
    if (imagePath.empty()) {
        wxMessageBox("Por favor, selecione uma imagem primeiro.", "Aviso", wxOK | wxICON_WARNING);
        return;
    }

    int width = 0;
    try {
        width = std::stoi(widthCtrl->GetValue().ToStdString());
    }
    catch (const std::exception& e) {
        wxMessageBox("Entrada inválida para largura.", "Erro", wxOK | wxICON_ERROR);
        return;
    }

    if (width < 10 || width > 300) {
        wxMessageBox("Largura inválida fornecida. Deve estar entre 10 e 300.", "Erro", wxOK | wxICON_ERROR);
        return;
    }

    //Gerar o HTML processado a partir da imagem
    std::string processedHTML = imageToProcessedHTML(imagePath, width);

    //Converter a string para wxString usando UTF-8
    wxString wxProcessedHTML = wxString::FromUTF8(processedHTML);

    //Copiar a string para o clipboard
    if (wxTheClipboard->Open()) {
        //Criar um objeto de dados de texto com a string Unicode
        wxTextDataObject* data = new wxTextDataObject(wxProcessedHTML);
        wxTheClipboard->SetData(data);
        wxTheClipboard->Close();
        wxMessageBox("Copied to clipboard.", "Sucesso", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Could not access clipboard.", "Erro", wxOK | wxICON_ERROR);
    }
}
