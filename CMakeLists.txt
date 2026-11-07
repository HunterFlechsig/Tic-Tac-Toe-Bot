cmake_minimum_required(VERSION 3.15)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT Python_EXECUTABLE)
    set(_backend_python "${CMAKE_CURRENT_SOURCE_DIR}/backend/.venv/bin/python")
    if(EXISTS "${_backend_python}")
        set(Python_EXECUTABLE "${_backend_python}" CACHE FILEPATH "Python interpreter")
    else()
        find_program(UV_EXECUTABLE uv)
        if(UV_EXECUTABLE)
            execute_process(
                COMMAND "${UV_EXECUTABLE}" python find 3.14 --no-project
                OUTPUT_VARIABLE _uv_python
                OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE _uv_python_result
            )
            if(_uv_python_result EQUAL 0 AND EXISTS "${_uv_python}")
                set(Python_EXECUTABLE "${_uv_python}" CACHE FILEPATH "Python interpreter")
            endif()
        endif()
    endif()
endif()

enable_testing()

add_subdirectory(engine)
add_subdirectory(bindings/python)
