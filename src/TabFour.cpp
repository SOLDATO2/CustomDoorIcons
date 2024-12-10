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
#include <wx/colourdata.h>
#include <wx/colordlg.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>

const std::string ROBOTO_BOLD = "Roboto Bold";

std::string TabFour::ColourToHex(const wxColour& color) {
    wxString hex = wxString::Format("#%02X%02X%02X", color.Red(), color.Green(), color.Blue());
    return std::string(hex.mb_str());
}


std::string TabFour::EscapeXML(const std::string& text) {
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

std::string TabFour::GetFormattedText(wxRichTextCtrl* ctrl) {
    std::stringstream formattedText;
    long length = ctrl->GetLastPosition();

    if (length == 0) {
        return formattedText.str();
    }

    long pos = 0;

    while (pos < length) {
        wxTextAttr attr;
        ctrl->GetStyle(pos, attr);
        wxColour currentColor = attr.GetTextColour();

        long runStart = pos;
        while (pos < length) {
            wxTextAttr runAttr;
            ctrl->GetStyle(pos, runAttr);
            wxColour runColor = runAttr.GetTextColour();

            if (runColor.Red() != currentColor.Red() ||
                runColor.Green() != currentColor.Green() ||
                runColor.Blue() != currentColor.Blue()) {
                break;
            }
            pos++;
        }
        long runEnd = pos;

        wxString runTextWx = ctrl->GetRange(runStart, runEnd);
        std::string runText = std::string(runTextWx.mb_str());

        std::string escapedText = EscapeXML(runText);

        // if color isnt default (black)
        bool isNotBlack = !(currentColor.Red() == 0 && currentColor.Green() == 0 && currentColor.Blue() == 0);

        if (isNotBlack) {
            formattedText << "<color=" << ColourToHex(currentColor) << ">" << escapedText << "</color>";
        }
        else {
            formattedText << escapedText;
        }
    }

    return formattedText.str();
}

TabFour::TabFour(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    backgroundColor = wxColour(30, 30, 30);
    textColor = *wxWHITE;
    buttonColor = wxColour(70, 70, 70);
    textCtrlBg = wxColour(50, 50, 50);
    textCtrlFg = *wxWHITE;

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    bool fontLoaded = LoadCustomFonts();

    if (!fontLoaded) {
        //
    }

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

    entries.resize(items.size(), TabFourEntryControls{nullptr, nullptr, nullptr, nullptr});

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> itemIcons = {
        {"Janitor Keycard", {__janitorkeycard_png, __janitorkeycard_png_len}},
        {"Scientist Keycard", {__scientistkeycard_png, __scientistkeycard_png_len}},
        {"Research Supervisor Keycard", {__researchkeycard_png, __researchkeycard_png_len}},
        {"Zone Manager Keycard", {__zonemanagerkeycard_png, __zonemanagerkeycard_png_len}},
        {"Guard Keycard", {__guardkeycard_png, __guardkeycard_png_len}},
        {"MTF Private Keycard", {privatekeycard_png, privatekeycard_png_len}},
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
        //{"SCP-1344", {__scp1344_png, __scp1344_png_len}}, no icon yet
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
                wxLogWarning("Failed to apply '%s' at: %s", ROBOTO_BOLD.c_str(), item.name.c_str());
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
                wxLogWarning("Failed to load icon to: %s", item.name.c_str());
            }
        }

        if (iconBitmap.IsOk()) {
            wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, iconBitmap);
            headerSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL);
        }

        itemSizer->Add(headerSizer, 0, wxEXPAND | wxBOTTOM, 10);

        wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
        nameLabel->SetForegroundColour(textColor);

        wxFont nameLabelFont;
        if (fontLoaded) {
            nameLabelFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (!nameLabelFont.IsOk()) {
                wxLogWarning("Falha ao aplicar '%s' a: Nome.", ROBOTO_BOLD.c_str());
                nameLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            }
        }
        else {
            nameLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        }
        nameLabel->SetFont(nameLabelFont);

        nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        MyRichTextCtrlTabFour* nameField = new MyRichTextCtrlTabFour(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 50), wxRE_MULTILINE | wxBORDER_SUNKEN);
        nameField->SetBackgroundColour(textCtrlBg);
        nameField->SetForegroundColour(textCtrlFg);

        if (fontLoaded) {
            wxFont customFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                nameField->SetFont(customFont);
            }
            else {
                nameField->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        }
        else {
            nameField->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        nameSizer->Add(nameField, 1, wxEXPAND | wxRIGHT, 5);

        wxButton* nameColorButton = new wxButton(this, wxID_ANY, "Color");
        nameColorButton->SetBackgroundColour(buttonColor);
        nameColorButton->SetForegroundColour(textColor);
        nameSizer->Add(nameColorButton, 0, wxALIGN_CENTER_VERTICAL);

        itemSizer->Add(nameSizer, 0, wxEXPAND | wxBOTTOM, 10);

        nameColorButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
            wxColourData data;
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK) {
                wxColour color = dialog.GetColourData().GetColour();
                if (color.IsOk()) {
                    long start, end;
                    nameField->GetSelection(&start, &end);
                    if (start != end) {
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        nameField->SetStyle(start, end, attr);
                    }
                    else {
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        nameField->SetDefaultStyle(attr);
                    }
                }
            }
        });

        MyRichTextCtrlTabFour* descriptionField = nullptr;
        wxButton* descColorButton = nullptr;

        if (!item.description.empty()) {
            wxBoxSizer* descSizer = new wxBoxSizer(wxHORIZONTAL);

            wxStaticText* descLabel = new wxStaticText(this, wxID_ANY, "Description:");
            descLabel->SetForegroundColour(textColor);

            wxFont descLabelFont;
            if (fontLoaded) {
                descLabelFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
                if (!descLabelFont.IsOk()) {
                    wxLogWarning("Failed to apply desc '%s' to: Description.", ROBOTO_BOLD.c_str());
                    descLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
                }
            }
            else {
                descLabelFont = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            }
            descLabel->SetFont(descLabelFont);

            descSizer->Add(descLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);


            descriptionField = new MyRichTextCtrlTabFour(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 50), wxRE_MULTILINE | wxBORDER_SUNKEN);
            descriptionField->SetBackgroundColour(textCtrlBg);
            descriptionField->SetForegroundColour(textCtrlFg);

            if (fontLoaded) {
                wxFont customFont = GetCustomFont(ROBOTO_BOLD, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
                if (customFont.IsOk()) {
                    descriptionField->SetFont(customFont);
                }
                else {
                    descriptionField->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
                }
            }
            else {
                descriptionField->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }

            descSizer->Add(descriptionField, 1, wxEXPAND | wxRIGHT, 5);

            descColorButton = new wxButton(this, wxID_ANY, "Color");
            descColorButton->SetBackgroundColour(buttonColor);
            descColorButton->SetForegroundColour(textColor);
            descSizer->Add(descColorButton, 0, wxALIGN_CENTER_VERTICAL);

            itemSizer->Add(descSizer, 0, wxEXPAND | wxBOTTOM, 10);

            descColorButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
                wxColourData data;
                wxColourDialog dialog(this, &data);
                if (dialog.ShowModal() == wxID_OK) {
                    wxColour color = dialog.GetColourData().GetColour();
                    if (color.IsOk()) {
                        long start, end;
                        descriptionField->GetSelection(&start, &end);
                        if (start != end) {
                            wxRichTextAttr attr;
                            attr.SetTextColour(color);
                            descriptionField->SetStyle(start, end, attr);
                        }
                        else {
                            wxRichTextAttr attr;
                            attr.SetTextColour(color);
                            descriptionField->SetDefaultStyle(attr);
                        }
                    }
                }
            });
        }

        wxStaticLine* separator = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        separator->SetForegroundColour(textColor);
        separator->SetBackgroundColour(textColor);
        itemSizer->Add(separator, 0, wxEXPAND | wxTOP, 5);

        entriesSizer->Add(itemSizer, 0, wxEXPAND | wxALL, 10);

        TabFourEntryControls controls;
        controls.nameField = nameField;
        controls.nameColorButton = nameColorButton;
        controls.descriptionField = descriptionField;
        controls.descriptionColorButton = descColorButton;
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
    std::stringstream output;

    for (size_t it = 0; it < items.size(); it++) {
        const auto& item = items[it];
        const auto& controls = entries[it];

        std::string nameText;
        if (controls.nameField && !controls.nameField->GetValue().IsEmpty()) {
            nameText = GetFormattedText(controls.nameField);
        } else {
            nameText = EscapeXML(item.name);
        }

        std::string descText;
        if (controls.descriptionField && !controls.descriptionField->GetValue().IsEmpty()) {
            descText = GetFormattedText(controls.descriptionField);
        } else if (!item.description.empty()) {
            descText = EscapeXML(item.description);
        }

        if (item.description.empty()) {
            output << item.id << "~" << nameText << "\n";
        } else {
            output << item.id << "~" << nameText << "~" << descText << "\n";
        }
    }

    wxString outputStr = wxString::FromUTF8(output.str().c_str());

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
    if (!outputStream.IsOk()) {
        wxMessageBox("Failed to save at:\n" + filePath, "Erro", wxOK | wxICON_ERROR);
        return;
    }

    wxTextOutputStream textStream(outputStream, wxEOL_NATIVE, wxConvUTF8);
    textStream.WriteString(outputStr);

    wxMessageBox("File saved at:\n" + filePath, "Sucesso", wxOK | wxICON_INFORMATION);
}
