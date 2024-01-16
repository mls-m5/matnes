
cmake_minimum_required(VERSION 3.25)
project(matnes)

add_subdirectory(
    lib/matgui
    )

add_executable(
    matnes
    src/main.cpp
    src/ppu.cpp
    src/ppuview.cpp
    src/cpu.cpp
    src/apu.cpp
    )

target_link_libraries(
    matnes
    PUBLIC
    MatGui
    SDL2_image
    )

