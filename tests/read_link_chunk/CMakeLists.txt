cmake_minimum_required (VERSION 3.8)
project (test_read_link_chunk)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

# executable for test
add_executable(test_read_link_chunk
    ../../libuhs/chunks/BasicChunk.cpp
    ../../libuhs/chunks/LinkChunk.cpp
    ../../libuhs/utility.cpp
    main.cpp)

# add test case for reading link chunk
add_test(NAME read_link_chunk
         COMMAND $<TARGET_FILE:test_read_link_chunk> ${CMAKE_CURRENT_SOURCE_DIR}/link_chunk.dat)
