#include "TabThree.h"
#include <wx/filedlg.h>
#include <wx/file.h>

TabThree::TabThree(wxNotebook* parent) : wxPanel(parent, wxID_ANY) {
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
        entrySizer->Add(label, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        wxBoxSizer* fieldsSizer = new wxBoxSizer(wxHORIZONTAL);

        wxTextCtrl* textField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
        fieldsSizer->Add(textField, 1, wxEXPAND | wxRIGHT, 5);

        if(labels[i] == "TRANSMITTING...\nTIME LEFT: {TIME_LEFT}" || labels[i] == "RESTARTING\n{TIME_LEFT}") {
            wxCheckBox* isTextCheckBox = new wxCheckBox(this, wxID_ANY, "Add timer");
            fieldsSizer->Add(isTextCheckBox, 0, wxALIGN_CENTER_VERTICAL);

            entrySizer->Add(fieldsSizer, 0, wxEXPAND | wxALL, 5);

            TabThreeEntryControls controls = { textField, isTextCheckBox };
            entries.push_back(controls);

            entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 5);
        }else{

            entrySizer->Add(fieldsSizer, 0, wxEXPAND | wxALL, 5);

            TabThreeEntryControls controls = { textField, nullptr };
            entries.push_back(controls);

            entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 5);

        }

    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 5);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);


    saveButton->Bind(wxEVT_BUTTON, &TabThree::OnSaveButtonClicked, this);

    SetSizer(mainSizer);
    Layout(); // Aplica o layout imediatamente
}

void TabThree::OnSaveButtonClicked(wxCommandEvent& event) {
    wxString output;

    for(size_t i = 0; i < labels.size(); i++) {
        wxString text = entries[i].textField->GetValue();
        if(text.IsEmpty()) {
            text = wxString::FromUTF8(labels[i].c_str());
            text.Replace("\n", "\\n");
        }else{
            text.Replace("\r\n", "\\n"); // Windows
            text.Replace("\n", "\\n");   // Unix/Linux/Mac
        }
        text.Replace("{TIME_LEFT}", "{0}");

        if(entries[i].isTextCheckBox && entries[i].isTextCheckBox->IsChecked()) {
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

    wxFile file;
    if (file.Open(filePath, wxFile::write)) {
        file.Write(output);
        file.Close();
        wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
    }

    
}
