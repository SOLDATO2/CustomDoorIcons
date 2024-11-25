// src/ImageProcessor.cpp
#include "ImageProcessor.h"
#include <opencv2/opencv.hpp>
#include <iostream>

// Implementação da função para processar a imagem e gerar HTML
wxString imageToProcessedHTML(const std::string& imagePath, int width) {
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cerr << "Error, could not load image: " << imagePath << std::endl;
        return "";
    }

    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);  // Converter para RGB

    // Calcular o novo tamanho mantendo a proporção
    double aspectRatio = static_cast<double>(img.rows) / img.cols;
    int newHeight = static_cast<int>(aspectRatio * width * 0.55);
    cv::resize(img, img, cv::Size(width, newHeight));

    // Iniciar a string HTML com <size=2>
    wxString htmlOutput = "<size=2>";

    for (int y = 0; y < img.rows; y++) {
        wxString currentColor = "";
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
            int r = pixel[0], g = pixel[1], b = pixel[2];
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
        if (y != img.rows - 1) {
            htmlOutput += "\\n";
        }
    }

    // Adicionar </size> no final da última linha
    htmlOutput += "</size>";

    return htmlOutput;
}
