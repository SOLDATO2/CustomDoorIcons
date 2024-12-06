#include "TabFour.h"
#include <wx/file.h>

// Construtor da classe TabFour
TabFour::TabFour(wxNotebook* parent) : wxScrolledWindow(parent, wxID_ANY) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Inicialização do vetor items com IDs específicos
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

    // Criação dos controles de entrada para cada item
    for (const auto& item : items) {
        wxBoxSizer* entrySizer = new wxBoxSizer(wxVERTICAL);

        // Rótulo com o nome do item
        wxString label = wxString::Format("%s\nName:", item.name);
        wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, label);
        entrySizer->Add(nameLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

        // Campo de texto para o nome
        wxTextCtrl* nameField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
        entrySizer->Add(nameField, 0, wxEXPAND | wxBOTTOM, 10);

        wxTextCtrl* descriptionField = nullptr; // Inicialmente nullptr

        // Verifica se o item possui descrição
        if (!item.description.empty()) {
            // Rótulo para a descrição
            wxStaticText* descriptionLabel = new wxStaticText(this, wxID_ANY, "Description:");
            entrySizer->Add(descriptionLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

            // Campo de texto para a descrição
            descriptionField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
            entrySizer->Add(descriptionField, 0, wxEXPAND | wxBOTTOM, 10);
        }

        // Armazena os controles de entrada
        TabFourEntryControls controls = { nameField, descriptionField };
        entries.push_back(controls);

        entriesSizer->Add(entrySizer, 0, wxEXPAND | wxALL, 10);
    }

    mainSizer->Add(entriesSizer, 1, wxEXPAND | wxALL, 10);

    // Botão de salvar
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save");
    mainSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 10);

    // Vincula o evento do botão de salvar
    saveButton->Bind(wxEVT_BUTTON, &TabFour::OnSaveButtonClicked, this);

    SetSizerAndFit(mainSizer);

    // Habilitar scroll
    SetScrollRate(10, 10);
}

void TabFour::OnSaveButtonClicked(wxCommandEvent& event){
    wxString output;
    for (size_t it = 0; it < items.size(); ++it){
        const auto& item = items[it];
        const auto& controls = entries[it];

        // Obtém o valor do campo de nome
        wxString nameText = controls.nameField->GetValue();
        if (nameText.IsEmpty()) {
            nameText = wxString::FromUTF8(item.name.c_str());
        }

        // Inicia a linha com ID e nome
        output += wxString::Format("%d~%s", item.id, nameText);

        // Se o item possui descrição, adiciona a descrição
        if (item.description.empty()) {
            // Não há descrição
            output += "\n";
        } else {
            wxString descText = controls.descriptionField->GetValue();
            if (descText.IsEmpty()) {
                descText = wxString::FromUTF8(item.description.c_str());
            }
            output += "~" + descText + "\n";
        }
    }

    // Diálogo para salvar o arquivo
    wxFileDialog saveFileDialog(
        this,
        _("Save Items.txt file"),
        "",
        "Items.txt",
        "Text files (*.txt)|*.txt",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return; // Usuário cancelou a operação
    }

    wxString filePath = saveFileDialog.GetPath();

    wxFile file;
    if (file.Open(filePath, wxFile::write)){
        file.Write(output);
        file.Close();
        wxMessageBox("File saved successfully at:\n" + filePath, "Success", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save the file at:\n" + filePath, "Error", wxOK | wxICON_ERROR);
    }
}
