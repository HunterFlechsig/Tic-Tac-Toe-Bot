cmake_minimum_required(VERSION 3.14)
project(TicTacToeEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tictactoe_lib
    src/Board.cpp
    src/Game.cpp
    src/GameState.cpp
    src/Player.cpp
)

target_include_directories(tictactoe_lib PUBLIC include)
set_target_properties(tictactoe_lib PROPERTIES POSITION_INDEPENDENT_CODE ON)

option(TICTACTOE_BUILD_CLI "Build the tic-tac-toe executable" ON)
option(TICTACTOE_BUILD_TESTS "Build the C++ tests" ON)

if(TICTACTOE_BUILD_CLI)
    add_executable(tictactoe
        src/main.cpp
    )

    target_link_libraries(tictactoe PRIVATE tictactoe_lib)
endif()

if(TICTACTOE_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(tests
        tests/BoardTest.cpp
        tests/GameStateTest.cpp
        tests/PlayerTest.cpp
    )

    target_link_libraries(tests
        PRIVATE
        tictactoe_lib
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
