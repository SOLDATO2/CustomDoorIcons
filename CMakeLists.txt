# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(CustomDoorIcons CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base richtext)
if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "couldnht find wxwidgets")
endif()

set(SOURCES
    src/main.cpp
    src/TabOne.cpp
    src/TabTwo.cpp
    src/ImageProcessor.cpp
    src/TabThree.cpp
    src/TabFour.cpp
    src/CustomFontLoader.cpp
    src/OliversBarney.cpp
    src/RobotoBold.cpp
)

if(WIN32)
    list(APPEND SOURCES include/icons/appicon.rc)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (MSVC AND WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# Linkar o executável com as dependências do wxWidgets
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
