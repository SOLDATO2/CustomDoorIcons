#include "TabTwo.h"
#include "CustomFontLoader.h"
#include "IconsCharacters.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <wx/colordlg.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/richtext/richtextbuffer.h>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <iomanip>

// Constantes de fonte
const std::string OLIVERS_BARNEY_REGULAR = "OliversBarney-Regular";

// Implementação da função ColourToHex
std::string TabTwo::ColourToHex(const wxColour& color) {
    wxString hex = wxString::Format("#%02X%02X%02X", color.Red(), color.Green(), color.Blue());
    return std::string(hex.mb_str());
}

// Função para escapar caracteres especiais no XML
std::string TabTwo::EscapeXML(const std::string& text) {
    std::string escaped;
    for (char c : text) {
        switch(c) {
            case '&': escaped += "&amp;"; break;
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&apos;"; break;
            default: escaped += c;
        }
    }
    return escaped;
}

// Reimplementação da função GetFormattedText
std::string TabTwo::GetFormattedText(wxRichTextCtrl* ctrl, const wxColour& defaultColor) {
    std::stringstream formattedText;
    long length = ctrl->GetLastPosition();

    // Log do início da função e do defaultColor
    std::string defaultColorHex = ColourToHex(defaultColor);
    wxLogMessage("GetFormattedText called. Default Color: %s", defaultColorHex);

    if (length == 0) {
        wxLogMessage("GetFormattedText: Control has no text.");
        return formattedText.str();
    }

    long pos = 0;

    while (pos < length) {
        wxTextAttr attr;
        ctrl->GetStyle(pos, attr);
        wxColour currentColor = attr.GetTextColour();

        // Log da cor atual
        std::string currentColorHex = ColourToHex(currentColor);
        wxLogMessage("Pos: %ld, Color: %s", pos, currentColorHex);

        // Determinar o fim do run atual com a mesma cor
        long runStart = pos;
        while (pos < length) {
            wxTextAttr runAttr;
            ctrl->GetStyle(pos, runAttr);
            wxColour runColor = runAttr.GetTextColour();

            // Comparação direta dos componentes RGB
            if (runColor.Red() != currentColor.Red() ||
                runColor.Green() != currentColor.Green() ||
                runColor.Blue() != currentColor.Blue()) {
                break;
            }
            pos++;
        }
        long runEnd = pos;

        // Extrair o texto do run atual
        wxString runTextWx = ctrl->GetRange(runStart, runEnd);
        std::string runText = std::string(runTextWx.mb_str());

        // Escapar caracteres especiais
        std::string escapedText = EscapeXML(runText);

        // Comparar com a cor padrão
        bool colorsDiffer = (currentColor.Red() != defaultColor.Red() ||
                             currentColor.Green() != defaultColor.Green() ||
                             currentColor.Blue() != defaultColor.Blue());

        // Log da comparação de cores
        wxLogMessage("Run [%ld, %ld): Comparing colors - Current: %s, Default: %s. Differ? %s",
                    runStart, runEnd, currentColorHex, defaultColorHex, colorsDiffer ? "Yes" : "No");

        if (colorsDiffer) {
            // Adicionar tag de cor
            formattedText << "<color=" << currentColorHex << ">" << escapedText << "</color>";
            wxLogMessage("Added colored text: <color=%s>%s</color>", currentColorHex, escapedText);
        }
        else {
            // Adicionar texto sem tag
            formattedText << escapedText;
            wxLogMessage("Added default colored text: %s", escapedText);
        }
    }

    // Log do texto formatado final
    std::string finalText = formattedText.str();
    wxLogMessage("Formatted Text: %s", finalText);

    return finalText;
}

// Implementação do construtor TabTwo
TabTwo::TabTwo(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    // Inicialização das cores principais como membros
    backgroundColor = wxColour(30, 30, 30);
    textColor = *wxWHITE; // Cor do texto dos labels
    buttonColor = wxColour(70, 70, 70);
    textCtrlBg = wxColour(50, 50, 50);
    textCtrlFg = *wxBLACK; // Definido como preto para alinhar com o texto padrão

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

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

    // Inicializa o vetor 'entries' com o tamanho de 'labels'
    entries.resize(labels.size(), TabTwoEntryControls{nullptr});

    // Criação do sizer principal para os grupos
    wxBoxSizer* groupsSizer = new wxBoxSizer(wxVERTICAL);

    // Carregar a fonte personalizada
    bool fontLoaded = LoadCustomFonts();

    if (!fontLoaded) {
        //wxLogWarning("Falha ao carregar fontes personalizadas. Usando fontes padrão.");
    }

    // Mapeamento de cores para cada label específico
    std::unordered_map<std::string, wxColour> labelColors = {
        {"SCP-173", wxColour(236, 34, 34)},
        {"SCP-079", wxColour(236, 34, 34)},
        {"SCP-106", wxColour(236, 34, 34)},
        {"SCP-096", wxColour(236, 34, 34)},
        {"SCP-049", wxColour(236, 34, 34)},
        {"SCP-049-2", wxColour(236, 34, 34)},
        {"SCP-939", wxColour(236, 34, 34)},
        {"SCP-3114", wxColour(236, 34, 34)},
        {"SCP-1507", wxColour(255, 89, 202)},
        {"SCP-1507-Alpha", wxColour(233, 76, 183)},
        {"SCP-1507-049", wxColour(236, 34, 34)},
        {"Facility Guard", wxColour(84, 98, 121)},
        {"Class-D Personnel", wxColour(255, 128, 8)},
        {"Scientist", wxColour(255, 255, 130)},
        {"Chaos Insurgency Rifleman", wxColour(0, 143, 28)},
        {"Chaos Insurgency Repressor", wxColour(21, 133, 61)},
        {"Chaos Insurgency Marauder", wxColour(6, 99, 40)},
        {"Chaos Insurgency Conscript", wxColour(85, 145, 1)},
        {"Nine-Tailed Fox Private", wxColour(74, 131, 170)},
        {"Nine-Tailed Fox Sergeant", wxColour(0, 133, 239)},
        {"Nine-Tailed Fox Captain", wxColour(0, 59, 193)},
        {"Nine-Tailed Fox Specialist", wxColour(0, 217, 255)},
        {"Tutorial", wxColour(255, 43, 176)},
        {"Overwatch", wxColour(0, 255, 255)},
    };

    // Mapeamento de ícones para cada label específico
    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> labelIcons = {
        // Civilians
        {"Class-D Personnel", {classdicon_png, classdicon_png_len}},
        {"Scientist", {scientisticon_png, scientisticon_png_len}},

        // Facility Guard
        {"Facility Guard", {guardicon_png, guardicon_png_len}},

        // Chaos Insurgency
        {"Chaos Insurgency Conscript", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Rifleman", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Marauder", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Repressor", {chaosicon_png, chaosicon_png_len}},

        // Nine-Tailed Fox
        {"Nine-Tailed Fox Specialist", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Sergeant", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Captain", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Private", {mtficon_png, mtficon_png_len}},

        // SCPs
        {"SCP-173", {scp173_png, scp173_png_len}},
        {"SCP-079", {scp079_png, scp079_png_len}},
        {"SCP-106", {scp106_png, scp106_png_len}},
        {"SCP-096", {scp096_png, scp096_png_len}},
        {"SCP-049", {scp049_png, scp049_png_len}},
        {"SCP-049-2", {zombie_png, zombie_png_len}},
        {"SCP-939", {scp939_png, scp939_png_len}},
        {"SCP-3114", {scp3114_png, scp3114_png_len}},
        {"SCP-1507", {scp1507_png, scp1507_png_len}},
        {"SCP-1507-Alpha", {scp1507_png, scp1507_png_len}},
        {"SCP-1507-049", {scp1507_png, scp1507_png_len}},

        // Others
        {"Tutorial", {generic_png, generic_png_len}},
        {"Overwatch", {generic_png, generic_png_len}},
        {"Filmmaker", {generic_png, generic_png_len}},
        {"Spectator", {generic_png, generic_png_len}}
    };

    // Agrupamento de labels
    std::vector<std::string> group1; // SCP-
    std::vector<std::string> group2; // Class-D Personnel e Scientist
    std::vector<std::string> group3; // Nine-Tailed Fox Sergeant, Captain, Private, Facility Guard, Specialist
    std::vector<std::string> group4; // Chaos Insurgency Conscript, Rifleman, Marauder, Repressor
    std::vector<std::string> group5; // Outros (incluindo "-")

    for (size_t i = 0; i < labels.size(); ++i) {
        const std::string& label = labels[i];

        if (label == "-") {
            group5.push_back(label); // Adiciona "-" no grupo5 para mantê-lo
            continue;
        }

        if (label.substr(0, 4) == "SCP-") {
            group1.push_back(label);
        }
        else if (label == "Class-D Personnel" || label == "Scientist") {
            group2.push_back(label);
        }
        else if (label == "Nine-Tailed Fox Sergeant" ||
                 label == "Nine-Tailed Fox Captain" ||
                 label == "Nine-Tailed Fox Private" ||
                 label == "Nine-Tailed Fox Specialist" ||
                 label == "Facility Guard") {
            group3.push_back(label);
        }
        else if (label == "Chaos Insurgency Conscript" ||
                 label == "Chaos Insurgency Rifleman" ||
                 label == "Chaos Insurgency Marauder" ||
                 label == "Chaos Insurgency Repressor") {
            group4.push_back(label);
        }
        else {
            group5.push_back(label);
        }
    }

    // Função lambda para criar controles de entrada com wxRichTextCtrl
    auto CreateEntryControls = [&](const std::string& labelStr, size_t originalIndex) -> wxBoxSizer* {
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

        // Label
        wxStaticText* label = new wxStaticText(this, wxID_ANY, labelStr);

        auto it = labelColors.find(labelStr);
        if (it != labelColors.end()) {
            label->SetForegroundColour(it->second);
        }
        else {
            label->SetForegroundColour(textColor);
        }

        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                label->SetFont(customFont);
            }
            else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao label.");
                label->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        }
        else {
            label->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        headerSizer->Add(label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

        // Ícone
        wxBitmap iconBitmap;
        auto iconIt = labelIcons.find(labelStr);
        if (iconIt != labelIcons.end()) {
            wxMemoryInputStream imgStream(iconIt->second.first, iconIt->second.second);
            wxImage image(imgStream, wxBITMAP_TYPE_PNG);
            if (image.IsOk()) {
                wxImage resizedImage = image.Scale(30, 30, wxIMAGE_QUALITY_HIGH);
                iconBitmap = wxBitmap(resizedImage);
            }
            else {
                //wxLogWarning("Falha ao carregar a imagem para o label: %s", labelStr);
            }
        }

        if (iconBitmap.IsOk()) {
            wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, iconBitmap);
            headerSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
        }

        // Botão para selecionar a cor
        wxButton* colorButton = new wxButton(this, wxID_ANY, "Cor");
        colorButton->SetBackgroundColour(buttonColor);
        colorButton->SetForegroundColour(textColor);
        headerSizer->Add(colorButton, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

        entrySizer->Add(headerSizer, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        // Substituir wxTextCtrl por wxRichTextCtrl
        wxRichTextCtrl* richTextCtrl = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 100), wxRE_MULTILINE | wxBORDER_SUNKEN); // Adicionado wxDefaultPosition
        richTextCtrl->SetBackgroundColour(textCtrlBg);
        richTextCtrl->SetForegroundColour(textCtrlFg);

        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                richTextCtrl->SetFont(customFont);
            }
            else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao campo de texto.");
                richTextCtrl->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        }
        else {
            richTextCtrl->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        // Definir o estilo padrão para textCtrlFg (preto)
        wxRichTextAttr defaultStyle;
        defaultStyle.SetTextColour(textCtrlFg);
        richTextCtrl->SetDefaultStyle(defaultStyle);

        // Ajuste de tamanho para o wxRichTextCtrl
        richTextCtrl->SetMinSize(wxSize(-1, 100)); // Define uma altura mínima de 100 pixels

        entrySizer->Add(richTextCtrl, 0, wxEXPAND | wxBOTTOM, 10); // Proporção 0 para evitar expansão vertical indesejada

        // Ligação do botão de cor com a função para mudar a cor do texto
        colorButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
            wxColourData data;
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK) {
                wxColour color = dialog.GetColourData().GetColour();
                if (color.IsOk()) {
                    // Verifica se há seleção de texto
                    long start, end;
                    richTextCtrl->GetSelection(&start, &end);
                    if (start != end) {
                        // Aplica a cor ao texto selecionado
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        richTextCtrl->SetStyle(start, end, attr);
                    }
                    else {
                        // Define a cor para o texto futuro
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        richTextCtrl->SetDefaultStyle(attr);
                    }
                }
            }
        });

        // Armazena o controle no vetor de entries
        entries[originalIndex].richTextCtrl = richTextCtrl;

        return entrySizer;
    };

    // Função lambda para adicionar grupos ao sizer
    auto AddGroupToSizer = [&](const std::vector<std::string>& group, wxBoxSizer* parentSizer, const std::string& groupTitle) {
        if (group.empty()) return;

        wxStaticText* title = new wxStaticText(this, wxID_ANY, groupTitle);
        title->SetForegroundColour(wxColour(255, 255, 255));
        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (customFont.IsOk()) {
                title->SetFont(customFont);
            }
            else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao título do grupo.");
                title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            }
        }
        else {
            title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        }

        parentSizer->Add(title, 0, wxALL, 5);

        wxBoxSizer* groupSizer = new wxBoxSizer(wxVERTICAL);
        for (const auto& labelStr : group) {
            size_t originalIndex = 0;
            for (; originalIndex < labels.size(); ++originalIndex) {
                if (labels[originalIndex] == labelStr) {
                    break;
                }
            }

            if (originalIndex < labels.size()) {
                wxBoxSizer* entrySizer = CreateEntryControls(labelStr, originalIndex);
                groupSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 2);
            }
        }

        parentSizer->Add(groupSizer, 0, wxEXPAND | wxALL, 5);
    };

    // Adiciona os grupos ao sizer principal
    AddGroupToSizer(group1, groupsSizer, "SCPs");
    AddGroupToSizer(group2, groupsSizer, "Civilians");
    AddGroupToSizer(group3, groupsSizer, "Mobile Task Force Unit");
    AddGroupToSizer(group4, groupsSizer, "Chaos Insurgency");
    AddGroupToSizer(group5, groupsSizer, "Others");

    mainSizer->Add(groupsSizer, 1, wxEXPAND | wxALL, 10);

    // Botão de salvar
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    // Botão de teste
    wxButton* testButton = new wxButton(this, wxID_ANY, "Test Formatted Text");
    testButton->SetBackgroundColour(buttonColor);
    testButton->SetForegroundColour(textColor);
    mainSizer->Add(testButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    SetScrollRate(10, 10);

    // Ligação do evento de salvar
    saveButton->Bind(wxEVT_BUTTON, &TabTwo::OnSaveButtonClicked, this);

    // Ligação do evento de teste
    testButton->Bind(wxEVT_BUTTON, &TabTwo::TestFormattedText, this); // Agora compatível com a assinatura
}

// Função para salvar os dados no arquivo
void TabTwo::OnSaveButtonClicked(wxCommandEvent& event) {
    std::stringstream output;

    for (size_t i = 0; i < labels.size(); i++) {
        if (labels[i] == "-") {
            output << "-\n";
            continue;
        }

        if (entries[i].richTextCtrl && !entries[i].richTextCtrl->GetValue().IsEmpty()) {
            std::string formattedText = GetFormattedText(entries[i].richTextCtrl, textCtrlFg);
            output << formattedText << "\n";
        }
        else {
            output << labels[i] << "\n";
        }
    }

    // Converter std::string para wxString
    wxString outputStr = wxString::FromUTF8(output.str().c_str());

    wxFileDialog saveFileDialog(
        this,
        _("Save Classes_Names.txt file"),
        "",
        "Classes_Names.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    wxFileOutputStream outputStream(filePath);
    if (!outputStream.IsOk()) {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxTextOutputStream textStream(outputStream, wxEOL_NATIVE, wxConvUTF8);
    textStream.WriteString(outputStr);

    wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
}

// Função para testar a formatação
void TabTwo::TestFormattedText(wxCommandEvent& event) { // Atualizado para aceitar wxCommandEvent&
    // Criar um wxRichTextCtrl temporário
    wxRichTextCtrl* testCtrl = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, 100), wxRE_MULTILINE | wxBORDER_SUNKEN);
    testCtrl->SetBackgroundColour(textCtrlBg);
    testCtrl->SetForegroundColour(textCtrlFg);

    // Definir o estilo padrão para textCtrlFg (preto)
    wxRichTextAttr defaultStyle;
    defaultStyle.SetTextColour(textCtrlFg);
    testCtrl->SetDefaultStyle(defaultStyle);

    // Inserir texto "peanut"
    testCtrl->WriteText("peanut");

    // Selecionar "pe" e aplicar a cor #00FFFF
    testCtrl->SetSelection(0, 2); // Seleciona "pe"
    wxRichTextAttr attr;
    attr.SetTextColour(wxColour(0, 255, 255)); // #00FFFF
    testCtrl->SetStyle(0, 2, attr);
    testCtrl->SetSelection(-1, -1); // Desselecionar

    // Salvar o texto formatado
    std::string formattedText = GetFormattedText(testCtrl, textCtrlFg);

    // Log do texto formatado
    wxLogMessage("Test Formatted Text: %s", formattedText);

    // Exibir o texto formatado em uma mensagem
    wxMessageBox(wxString::FromUTF8(formattedText.c_str()), "Test Formatted Text", wxOK | wxICON_INFORMATION);

    // Limpar
    testCtrl->Destroy();
}
