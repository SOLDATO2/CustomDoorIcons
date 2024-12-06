// CustomFontLoader.cpp
#include "CustomFontLoader.h"
#include "OliversBarney.h"
#include "RobotoBold.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <map>

struct FontInfo {
    std::string name;
    unsigned char* data;
    unsigned int length;
};

static bool g_fontsLoaded = false;
static std::map<std::string, std::string> fontNameMap;

bool LoadCustomFonts() {
#ifdef _WIN32
    if (g_fontsLoaded) return true; // avoid multiple calls

    // fonts to load
    FontInfo fonts[] = {
        {"OliversBarney-Regular", OliversBarney_otf, OliversBarney_otf_len},
        {"Roboto Bold", Roboto_Bold_ttf, Roboto_Bold_ttf_len}
    };

    for (const auto& font : fonts) {
        DWORD numFontsAdded;
        HANDLE fontHandle = AddFontMemResourceEx((void*)font.data, font.length, NULL, &numFontsAdded);
        if (fontHandle) {
            //wxLogMessage("Fonte personalizada '%s' carregada com sucesso. Número de fontes adicionadas: %u", 
            //            font.name.c_str(), numFontsAdded);
            fontNameMap[font.name] = font.name;
        } else {
            DWORD error = GetLastError();
            wxLogError("Failed to load font '%s'. Error: %lu. Report it to the developers if u want to.", 
                      font.name.c_str(), error);
        }
    }

    g_fontsLoaded = true;
    return g_fontsLoaded;
#else
    // linux isnt supported rn so we just return this message
    wxLogWarning("Fonts aren't supported in linux yet. Using default fonts.");
    return false;
#endif
}

wxFont GetCustomFont(const std::string& fontName, int pointSize, wxFontFamily family, wxFontStyle style, wxFontWeight weight) {
    if (!g_fontsLoaded) {
        wxLogWarning("Fonts weren't loaded, callling LoadCustomFonts().");
        LoadCustomFonts();
    }

    if (fontNameMap.find(fontName) == fontNameMap.end()) {
        wxLogWarning("Couldn't find font '%s'. Using default.", fontName.c_str());
        return wxFont(pointSize, family, style, weight);
    }

    wxFont customFont(pointSize, family, style, weight, false, wxString::FromUTF8(fontName.c_str()));

    if (!customFont.IsOk()) {
        wxLogWarning("Failed to load font '%s'. Using default.", fontName.c_str());
        return wxFont(pointSize, family, style, weight);
    }

    return customFont;
}
