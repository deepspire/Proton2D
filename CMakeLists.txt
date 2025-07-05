cmake_minimum_required(VERSION 3.22)

project(Proton2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cc
    src/menu.cc
    src/ss.cc

    src/proton/audio.cc
    src/proton/proton.cc
    src/proton/resourcemanager.cc
    src/jni/JNIBindings.cpp
)

if(ANDROID)
    add_library(main SHARED ${SOURCE_FILES})
    set(SDL_MAIN_TARGET main)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
else()
    add_executable(Proton2D ${SOURCE_FILES})
    set(SDL_MAIN_TARGET Proton2D)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
endif()

add_subdirectory(ext/SDL3)
set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF support")
set(SDLIMAGE_GIF OFF CACHE BOOL "Disable GIF support") # !!!
set(SDLIMAGE_JXL OFF CACHE BOOL "Disable JXL support")
set(SDLIMAGE_LBM OFF CACHE BOOL "Disable LBM support")
set(SDLIMAGE_PCX OFF CACHE BOOL "Disable PCX support")
set(SDLIMAGE_PNM OFF CACHE BOOL "Disable PNM support")
set(SDLIMAGE_QOI OFF CACHE BOOL "Disable QOI support")
set(SDLIMAGE_SVG OFF CACHE BOOL "Disable SVG support")
set(SDLIMAGE_TIF OFF CACHE BOOL "Disable TIFF support")
set(SDLIMAGE_WEBP OFF CACHE BOOL "Disable WEBP support") # !!!
set(SDLIMAGE_XCF OFF CACHE BOOL "Disable XCF support")
set(SDLIMAGE_XPM OFF CACHE BOOL "Disable XPM support")
set(SDLIMAGE_XV OFF CACHE BOOL "Disable XV support")
set(SDLIMAGE_BMP OFF CACHE BOOL "Disable BMP support")
set(SDLIMAGE_TGA OFF CACHE BOOL "Disable TGA support")
add_subdirectory(ext/SDL3_image)
add_subdirectory(ext/SDL3_ttf)

target_include_directories(${SDL_MAIN_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext) # ext/miniaudio.h !!!
target_link_libraries(${SDL_MAIN_TARGET} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

if(WIN32)
    target_link_options(Proton2D PRIVATE -static-libgcc -static-libstdc++)
    # target_link_options(Proton2D PRIVATE -mwindows) # чтобы не было консольки!!!!!!!!!!

    add_custom_command(TARGET Proton2D POST_BUILD
        COMMAND_EXPAND_LISTS

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Proton2D>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:Proton2D>
        $<TARGET_FILE_DIR:Proton2D>
    )
elseif(ANDROID)
    target_link_libraries(${SDL_MAIN_TARGET} PRIVATE log android)
elseif(UNIX AND NOT APPLE)
    add_custom_command(TARGET Proton2D POST_BUILD
        COMMAND_EXPAND_LISTS

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Proton2D>/assets"
    )

    set_target_properties(Proton2D PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH ON
    )
else()
    message(FATAL_ERROR "platform pizdec")
endif()

find_package(Java)

if(Java_FOUND)
    if(WIN32)
        set(GRADLEW_EXECUTABLE gradlew.bat)
    else()
        set(GRADLEW_EXECUTABLE ./gradlew)
    endif()

    add_custom_target(
        "Build-android-Release"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/android_build
        COMMAND ${CMAKE_COMMAND} -E env
        "JAVA_HOME=$ENV{JAVA_HOME}"
        ${GRADLEW_EXECUTABLE} assembleRelease
        USES_TERMINAL
    )

    add_custom_target(
        "Install-android-Release"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/android_build
        COMMAND ${CMAKE_COMMAND} -E env
        "JAVA_HOME=$ENV{JAVA_HOME}"
        ${GRADLEW_EXECUTABLE} installRelease
        USES_TERMINAL
    )

    add_custom_target(
        "Clean-android"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/android_build
        COMMAND ${CMAKE_COMMAND} -E env
        "JAVA_HOME=$ENV{JAVA_HOME}"
        ${GRADLEW_EXECUTABLE} clean
        USES_TERMINAL
    )
else()
    message(STATUS "Couldn't find java.")
endif()
