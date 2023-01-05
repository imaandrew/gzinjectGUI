cmake_minimum_required(VERSION 3.5)
project(gzinjectGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

execute_process(
        COMMAND git describe --always
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_compile_definitions(GIT_VERSION=${GIT_VERSION})

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(gzinjectGUI WIN32 main.cpp gzinjectGUI.cpp commandOutput.cpp gzguiwidget.cpp preset.cpp)
target_link_libraries(gzinjectGUI
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
