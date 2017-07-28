cmake_minimum_required(VERSION 2.8)

project(Argonaut)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

add_library(argonaut 
        json.cpp utf8.cpp lexer.cpp reader.cpp stream_reader.cpp
        fd_reader.cpp file_reader.cpp writer.cpp stream_writer.cpp
        fd_writer.cpp file_writer.cpp token.cpp json_exception.cpp 
        parser.cpp unparser.cpp json_io_exception.cpp 
        json_parser_exception.cpp json_utf8_exception.cpp
        json_array_index_range_exception.cpp
        json_invalid_key_exception.cpp)

add_executable(json_test json_test.cpp)
target_link_libraries(json_test argonaut)
add_executable(json_example json_example.cpp)
target_link_libraries(json_example argonaut)

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(
        doc
        COMMAND ${DOXYGEN_EXECUTABLE} doxygen.conf
        COMMENT "Generating documentation using Doxygen"
        VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the Argonaut documentation")
endif (DOXYGEN_FOUND)

add_custom_command(TARGET json_test POST_BUILD COMMAND ./json_test COMMENT "Running unit tests...")
