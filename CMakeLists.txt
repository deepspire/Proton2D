cmake_minimum_required(VERSION 3.16)

project(Proton2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

if(WIN32)
    include(FetchContent)

    FetchContent_Declare(
            SDL3_TTF_FETCH
            URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-devel-3.2.2-mingw.tar.gz"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_Declare(
            SDL3_IMAGE_FETCH
            URL "https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-devel-3.2.4-mingw.tar.gz"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_Declare(
            SDL3_FETCH
            URL "https://github.com/libsdl-org/SDL/releases/download/release-3.2.2/SDL3-devel-3.2.2-mingw.tar.gz"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(SDL3_FETCH)

    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_fetch_SOURCE_DIR}/${SDL_ARCH_DIR}")

    FetchContent_MakeAvailable(SDL3_TTF_FETCH)
    FetchContent_MakeAvailable(SDL3_IMAGE_FETCH)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL_ARCH_DIR "x86_64-w64-mingw32")
    else()
        set(SDL_ARCH_DIR "i686-w64-mingw32") # ыыы нищий 32 бит
    endif()

    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_fetch_SOURCE_DIR}/${SDL_ARCH_DIR}")
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_image_fetch_SOURCE_DIR}/${SDL_ARCH_DIR}")
endif()

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(Proton2D ${SOURCE_FILES}
        src/proton/proton.cc
)
target_include_directories(Proton2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext)
target_link_libraries(Proton2D PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)

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
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
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
