// TabFour.cpp
#include "TabFour.h"
#include "CustomFontLoader.h"
#include "IconsItems.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/statline.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/mstream.h>
#include <iostream>
#include <string>
#include <unordered_map>

//#ifdef _WIN32
//#include <windows.h>
//#endif


const std::string ROBOTO_BOLD = "Roboto Bold";

TabFour::TabFour(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    wxColour backgroundColor(30, 30, 30);
    wxColour textColor(*wxWHITE);
    wxColour buttonColor(70, 70, 70);
    wxColour textCtrlBg(50, 50, 50);
    wxColour textCtrlFg(*wxWHITE);

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    bool fontLoaded = LoadCustomFonts();

    if (!fontLoaded) {
        wxLogWarning("Falha ao carregar fontes personalizadas. Usando fontes padrão.");
    }

    items = {
        {0, "Janitor Keycard", ""}, //ok
        {1, "Scientist Keycard", ""}, //ok
        {2, "Research Supervisor Keycard", ""}, //ok
        {3, "Zone Manager Keycard", ""}, //ok
        {4, "Guard Keycard", ""}, //ok
        {5, "MTF Private Keycard", ""}, //ok
        {6, "Containment Engineer Keycard", ""}, //ok
        {7, "MTF Operative Keycard", ""}, //ok
        {8, "MTF Captain Keycard", ""}, //ok
        {9, "Facility Manager Keycard", ""}, //ok
        {10, "Chaos Insurgency Access Device", ""} , //ok
        {11, "O5-level Keycard", ""}, //ok
        {12, "Radio", "Allows communication over long distances between devices. Range can be adjusted."}, //ok
        {13, "COM-15", ""}, //ok
        {14, "First Aid Kit", "Heals your injuries."}, //ok
        {15, "Flashlight", "Let there be light!"}, //ok
        {16, "Micro H.I.D.", "Heavy electrical device designed for the termination of anomalies."}, //ok
        {17, "SCP-500", "The Panacea. Instantly restores all health and cures most afflictions."}, //ok
        {18, "SCP-207", "Harmfully increases motor skills."}, //ok
        {20, "MTF-E11-SR", ""}, //ok
        {21, "Crossvec", ""}, //ok
        {23, "FSP-9", ""}, //ok
        {24, "Logicer", ""}, //ok
        {25, "High-Explosive Grenade", "A high explosive dealing significant damage. Detonates after a brief delay once thrown."}, //ok
        {26, "Flashbang Grenade", "Blinds and disorients your enemies. Detonates after a brief delay once thrown."}, //ok
        {30, "COM-18", ""}, //ok
        {31, "SCP-018", "Superball with the ability to bounce with extreme efficiency. Deadly ricochet at high speeds."}, //ok
        {32, "SCP-268", "When worn, temporarily makes you invisible unless it is interrupted by any user interaction."}, //ok
        {33, "Adrenaline", "Provides a temporary health boost and a short burst of stamina."}, //ok
        {34, "Painkillers", "Slowly restores health over time."},//ok
        {35, "Coin", "You're not sure why you'd want to carry this around..."}, //ok
        {36, "Light Armor", ""}, //ok
        {37, "Combat Armor", ""},//ok
        {38, "Heavy Armor", ""},//ok
        {39, ".44 Revolver", ""}, //ok
        {40, "AK", ""}, //ok
        {41, "Shotgun", ""}, //ok
        {42, "Bag of Candies", "A bag containing the following candies:"}, //ok
        {43, "SCP-2176", "Your very own Ghostlight! Caution: Fragile. Disrupts electronics when broken."}, //ok
        {44, "SCP-244-A", "An ancient vase, freezing to the touch. Creates a large cloud of icy fog when placed."}, //ok
        {45, "SCP-244-B", "An ancient vase, freezing to the touch. Creates a large cloud of icy fog when placed."}, //ok
        {46, "SCP-1853", "Increases dexterity and weapon handling when your life is in danger."}, //ok
        {47, "3-X Particle Disruptor", "A strange weapon that fires powerful, disintegrating energy beams."}, //ok
        {48, "COM-45", ""}, //ok
        {49, "SCP-1576", "Allows temporary communication with the dead. Can be used again after some time passes."}, //ok
        {50, "Jailbird", "What?!"}, //ok
        {51, "Anti-Cola", "The result of an experiment with SCP-914. Good for your health, bad for your motor skills. Will save your life in a pinch."}, //ok
        {52, "FR-MG-0", ""}, //ok
        {53, "A7", ""}, //ok
        {54, "Lantern", "A classic oil lantern, inscribed with the name 'Daniel'."}, //ok
        {55, "SCP-1344", "A set of modified goggles that lets you perceive the positions of other players through walls. Once worn, they must never be removed!"},
        {56, "Snowball", ""}, //ok
        {57, "Coal", ""}, //ok
        {58, "Coal?", ""}, //ok
        {59, "Tape Player?", "Contains an unlabeled tape. Please don’t play it. Please."}, //ok
        {60, "Tape Player", "Contains an unlabeled tape. Please don’t play it. Please."} //ok
    };

    entries.resize(items.size(), TabFourEntryControls{nullptr, nullptr});

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> itemIcons = {
        {"Janitor Keycard", {__janitorkeycard_png, __janitorkeycard_png_len}},
        {"Scientist Keycard", {__scientistkeycard_png, __scientistkeycard_png_len}},
        {"Research Supervisor Keycard", {__researchkeycard_png, __researchkeycard_png_len}},
        {"Zone Manager Keycard", {__zonemanagerkeycard_png, __zonemanagerkeycard_png_len}},
        {"Guard Keycard", {__guardkeycard_png, __guardkeycard_png_len}},
        {"MTF Private Keycard", {__operativekeycard_png, __operativekeycard_png_len}},
        {"Containment Engineer Keycard", {cengineerkeycard_png, cengineerkeycard_png_len}},
        {"MTF Operative Keycard", {__operativekeycard_png, __operativekeycard_png_len}},
        {"MTF Captain Keycard", {__captainkeycard_png, __captainkeycard_png_len}},
        {"Facility Manager Keycard", {__facilitymanagerkeycard_png, __facilitymanagerkeycard_png_len}},
        {"Chaos Insurgency Access Device", {__chaosinsurgencydevice_png, __chaosinsurgencydevice_png_len}},
        {"O5-level Keycard", {__o5keycard_png, __o5keycard_png_len}},
        {"Radio", {__radio_png, __radio_png_len}},
        {"COM-15", {__com15_png, __com15_png_len}},
        {"First Aid Kit", {__medkit_png, __medkit_png_len}},
        {"Flashlight", {__flashlight_png, __flashlight_png_len}},
        {"Micro H.I.D.", {__micro_png, __micro_png_len}},
        {"SCP-500", {scp500_png, scp500_png_len}},
        {"SCP-207", {scp207_png, scp207_png_len}},
        {"MTF-E11-SR", {__e11_png, __e11_png_len}},
        {"Crossvec", {__crossvec_png, __crossvec_png_len}},
        {"FSP-9", {__fsp_png, __fsp_png_len}},
        {"Logicer", {__logicer_png, __logicer_png_len}},
        {"High-Explosive Grenade", {__granade_png, __granade_png_len}},
        {"Flashbang Grenade", {__flashbang_png, __flashbang_png_len}},
        {"COM-18", {__com18_png, __com18_png_len}},
        {"SCP-018", {scp018_png, scp018_png_len}},
        {"SCP-268", {scp268_png, scp268_png_len}},
        {"Adrenaline", {__adrenaline_png, __adrenaline_png_len}},
        {"Painkillers", {__painkillers_png, __painkillers_png_len}},
        {"Coin", {__coin_png, __coin_png_len}},
        {"Light Armor", {__lightarmor_png, __lightarmor_png_len}},
        {"Combat Armor", {__combatarmor_png, __combatarmor_png_len}},
        {".44 Revolver", {__revolver_png, __revolver_png_len}},
        {"AK", {__ak_png, __ak_png_len}},
        {"Heavy Armor", {__heavyarmor_png, __heavyarmor_png_len}},
        {"Shotgun", {__shotgun_png, __shotgun_png_len}},
        {"Bag of Candies", {__bagofcandies_png, __bagofcandies_png_len}},
        {"SCP-2176", {scp2176_png, scp2176_png_len}},
        {"SCP-244-A", {scp244A_png, scp244A_png_len}},
        {"SCP-244-B", {scp244B_png, scp244B_png_len}},
        {"SCP-1853", {scp1853_png, scp1853_png_len}},
        {"3-X Particle Disruptor", {__particledisruptor_png, __particledisruptor_png_len}},
        {"COM-45", {__com45_png, __com45_png_len}},
        {"SCP-1576", {scp1576_png, scp1576_png_len}},
        {"Jailbird", {__jailbird_png, __jailbird_png_len}},
        {"Anti-Cola", {antiscp207_png, antiscp207_png_len}},
        {"FR-MG-0", {__frmg_png, __frmg_png_len}},
        {"A7", {__a7_png, __a7_png_len}},
        {"Lantern", {__lantern_png, __lantern_png_len}},
        //{"SCP-1344", {__scp1344_png, __scp1344_png_len}},
        {"Snowball", {__snowball_png, __snowball_png_len}},
        {"Coal", {__coal_png, __coal_png_len}},
        {"Coal?", {__coal_png, __coal_png_len}},
        {"Tape Player?", {__tapeplayer_png, __tapeplayer_png_len}},
        {"Tape Player", {__tapeplayer_png, __tapeplayer_png_len}},
    };

    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];

        wxBoxSizer* itemSizer = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* itemNameLabel = new wxStaticText(this, wxID_ANY, wxString::FromUTF8(item.name.c_str()));
        itemNameLabel->SetForegroundColour(textColor);

        wxFont customLabelFont;
        if (fontLoaded) {
            customLabelFont = GetCustomFont(ROBOTO_BOLD, 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (!customLabelFont.IsOk()) {
                wxLogWarning("Failed to apply 'OliversBarney-Regular' to: %s", item.name);
                customLabelFont = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            }
        }
        else {
            customLabelFont = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        }
        itemNameLabel->SetFont(customLabelFont);

        headerSizer->Add(itemNameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

        wxBitmap iconBitmap;
        auto iconIt = itemIcons.find(item.name);
        if (iconIt != itemIcons.end()) {
            wxMemoryInputStream imgStream(iconIt->second.first, iconIt->second.second);
            wxImage image(imgStream, wxBITMAP_TYPE_PNG);
            if (image.IsOk()) {
                wxImage resizedImage = image.Scale(45, 45, wxIMAGE_QUALITY_HIGH);
                iconBitmap = wxBitmap(resizedImage);
            }
            else {
                wxLogWarning("Failed to load image to: %s", item.name);
            }
        }

        if (iconBitmap.IsOk()) {
            wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, iconBitmap);
            headerSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL);
        }
        itemSizer->Add(headerSizer, 0, wxEXPAND | wxBOTTOM, 10);

        wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
        nameLabel->SetForegroundColour(textColor);

        wxFont nameLabelFont;
        if (fontLoaded) {
            nameLabelFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (!nameLabelFont.IsOk()) {
                wxLogWarning("Failed to apply 'OliversBarney-Regular' to: Name.");
                nameLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            }
        }
        else {
            nameLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        }
        nameLabel->SetFont(nameLabelFont);

        itemSizer->Add(nameLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        wxTextCtrl* nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        nameField->SetBackgroundColour(textCtrlBg);
        nameField->SetForegroundColour(textCtrlFg);

        wxFont nameFieldFont;
        if (fontLoaded) {
            nameFieldFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (!nameFieldFont.IsOk()) {
                wxLogWarning("Failed to apply 'OliversBarney-Regular' to: Name.");
                nameFieldFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            }
        }
        else {
            nameFieldFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        }
        nameField->SetFont(nameFieldFont);

        itemSizer->Add(nameField, 0, wxEXPAND | wxBOTTOM, 10);

        wxTextCtrl* descriptionField = nullptr;

        if (!item.description.empty()) {
            wxStaticText* descLabel = new wxStaticText(this, wxID_ANY, "Description:");
            descLabel->SetForegroundColour(textColor);

            wxFont descLabelFont;
            if (fontLoaded) {
                descLabelFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
                if (!descLabelFont.IsOk()) {
                    wxLogWarning("Failed to apply 'OliversBarney-Regular' to: Name.");
                    descLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
                }
            }
            else {
                descLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            }
            descLabel->SetFont(descLabelFont);

            itemSizer->Add(descLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

            descriptionField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
            descriptionField->SetBackgroundColour(textCtrlBg);
            descriptionField->SetForegroundColour(textCtrlFg);

            wxFont descFieldFont;
            if (fontLoaded) {
                descFieldFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
                if (!descFieldFont.IsOk()) {
                    wxLogWarning("Failed to apply 'OliversBarney-Regular' to: Description.");
                    descFieldFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
                }
            }
            else {
                descFieldFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            }
            descriptionField->SetFont(descFieldFont);

            itemSizer->Add(descriptionField, 0, wxEXPAND | wxBOTTOM, 10);
        }

        wxStaticLine* separator = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        separator->SetForegroundColour(textColor);
        separator->SetBackgroundColour(textColor);
        itemSizer->Add(separator, 0, wxEXPAND | wxTOP, 5);

        entriesSizer->Add(itemSizer, 0, wxEXPAND | wxALL, 10);

        TabFourEntryControls controls;
        controls.nameField = nameField;
        controls.descriptionField = descriptionField;
        entries[i] = controls;
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);

    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    saveButton->Bind(wxEVT_BUTTON, &TabFour::OnSaveButtonClicked, this);

    SetSizer(mainSizer);
    SetScrollRate(20, 20);

    Layout();
    FitInside();
}

void TabFour::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;
    for (size_t it = 0; it < items.size(); ++it){
        const auto& item = items[it];
        const auto& controls = entries[it];

        wxString nameText = controls.nameField->GetValue();
        if (nameText.IsEmpty()) {
            nameText = wxString::FromUTF8(item.name.c_str());
        }

        output += wxString::Format("%d~%s", item.id, nameText);

        if (item.description.empty()) {
            output += "\n";
        } else {
            wxString descText = controls.descriptionField->GetValue();
            if (descText.IsEmpty()) {
                descText = wxString::FromUTF8(item.description.c_str());
            }
            output += "~" + descText + "\n";
        }
    }

    wxFileDialog saveFileDialog(
        this,
        _("Save Items.txt file"),
        "",
        "Items.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    wxString filePath = saveFileDialog.GetPath();

    wxFileOutputStream outputStream(filePath);
    if (!outputStream.IsOk()){
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxTextOutputStream textStream(outputStream, wxEOL_NATIVE, wxConvUTF8);
    textStream.WriteString(output);

    wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
}
