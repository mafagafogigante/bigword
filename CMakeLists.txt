cmake_minimum_required (VERSION 3.2.2)

project (bigword)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)

set (CMAKE_CXX_STANDARD 14)

# Just add our own flags if using GCC or Clang.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wreturn-type -Wall -Wextra -Wpedantic -Werror")
endif ()

set (BIGWORD_DEFAULT_SOURCE "source.txt")
set (BIGWORD_DEFAULT_LICENSE "license.txt")

configure_file ("${PROJECT_SOURCE_DIR}/sources/configuration.hpp.in"
                "${PROJECT_BINARY_DIR}/sources/configuration.hpp")

# Add the binary tree to the search path for include files
# so that we will find it.
include_directories("${PROJECT_BINARY_DIR}/sources/")

set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable (bigword
    sources/linenumber.hpp
    sources/linestream.hpp
    sources/linestream.cpp
    sources/analyzer.hpp
    sources/analyzer.cpp
    sources/license.hpp
    sources/license.cpp
    sources/digest.hpp
    sources/digest.cpp
    sources/option.hpp
    sources/option.cpp
    sources/clock.hpp
    sources/clock.cpp
    sources/rules.hpp
    sources/rules.cpp
    sources/store.hpp
    sources/store.cpp
    sources/data.hpp
    sources/data.cpp
    sources/word.hpp
    sources/word.cpp
    sources/main.cpp)

find_package (OpenSSL REQUIRED)
find_package (Threads REQUIRED)
include_directories (${OPENSSL_INCLUDE_DIRS} ${THREADS_INCLUDE_DIRS})
target_link_libraries (bigword ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# Copy needed data files.
add_custom_command (TARGET bigword PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/LICENSE.txt
    ${CMAKE_BINARY_DIR}/${BIGWORD_DEFAULT_LICENSE}
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/data/en/words/words.txt
    ${CMAKE_BINARY_DIR}/${BIGWORD_DEFAULT_SOURCE})
