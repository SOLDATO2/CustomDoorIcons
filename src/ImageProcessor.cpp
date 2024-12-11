#include "ImageProcessor.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <iostream>

wxString imageToProcessedHTML(const std::string& imagePath, int width) {
    wxImage img;
    if (!img.LoadFile(wxString::FromUTF8(imagePath.c_str()))) {
        std::cerr << "Error, could not load image: " << imagePath << std::endl;
        return "";
    }

    double aspectRatio = static_cast<double>(img.GetHeight()) / img.GetWidth();
    int newHeight = static_cast<int>(aspectRatio * width * 0.55);
    img.Rescale(width, newHeight, wxIMAGE_QUALITY_HIGH);

    wxString htmlOutput = "<size=2>";

    for (int y = 0; y < img.GetHeight(); y++) {
        wxString currentColor = "";
        for (int x = 0; x < img.GetWidth(); x++) {
            unsigned char r = img.GetRed(x, y);
            unsigned char g = img.GetGreen(x, y);
            unsigned char b = img.GetBlue(x, y);

            char hexColor[8];
            snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", r, g, b);
            wxString hexColorStr(hexColor);

            if (currentColor != hexColorStr) {
                if (!currentColor.IsEmpty()) {
                    htmlOutput += "</color>";
                }
                currentColor = hexColorStr;
                htmlOutput += "<color=" + currentColor + ">";
            }

            htmlOutput += L"\u2588";
        }
        if (!currentColor.IsEmpty()) {
            htmlOutput += "</color>";
        }

        if (y != img.GetHeight() - 1) {
            htmlOutput += "\\n";
        }
    }

    htmlOutput += "</size>";

    return htmlOutput;
}
