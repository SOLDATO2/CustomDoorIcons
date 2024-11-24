// src/main.cpp
#include <wx/wx.h>
#include "TabOne.h"
#include "TabTwo.h"
#include <wx/notebook.h>

// Classe principal da aplicação wxWidgets
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Inicializar todos os manipuladores de imagem do wxWidgets
    wxInitAllImageHandlers();

    // Criar a janela principal
    wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Custom Door Icons", wxDefaultPosition, wxDefaultSize);
    SetTopWindow(frame);

    // Criar o notebook (abas)
    wxNotebook* notebook = new wxNotebook(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);

    // Criar as duas abas
    TabOne* tabOne = new TabOne(notebook);
    TabTwo* tabTwo = new TabTwo(notebook);

    // Adicionar as abas ao notebook
    notebook->AddPage(tabOne, "Aba 1");
    notebook->AddPage(tabTwo, "Aba 2");

    // Configurar o sizer da janela principal e ajustar o tamanho
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 10);
    frame->SetSizerAndFit(mainSizer); // Ajusta o tamanho da janela para caber todo o conteúdo

    frame->Show(true);
    return true; // Iniciar o loop de eventos
}
