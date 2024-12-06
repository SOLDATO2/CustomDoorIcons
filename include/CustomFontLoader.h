// CustomFontLoader.h
#ifndef CUSTOMFONTLOADER_H
#define CUSTOMFONTLOADER_H

#include <wx/wx.h>
#include <string>

bool LoadCustomFonts();

wxFont GetCustomFont(const std::string& fontName, int pointSize, 
                    wxFontFamily family = wxFONTFAMILY_DEFAULT, 
                    wxFontStyle style = wxFONTSTYLE_NORMAL, 
                    wxFontWeight weight = wxFONTWEIGHT_NORMAL);

#endif // CUSTOMFONTLOADER_H
