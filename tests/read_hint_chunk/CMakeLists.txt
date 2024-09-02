cmake_minimum_required (VERSION 3.8)
project (test_read_hint_chunk)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -O3 -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

# executable for test
add_executable(test_read_hint_chunk
    ../../libuhs/chunks/BasicChunk.cpp
    ../../libuhs/chunks/HintChunk.cpp
    ../../libuhs/decryption.cpp
    ../../libuhs/utility.cpp
    main.cpp)

# add test case for reading hint chunk
add_test(NAME read_hint_chunk
         COMMAND $<TARGET_FILE:test_read_hint_chunk> ${CMAKE_CURRENT_SOURCE_DIR}/example_hint_chunk.dat)
