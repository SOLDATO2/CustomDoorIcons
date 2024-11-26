// include/ImageProcessor.h
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <wx/string.h>

// Declaração da função para processar a imagem e gerar HTML
wxString imageToProcessedHTML(const std::string& imagePath, int width);

#endif // IMAGEPROCESSOR_H
