cmake_minimum_required(VERSION 3.16)

project(fpInject VERSION 1.0.0 LANGUAGES CXX)

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

if (APPLE)
set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/res/icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")
elseif(WIN32)
set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/res/icon.rc")
endif()

qt_add_executable(fpInject
        main.cpp
        fpInject.cpp
        commandOutput.cpp
        fpguiwidget.cpp
	preset.cpp
	${app_icon}
)

target_link_libraries(fpInject PRIVATE Qt6::Widgets)

set_target_properties(fpInject PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

if(APPLE)
add_custom_command(TARGET fpInject
      POST_BUILD COMMAND
      ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/"
      $<TARGET_FILE:fpInject>)
endif()

install(TARGETS fpInject
  RUNTIME DESTINATION bin BUNDLE DESTINATION bin)
