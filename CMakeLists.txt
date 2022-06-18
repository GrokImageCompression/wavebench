cmake_minimum_required(VERSION 3.16)

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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/highway EXCLUDE_FROM_ALL)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src/highway
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tile
  ${CMAKE_CURRENT_SOURCE_DIR}/src/util
  ${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet
)
  
# Compiler specific flags:
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  set(WAVELET_COMPILE_OPTIONS ${WAVELET_COMPILE_OPTIONS} -Wall -Wextra -Wconversion -Wsign-conversion -Wunused-parameter)
endif()

set(LIBRARY_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/src/wavelet/WaveletHWY.cpp
)
 
add_executable(wavebench ${CMAKE_CURRENT_SOURCE_DIR}/src/wavebench.cpp  ${LIBRARY_SRCS} )

target_link_libraries(wavebench PRIVATE ${CMAKE_THREAD_LIBS_INIT} hwy )
target_compile_options(wavebench PRIVATE ${WAVELET_COMPILE_OPTIONS} ${HWY_FLAGS})
