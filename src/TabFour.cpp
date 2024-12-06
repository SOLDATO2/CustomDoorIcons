// TabFour.cpp
#include "TabFour.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/statline.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

TabFour::TabFour(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {

    wxColour backgroundColor(30, 30, 30);
    wxColour textColor(*wxWHITE);
    wxColour buttonColor(70, 70, 70);
    wxColour textCtrlBg(50, 50, 50);
    wxColour textCtrlFg(*wxWHITE);

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);


    items = {
        {0, "Janitor Keycard", ""},
        {1, "Scientist Keycard", ""},
        {2, "Research Supervisor Keycard", ""},
        {3, "Zone Manager Keycard", ""},
        {4, "Guard Keycard", ""},
        {5, "MTF Private Keycard", ""},
        {6, "Containment Engineer Keycard", ""},
        {7, "MTF Operative Keycard", ""},
        {8, "MTF Captain Keycard", ""},
        {9, "Facility Manager Keycard", ""},
        {10, "Chaos Insurgency Access Device", ""},
        {11, "O5-level Keycard", ""},
        {12, "Radio", "Allows communication over long distances between devices. Range can be adjusted."},
        {13, "COM-15", ""},
        {14, "First Aid Kit", "Heals your injuries."},
        {15, "Flashlight", "Let there be light!"},
        {16, "Micro H.I.D.", "Heavy electrical device designed for the termination of anomalies."},
        {17, "SCP-500", "The Panacea. Instantly restores all health and cures most afflictions."},
        {18, "SCP-207", "Harmfully increases motor skills."},
        {20, "MTF-E11-SR", ""},
        {21, "Crossvec", ""},
        {23, "FSP-9", ""},
        {24, "Logicer", ""},
        {25, "High-Explosive Grenade", "A high explosive dealing significant damage. Detonates after a brief delay once thrown."},
        {26, "Flashbang Grenade", "Blinds and disorients your enemies. Detonates after a brief delay once thrown."},
        {30, "COM-18", ""},
        {31, "SCP-018", "Superball with the ability to bounce with extreme efficiency. Deadly ricochet at high speeds."},
        {32, "SCP-268", "When worn, temporarily makes you invisible unless it is interrupted by any user interaction."},
        {33, "Adrenaline", "Provides a temporary health boost and a short burst of stamina."},
        {34, "Painkillers", "Slowly restores health over time."},
        {35, "Coin", "You're not sure why you'd want to carry this around..."},
        {36, "Light Armor", ""},
        {37, "Combat Armor", ""},
        {38, "Heavy Armor", ""},
        {39, ".44 Revolver", ""},
        {40, "AK", ""},
        {41, "Shotgun", ""},
        {42, "Bag of Candies", "A bag containing the following candies:"},
        {43, "SCP-2176", "Your very own Ghostlight! Caution: Fragile. Disrupts electronics when broken."},
        {44, "SCP-244-A", "An ancient vase, freezing to the touch. Creates a large cloud of icy fog when placed."},
        {45, "SCP-244-B", "An ancient vase, freezing to the touch. Creates a large cloud of icy fog when placed."},
        {46, "SCP-1853", "Increases dexterity and weapon handling when your life is in danger."},
        {47, "3-X Particle Disruptor", "A strange weapon that fires powerful, disintegrating energy beams."},
        {48, "COM-45", ""},
        {49, "SCP-1576", "Allows temporary communication with the dead. Can be used again after some time passes."},
        {50, "Jailbird", "What?!"},
        {51, "Anti-Cola", "The result of an experiment with SCP-914. Good for your health, bad for your motor skills. Will save your life in a pinch."},
        {52, "FR-MG-0", ""},
        {53, "A7", ""},
        {54, "Lantern", "A classic oil lantern, inscribed with the name 'Daniel'."},
        {55, "SCP-1344", "A set of modified goggles that lets you perceive the positions of other players through walls. Once worn, they must never be removed!"},
        {56, "Snowball", ""},
        {57, "Coal", ""},
        {58, "Coal?", ""},
        {59, "Tape Player?", "Contains an unlabeled tape. Please don’t play it. Please."},
        {60, "Tape Player", "Contains an unlabeled tape. Please don’t play it. Please."}
    };

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

   
    for (const auto& item : items) {

        wxBoxSizer* itemSizer = new wxBoxSizer(wxVERTICAL);

        // Name
        wxStaticText* itemNameLabel = new wxStaticText(this, wxID_ANY, wxString::FromUTF8(item.name.c_str()));
        itemNameLabel->SetForegroundColour(textColor);

        itemNameLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        itemSizer->Add(itemNameLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        // Name Field
        wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
        nameLabel->SetForegroundColour(textColor);
        nameLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

        itemSizer->Add(nameLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        wxTextCtrl* nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
            wxTE_PROCESS_ENTER);
        nameField->SetBackgroundColour(textCtrlBg);
        nameField->SetForegroundColour(textCtrlFg);
        nameField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        itemSizer->Add(nameField, 0, wxEXPAND | wxBOTTOM, 10);

        wxTextCtrl* descriptionField = nullptr; 

        if (!item.description.empty()) {
  
            wxStaticText* descLabel = new wxStaticText(this, wxID_ANY, "Description:");
            descLabel->SetForegroundColour(textColor);
            descLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

            itemSizer->Add(descLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

            descriptionField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                wxTE_PROCESS_ENTER);
            descriptionField->SetBackgroundColour(textCtrlBg);
            descriptionField->SetForegroundColour(textCtrlFg);
            descriptionField->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

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
        entries.push_back(controls);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    saveButton->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

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
