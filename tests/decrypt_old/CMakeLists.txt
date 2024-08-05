cmake_minimum_required (VERSION 2.8)
project (test_decrypt_old)

add_definitions(-std=c++0x)
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

# executable for test
add_executable(decrypt_old
    ../../libuhs/decryption.cpp
    main.cpp)

# add test case for decryption
add_test(NAME decryption_old
         COMMAND $<TARGET_FILE:decrypt_old>)
