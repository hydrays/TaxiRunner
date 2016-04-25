find_package(LibOSRM REQUIRED)
find_package(Boost 1.49.0 COMPONENTS filesystem system thread REQUIRED)

include_directories(SYSTEM ${LibOSRM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
add_library(src runner.cpp convector.cpp collector.cpp)