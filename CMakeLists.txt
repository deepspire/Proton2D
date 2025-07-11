cmake_minimum_required(VERSION 3.22)

project(Proton2D)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        src/proton/audio.cc
        src/proton/proton.cc
        src/proton/physics.cc
        src/proton/resourcemanager.cc
        src/proton/scene.cc
        src/jni/JNIBindings.cpp

        src/proton/shapes/shape.cc
        src/proton/shapes/rectangle.cc
        src/proton/shapes/line.cc
        src/proton/shapes/circle.cc
        src/proton/shapes/text.cc
        src/proton/shapes/textbox.cc
        src/proton/shapes/rectanglebutton.cc
        src/proton/shapes/container.cc
        src/proton/shapes/buttonarea.cc
        src/proton/shapes/group.cc
        src/proton/shapes/image.cc
        src/proton/shapes/imagebutton.cc
        src/proton/shapes/progressbar.cc
)

if(ANDROID)
    add_library(main SHARED ${SOURCE_FILES})
    set(TARGET_NAME main)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
else()
    add_library(Proton2D STATIC ${SOURCE_FILES})
    set(TARGET_NAME Proton2D)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
endif()



add_subdirectory(ext/SDL3)
set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF support")
set(SDLIMAGE_GIF OFF CACHE BOOL "Disable GIF support")
set(SDLIMAGE_JXL OFF CACHE BOOL "Disable JXL support")
set(SDLIMAGE_LBM OFF CACHE BOOL "Disable LBM support")
set(SDLIMAGE_PCX OFF CACHE BOOL "Disable PCX support")
set(SDLIMAGE_PNM OFF CACHE BOOL "Disable PNM support")
set(SDLIMAGE_QOI OFF CACHE BOOL "Disable QOI support")
set(SDLIMAGE_SVG OFF CACHE BOOL "Disable SVG support")
set(SDLIMAGE_TIF OFF CACHE BOOL "Disable TIFF support")
set(SDLIMAGE_WEBP OFF CACHE BOOL "Disable WEBP support")
set(SDLIMAGE_XCF OFF CACHE BOOL "Disable XCF support")
set(SDLIMAGE_XPM OFF CACHE BOOL "Disable XPM support")
set(SDLIMAGE_XV OFF CACHE BOOL "Disable XV support")
set(SDLIMAGE_BMP OFF CACHE BOOL "Disable BMP support")
set(SDLIMAGE_TGA OFF CACHE BOOL "Disable TGA support")
add_subdirectory(ext/SDL3_image)
add_subdirectory(ext/SDL3_ttf)
add_subdirectory(ext/box2d)

# Далее использовать её
target_include_directories(${TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ext
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image box2d::box2d)

if(WIN32)
    target_link_options(${TARGET_NAME} PRIVATE -static-libgcc -static-libstdc++)
    # The custom commands for copying assets and DLLs are typically for executables.
    # If you are building a static library, these commands might not be necessary
    # unless you have a separate executable project that links against this library
    # and needs these assets/DLLs copied.
    # add_custom_command(TARGET ${PRO_TARGET} POST_BUILD
    #     COMMAND_EXPAND_LISTS

    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #     "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    #     "$<TARGET_FILE_DIR:${PRO_TARGET}>/assets"
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #     $<TARGET_RUNTIME_DLLS:${PRO_TARGET}>
    #     $<TARGET_FILE_DIR:${PRO_TARGET}>
    # )
elseif(ANDROID)
    target_link_libraries(${TARGET_NAME} PRIVATE log android)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${TARGET_NAME} PRIVATE m dl)
    # Similar to Windows, these custom commands might not be needed for a static library.
    # add_custom_command(TARGET ${PRO_TARGET} POST_BUILD
    #     COMMAND_EXPAND_LISTS

    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #     "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    #     "$<TARGET_FILE_DIR:${PRO_TARGET}>/assets"
    # )

    # set_target_properties(${PRO_TARGET} PROPERTIES
    #     INSTALL_RPATH "$ORIGIN"
    #     BUILD_WITH_INSTALL_RPATH ON
    # )
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