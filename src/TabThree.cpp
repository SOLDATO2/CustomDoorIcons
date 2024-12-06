// TabThree.cpp
#include "TabThree.h"
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <iostream>

TabThree::TabThree(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    wxColour backgroundColor(30, 30, 30);
    wxColour textColor(*wxWHITE);
    wxColour buttonColor(70, 70, 70);
    wxColour textCtrlBg(50, 50, 50);
    wxColour textCtrlFg(*wxWHITE);

    SetBackgroundColour(backgroundColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    labels = {
        "READY",
        "TRANSMITTING...\nTIME LEFT: {TIME_LEFT}",
        "TRANSMITTING...\nBYPASS MODE",
        "RESTARTING\n{TIME_LEFT}",
        "ADMIN IS USING\nTHE INTERCOM NOW",
        "YOU ARE MUTED BY ADMIN",
        "UNKNOWN",
        "PLEASE WAIT..."
    };

    wxBoxSizer* entriesSizer = new wxBoxSizer(wxVERTICAL);

    for (size_t i = 0; i < labels.size(); i++) {
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label = new wxStaticText(this, wxID_ANY, labels[i]);
        label->SetForegroundColour(textColor);
        entrySizer->Add(label, 0, wxALIGN_LEFT | wxALL, 5);

        wxBoxSizer* fieldsSizer = new wxBoxSizer(wxHORIZONTAL);

        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_PROCESS_ENTER);
        textField->SetBackgroundColour(textCtrlBg);
        textField->SetForegroundColour(textCtrlFg);
        fieldsSizer->Add(textField, 1, wxEXPAND | wxRIGHT, 5);

        TabThreeEntryControls controls = { textField, nullptr };

        if (labels[i] == "TRANSMITTING...\nTIME LEFT: {TIME_LEFT}" || labels[i] == "RESTARTING\n{TIME_LEFT}") {
            wxCheckBox* addTimerCheckBox = new wxCheckBox(this, wxID_ANY, "Add timer");
            addTimerCheckBox->SetForegroundColour(textColor);
            fieldsSizer->Add(addTimerCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
            controls.addTimerCheckBox = addTimerCheckBox;
        }

        entries.push_back(controls);

        entrySizer->Add(fieldsSizer, 0, wxEXPAND | wxALL, 5);

        entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 5);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    saveButton->SetBackgroundColour(buttonColor);
    saveButton->SetForegroundColour(textColor);
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    Layout();

    saveButton->Bind(wxEVT_BUTTON, &TabThree::OnSaveButtonClicked, this);

    SetScrollRate(10, 10);
}

void TabThree::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for (size_t i = 0; i < labels.size(); i++) {
        wxString text = entries[i].textField->GetValue();
        if (text.IsEmpty()) {
            text = wxString::FromUTF8(labels[i].c_str());
            text.Replace("\n", "\\n");
        }
        else {
            text.Replace("\r\n", "\\n"); // Windows
            text.Replace("\n", "\\n");   // Unix/Linux/Mac
        }
        text.Replace("{TIME_LEFT}", "{0}");

        if (entries[i].addTimerCheckBox && entries[i].addTimerCheckBox->IsChecked()) {
            text += " {0}";
        }

        output += text + "\n";
    }

    wxFileDialog saveFileDialog(
        this,
        _("Save Intercom.txt file"),
        "",
        "Intercom.txt",
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
