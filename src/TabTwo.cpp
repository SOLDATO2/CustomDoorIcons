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

const std::string OLIVERS_BARNEY_REGULAR = "OliversBarney-Regular";

std::string TabTwo::ColourToHex(const wxColour& color) {
    wxString hex = wxString::Format("#%02X%02X%02X", color.Red(), color.Green(), color.Blue());
    return std::string(hex.mb_str());
}

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

std::string TabTwo::GetFormattedText(wxRichTextCtrl* ctrl, const wxColour& defaultColor) {
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

        bool colorsDiffer = (currentColor.Red() != defaultColor.Red() ||
                             currentColor.Green() != defaultColor.Green() ||
                             currentColor.Blue() != defaultColor.Blue());

        if (colorsDiffer) {
            formattedText << "<color=" << ColourToHex(currentColor) << ">" << escapedText << "</color>";
        }
        else {
            formattedText << escapedText;
        }
    }

    return formattedText.str();
}

TabTwo::TabTwo(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    backgroundColor = wxColour(30, 30, 30);
    textColor = *wxWHITE;
    buttonColor = wxColour(70, 70, 70);
    textCtrlBg = wxColour(50, 50, 50);
    #ifdef _WIN32
    textCtrlFg = *wxBLACK;
    #elif __linux__
    textCtrlFg = wxColour(247, 247, 247);
    #endif
    

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

    entries.resize(labels.size(), TabTwoEntryControls{nullptr});

    wxBoxSizer* groupsSizer = new wxBoxSizer(wxVERTICAL);

    bool fontLoaded = LoadCustomFonts();

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
        {"Class-D Personnel", {classdicon_png, classdicon_png_len}},
        {"Scientist", {scientisticon_png, scientisticon_png_len}},
        {"Facility Guard", {guardicon_png, guardicon_png_len}},
        {"Chaos Insurgency Conscript", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Rifleman", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Marauder", {chaosicon_png, chaosicon_png_len}},
        {"Chaos Insurgency Repressor", {chaosicon_png, chaosicon_png_len}},
        {"Nine-Tailed Fox Specialist", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Sergeant", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Captain", {mtficon_png, mtficon_png_len}},
        {"Nine-Tailed Fox Private", {mtficon_png, mtficon_png_len}},
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
        {"Tutorial", {generic_png, generic_png_len}},
        {"Overwatch", {generic_png, generic_png_len}},
        {"Filmmaker", {generic_png, generic_png_len}},
        {"Spectator", {generic_png, generic_png_len}}
    };

    std::vector<std::string> group1;
    std::vector<std::string> group2;
    std::vector<std::string> group3;
    std::vector<std::string> group4;
    std::vector<std::string> group5;

    for (size_t i = 0; i < labels.size(); ++i) {
        const std::string& label = labels[i];

        if (label == "-") {
            group5.push_back(label);
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

    auto CreateEntryControls = [&](const std::string& labelStr, size_t originalIndex) -> wxBoxSizer* {
        if (labelStr == "-") return nullptr;

        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

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
        }

        if (iconBitmap.IsOk()) {
            wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, iconBitmap);
            headerSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
        }

        wxButton* colorButton = new wxButton(this, wxID_ANY, "Color");
        colorButton->SetBackgroundColour(buttonColor);
        colorButton->SetForegroundColour(textColor);
        headerSizer->Add(colorButton, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

        entrySizer->Add(headerSizer, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        MyRichTextCtrl* richTextCtrl = new MyRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 50), wxRE_MULTILINE | wxBORDER_SUNKEN);
        richTextCtrl->SetBackgroundColour(textCtrlBg);
        richTextCtrl->SetForegroundColour(textCtrlFg);

        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
            if (customFont.IsOk()) {
                richTextCtrl->SetFont(customFont);
            }
            else {
                richTextCtrl->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            }
        }
        else {
            richTextCtrl->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        }

        wxRichTextAttr defaultStyle;
        defaultStyle.SetTextColour(textCtrlFg);
        richTextCtrl->SetDefaultStyle(defaultStyle);

        richTextCtrl->SetMinSize(wxSize(-1, 50));

        entrySizer->Add(richTextCtrl, 0, wxEXPAND | wxBOTTOM, 10);

        colorButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
            wxColourData data;
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK) {
                wxColour color = dialog.GetColourData().GetColour();
                if (color.IsOk()) {
                    long start, end;
                    richTextCtrl->GetSelection(&start, &end);
                    if (start != end) {
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        richTextCtrl->SetStyle(start, end, attr);
                    }
                    else {
                        wxRichTextAttr attr;
                        attr.SetTextColour(color);
                        richTextCtrl->SetDefaultStyle(attr);
                    }
                }
            }
        });

        entries[originalIndex].richTextCtrl = richTextCtrl;

        return entrySizer;
    };

    auto AddGroupToSizer = [&](const std::vector<std::string>& group, wxBoxSizer* parentSizer, const std::string& groupTitle) {
        bool hasValidLabels = false;
        for (const auto& label : group) {
            if (label != "-") {
                hasValidLabels = true;
                break;
            }
        }
        if (!hasValidLabels) return;

        wxStaticText* title = new wxStaticText(this, wxID_ANY, groupTitle);
        title->SetForegroundColour(wxColour(255, 255, 255));
        if (fontLoaded) {
            wxFont customFont = GetCustomFont(OLIVERS_BARNEY_REGULAR, 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            if (customFont.IsOk()) {
                title->SetFont(customFont);
            }
            else {
                title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            }
        }
        else {
            title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        }

        parentSizer->Add(title, 0, wxALL, 5);

        wxBoxSizer* groupSizer = new wxBoxSizer(wxVERTICAL);
        for (const auto& labelStr : group) {
            if (labelStr == "-") continue;
            size_t originalIndex = 0;
            for (; originalIndex < labels.size(); ++originalIndex) {
                if (labels[originalIndex] == labelStr) {
                    break;
                }
            }

            if (originalIndex < labels.size()) {
                wxBoxSizer* entrySizer = CreateEntryControls(labelStr, originalIndex);
                if (entrySizer) {
                    groupSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 2);
                }
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
