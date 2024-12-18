// CustomFontLoader.cpp
#include "CustomFontLoader.h"
#include "fonts/OliversBarney.h"
#include "fonts/RobotoBold.h"

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

    fontNameMap["Roboto-Bold"] = "Roboto Bold";
    fontNameMap["OliversBarney-Regular"] = "OliversBarney-Regular";

    g_fontsLoaded = false; //false by default because linux has skill issue with fonts
    return g_fontsLoaded;
#endif
}

wxFont GetCustomFont(const std::string& fontName, int pointSize, wxFontFamily family, wxFontStyle style, wxFontWeight weight) {
    if (!g_fontsLoaded) {
        wxLogWarning("custom fonts didnt load, calling LoadCustomFonts().");
        LoadCustomFonts();
    }

    if (fontNameMap.find(fontName) == fontNameMap.end()) {
        wxLogWarning("Font '%s' not found, using default", fontName.c_str());
        return wxFont(pointSize, family, style, weight);
    }

    wxFont customFont(pointSize, family, style, weight, false, wxString::FromUTF8(fontNameMap[fontName].c_str()));

    if (!customFont.IsOk()) {
        wxLogWarning("couldnt create custom font '%s'. using default font.", fontName.c_str());
        return wxFont(pointSize, family, style, weight);
    }

    return customFont;
}