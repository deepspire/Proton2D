cmake_minimum_required(VERSION 3.16)

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
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

add_subdirectory(ext/SDL3)
set(SDLIMAGE_AVIF    OFF CACHE BOOL "Disable AVIF support")
set(SDLIMAGE_GIF     OFF CACHE BOOL "Disable GIF support" )  # !!!
set(SDLIMAGE_JXL     OFF CACHE BOOL "Disable JXL support" )
set(SDLIMAGE_LBM     OFF CACHE BOOL "Disable LBM support" )
set(SDLIMAGE_PCX     OFF CACHE BOOL "Disable PCX support" )
set(SDLIMAGE_PNM     OFF CACHE BOOL "Disable PNM support" )
set(SDLIMAGE_QOI     OFF CACHE BOOL "Disable QOI support" )
set(SDLIMAGE_SVG     OFF CACHE BOOL "Disable SVG support" )
set(SDLIMAGE_TIF     OFF CACHE BOOL "Disable TIFF support")
set(SDLIMAGE_WEBP    OFF CACHE BOOL "Disable WEBP support")  # !!!
set(SDLIMAGE_XCF     OFF CACHE BOOL "Disable XCF support" )
set(SDLIMAGE_XPM     OFF CACHE BOOL "Disable XPM support" )
set(SDLIMAGE_XV      OFF CACHE BOOL "Disable XV support"  )
set(SDLIMAGE_BMP     OFF CACHE BOOL "Disable BMP support" )
set(SDLIMAGE_TGA     OFF CACHE BOOL "Disable TGA support" )
add_subdirectory(ext/SDL3_image)
add_subdirectory(ext/SDL3_ttf)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(Proton2D ${SOURCE_FILES})
target_include_directories(Proton2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext) # ext/miniaudio.h !!!
target_link_libraries(Proton2D PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

if(WIN32)
    target_link_options(Proton2D PRIVATE -static-libgcc -static-libstdc++)
    # target_link_options(Proton2D PRIVATE -mwindows) # чтобы не было консольки!!!!!!!!!!


    # наверное надо потом добавить
    #COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #"${MINGW_BIN_DIR}/libwinpthread-1.dll"
    #$<TARGET_FILE_DIR:Proton2D>

    add_custom_command(TARGET Proton2D POST_BUILD
        COMMAND_EXPAND_LISTS

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Proton2D>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:Proton2D>
        $<TARGET_FILE_DIR:Proton2D>
    )

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
