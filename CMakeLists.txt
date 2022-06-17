cmake_minimum_required(VERSION 3.17)

project(wavelet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

find_package(Threads REQUIRED)

find_package(Valgrind)
message(STATUS "Looking for Valgrind")
if (Valgrind_FOUND)
    message(STATUS "Found Valgrind")
    add_definitions(-DHAVE_VALGRIND)
    include_directories(${Valgrind_INCLUDE_DIR})
else(Valgrind_FOUND)
    message(STATUS "Valgrind not found")
endif(Valgrind_FOUND)

include(GNUInstallDirs)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  ${TIFF_INCLUDE_DIRNAME})
  
# Compiler specific flags:
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  set(WAVELET_COMPILE_OPTIONS ${WAVELET_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -Wsign-conversion -Wunused-parameter)
endif()

set(LIBRARY_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet/wavelet.cpp
)
  
add_executable(wavelet ${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet.cpp  ${LIBRARY_SRCS} )

target_link_libraries(wavelet ${CMAKE_THREAD_LIBS_INIT} )

target_compile_options(wavelet PRIVATE ${WAVELET_COMPILE_OPTIONS})
