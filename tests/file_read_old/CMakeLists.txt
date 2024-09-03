cmake_minimum_required (VERSION 3.8)
project (test_file_read_old)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wshadow -Wpedantic -pedantic-errors -O3 -fexceptions)
    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

# executable for test
add_executable(test_file_read_old
    ../../libuhs/ChunkReader.cpp
    ../../libuhs/chunks/BasicChunk.cpp
    ../../libuhs/chunks/HintChunk.cpp
    ../../libuhs/chunks/HyperPNGChunk.cpp
    ../../libuhs/chunks/InfoChunk.cpp
    ../../libuhs/chunks/LinkChunk.cpp
    ../../libuhs/chunks/NestHintChunk.cpp
    ../../libuhs/chunks/SubjectChunk.cpp
    ../../libuhs/chunks/TextChunk.cpp
    ../../libuhs/chunks/VersionChunk.cpp
    ../../libuhs/decryption.cpp
    ../../libuhs/file.cpp
    ../../libuhs/utility.cpp
    main.cpp)

# add test case for reading old file format
add_test(NAME read_old_format
         COMMAND $<TARGET_FILE:test_file_read_old> ${CMAKE_CURRENT_SOURCE_DIR}/file_read_old.dat)
