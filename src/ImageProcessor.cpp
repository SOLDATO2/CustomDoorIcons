#include "ImageProcessor.h"
#include <wx/wx.h>
#include <wx/image.h>
#include <iostream>

// Implementação da função para processar a imagem e gerar HTML usando wxImage
wxString imageToProcessedHTML(const std::string& imagePath, int width) {
    wxImage img;
    if (!img.LoadFile(wxString::FromUTF8(imagePath.c_str()))) {
        std::cerr << "Error, could not load image: " << imagePath << std::endl;
        return "";
    }

    // Calcular o novo tamanho mantendo a proporção
    double aspectRatio = static_cast<double>(img.GetHeight()) / img.GetWidth();
    int newHeight = static_cast<int>(aspectRatio * width * 0.55);
    img.Rescale(width, newHeight, wxIMAGE_QUALITY_HIGH);

    // Iniciar a string HTML com <size=2>
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

            // Verificar se a cor atual é diferente da anterior para agrupar caracteres
            if (currentColor != hexColorStr) {
                if (!currentColor.IsEmpty()) {
                    htmlOutput += "</color>";
                }
                currentColor = hexColorStr;
                htmlOutput += "<color=" + currentColor + ">";
            }

            htmlOutput += L"\u2588";  // Usar o caractere Unicode para '█'
        }
        if (!currentColor.IsEmpty()) {
            htmlOutput += "</color>";
        }

        // Adicionar \n no final de cada linha, exceto na última
        if (y != img.GetHeight() - 1) {
            htmlOutput += "\\n";
        }
    }

    // Adicionar </size> no final da última linha
    htmlOutput += "</size>";

    return htmlOutput;
}
