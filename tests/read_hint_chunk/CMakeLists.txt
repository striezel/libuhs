# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)
project (test_read_hint_chunk)

add_definitions(-std=c++0x)
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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
