
cmake_minimum_required(VERSION 3.25)
project(matnes)


add_subdirectory(
    lib/matgui
    )


add_executable(
    matnes
    src/main.cpp
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


target_compile_features(
    matnes
    PRIVATE
    cxx_std_20
    )
