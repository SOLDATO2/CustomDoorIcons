// TabTwo.cpp
#include "TabTwo.h"
#include "CustomFontLoader.h"
#include "icons.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <iostream>
#include <unordered_map>

const std::string OLIVERS_BARNEY_REGULAR = "OliversBarney-Regular";

TabTwo::TabTwo(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
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

    // Inicializa o vetor 'entries' com o tamanho de 'labels'
    entries.resize(labels.size(), TabTwoEntryControls{nullptr});

    // Criação do sizer principal para os grupos
    wxBoxSizer* groupsSizer = new wxBoxSizer(wxVERTICAL);

    // Carregar a fonte personalizada
    bool fontLoaded = LoadCustomFonts();

    if (!fontLoaded) {
        wxLogWarning("Falha ao carregar fontes personalizadas. Usando fontes padrão.");
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

    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> labelIcons = {
        //civilians
        {"Class-D Personnel", {classdicon_png, classdicon_png_len}},
        {"Scientist", {scientisticon_png, scientisticon_png_len}},

        // Facility Guard
        {"Facility Guard", {guardicon_png, guardicon_png_len}},

        //Chaos Insurgency
        {"Chaos Insurgency Conscript", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Rifleman", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Marauder", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Repressor", {chaosicon_png, chaosicon_png_len}},

        //Nine-Tailed Fox
        {"Nine-Tailed Fox Specialist", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Sergeant", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Captain", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Private", {mtficon_png, mtficon_png_len}},
        //scps
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
        
        //others
        {"Tutorial", {generic_png, generic_png_len}},
        {"Overwatch", {generic_png, generic_png_len}},
        {"Filmmaker", {generic_png, generic_png_len}},
        {"Spectator", {generic_png, generic_png_len}}
    };

    // Definição dos grupos
    std::vector<std::string> group1; // SCP-
    std::vector<std::string> group2; // Class-D Personnel e Scientist
    std::vector<std::string> group3; // Nine-Tailed Fox Sergeant, Captain, Private, Facility Guard, Specialist
    std::vector<std::string> group4; // Chaos Insurgency Conscript, Rifleman, Marauder, Repressor
    std::vector<std::string> group5; // Outros (incluindo "-")

    // Categorizar os labels nos grupos apropriados
    for (size_t i = 0; i < labels.size(); ++i) {
        const std::string& label = labels[i];

        if (label == "-") {
            continue; // Não adiciona "-" a nenhum grupo para que não seja exibido na GUI
        }

        // Não ignore labels que são "-"
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

    // Função lambda para criar controles de entrada com ícone próximo ao label e acima do textfield
    auto CreateEntryControls = [&](const std::string& labelStr, size_t originalIndex) -> wxBoxSizer* {
        // Usar wxBoxSizer vertical para header (label + ícone) e textfield
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        // Criar sizer horizontal para label e ícone
        wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

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
                wxLogWarning("Falha ao carregar a imagem para o label: %s", labelStr);
            }
        }

        if (iconBitmap.IsOk()) {
            wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, iconBitmap);
            headerSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL);
        }

        entrySizer->Add(headerSizer, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
            wxTE_PROCESS_ENTER);
        textField->SetBackgroundColour(textCtrlBg);
        textField->SetForegroundColour(textCtrlFg);

        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                textField->SetFont(customFont);
            }
            else {
                wxLogWarning("Falha ao aplicar a fonte OliversBarney-Regular ao campo de texto.");
                textField->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        }
        else {
            textField->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        entrySizer->Add(textField, 1, wxEXPAND | wxBOTTOM, 10);

        entries[originalIndex].textField = textField;

        return entrySizer;
    };

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

    AddGroupToSizer(group1, groupsSizer, "SCPs");
    AddGroupToSizer(group2, groupsSizer, "Civilians");
    AddGroupToSizer(group3, groupsSizer, "Mobile Task Force Unit");
    AddGroupToSizer(group4, groupsSizer, "Chaos Insurgency");
    AddGroupToSizer(group5, groupsSizer, "Others");

    mainSizer->Add(groupsSizer, 1, wxEXPAND | wxALL, 10);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    SetScrollRate(10, 10);

    saveButton->Bind(wxEVT_BUTTON, &TabTwo::OnSaveButtonClicked, this);
}

void TabTwo::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for (size_t i = 0; i < labels.size(); i++) {
        if (labels[i] == "-") {
            output += "-\n";
            continue;
        }

        wxString text;
        if (entries[i].textField && !entries[i].textField->GetValue().IsEmpty()) {
            text = entries[i].textField->GetValue();
        }
        else {
            text = wxString::FromUTF8(labels[i].c_str());
        }

        output += text + "\n";
    }

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
    textStream.WriteString(output);

    wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
}