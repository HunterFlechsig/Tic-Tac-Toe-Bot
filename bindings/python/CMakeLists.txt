cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(tictactoe LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(TICTACTOE_BUILD_CLI OFF)
    set(TICTACTOE_BUILD_TESTS OFF)

    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../engine
        ${CMAKE_CURRENT_BINARY_DIR}/engine
    )
endif()

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG QUIET)
if(NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v3.1.0
    )
    FetchContent_MakeAvailable(pybind11)
endif()

pybind11_add_module(tictactoe_py src/module.cpp)
target_link_libraries(tictactoe_py PRIVATE tictactoe_lib)
set_target_properties(tictactoe_py PROPERTIES OUTPUT_NAME tictactoe)

install(TARGETS tictactoe_py DESTINATION .)
