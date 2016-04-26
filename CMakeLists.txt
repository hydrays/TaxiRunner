cmake_minimum_required(VERSION 2.8.8)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'. Please delete them.")
endif()

project(osrm-example C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
  message(STATUS "Building on a 64 bit system")
else()
  message(WARNING "Building on a 32 bit system is unsupported")
endif()

if(WIN32 AND MSVC_VERSION LESS 1800)
  message(FATAL_ERROR "Building with Microsoft compiler needs Visual Studio 2013 or later (Express version works too)")
endif()

FIND_PACKAGE(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
message(${GDAL_INCLUDE_DIR})
message(${GDAL_LIBRARIES})

include_directories ("${PROJECT_SOURCE_DIR}/src")
add_subdirectory (src)
set (EXTRA_LIBS ${EXTRA_LIBS} src)

add_executable(TaxiRunner main.cpp)

find_package(LibOSRM REQUIRED)
find_package(Boost 1.49.0 COMPONENTS filesystem system thread REQUIRED)

target_link_libraries(TaxiRunner ${EXTRA_LIBS} ${LibOSRM_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries (TaxiRunner ${GDAL_LIBRARIES})
include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

