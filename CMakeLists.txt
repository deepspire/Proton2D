cmake_minimum_required(VERSION 3.16)

project(Proton2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")

if(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "/usr/local/lib")
endif()

add_executable(Proton2D ${SOURCE_FILES})

add_custom_command(TARGET Proton2D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_CURRENT_BINARY_DIR}/assets"
)

if(WIN32)
    # TODO use PATH
    target_include_directories(Proton2D PRIVATE
        "C:/SDL3/x86_64-w64-mingw32/include"
        "C:/SDL3_ttf/x86_64-w64-mingw32/include"
        "C:/SDL3_image/x86_64-w64-mingw32/include"
    )

    target_link_directories(Proton2D PRIVATE
        "C:/SDL3/x86_64-w64-mingw32/lib"
        "C:/SDL3_ttf/x86_64-w64-mingw32/lib"
        "C:/SDL3_image/x86_64-w64-mingw32/lib"
    )

    target_link_libraries(Proton2D PRIVATE SDL3 SDL3_ttf SDL3_image)

    # target_link_options(Proton2D PRIVATE -mwindows) # чтобы не было консольки!!!!!!!!!!
elseif(UNIX AND NOT APPLE)
    find_package(SDL3 REQUIRED)
    find_package(SDL3_ttf REQUIRED)
    find_package(SDL3_image REQUIRED)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL3_gfx REQUIRED sdl3-gfx)

    target_link_libraries(Proton2D PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image ${SDL3_gfx_LIBRARIES})
    target_include_directories(Proton2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ext ${SDL3_gfx_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "platform pizdec")
endif()
