# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(CustomDoorIcons CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Encontrar wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base richtext)
if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets não encontrado!")
endif()

# Adicionar o executável
add_executable(${PROJECT_NAME} src/main.cpp src/TabOne.cpp src/TabTwo.cpp src/ImageProcessor.cpp src/TabThree.cpp src/TabFour.cpp src/CustomFontLoader.cpp src/OliversBarney.cpp src/RobotoBold.cpp)

# Adicionar diretórios de inclusão
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Definir propriedades específicas para MSVC (Visual Studio)
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# Linkar o executável com as dependências
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
