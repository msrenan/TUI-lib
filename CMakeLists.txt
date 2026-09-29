cmake_minimum_required(VERSION 3.14)
project(TUI-lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Biblioteca estática.
add_library(tui STATIC
    src/Window.cpp
    src/Text.cpp
    src/Shape.cpp
    src/Title.cpp
    src/Utf8.cpp
)
target_include_directories(tui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(tui PRIVATE -Wall -Wextra -Wpedantic)

# Exemplos.
foreach(example 01_hello 02_bounce 03_input)
    add_executable(${example} examples/${example}.cpp)
    target_link_libraries(${example} PRIVATE tui)
endforeach()

# Testes.
enable_testing()
add_executable(test_tui tests/test_tui.cpp)
target_link_libraries(test_tui PRIVATE tui)
add_test(NAME test_tui COMMAND test_tui)
