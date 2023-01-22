cmake_minimum_required(VERSION 3.16)

project(gzinjectGUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

execute_process(
        COMMAND git describe --always
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_compile_definitions(GIT_VERSION=${GIT_VERSION})

add_executable(gzinjectGUI
        main.cpp
        gzinjectGUI.cpp
        commandOutput.cpp
        gzguiwidget.cpp
	preset.cpp
)

target_link_libraries(gzinjectGUI PRIVATE Qt6::Widgets)

set_target_properties(gzinjectGUI PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

if(APPLE)
add_custom_command(TARGET gzinjectGUI
      POST_BUILD COMMAND
      ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/"
      $<TARGET_FILE:gzinjectGUI>)
endif()

install(TARGETS gzinjectGUI
  RUNTIME DESTINATION bin BUNDLE DESTINATION bin)
