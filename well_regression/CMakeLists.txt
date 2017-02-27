cmake_minimum_required(VERSION 3.0)

# Debug or Release
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# we need more warnings
add_definitions(-W -Wall)

set(TARGET_NAME well_regression)

set(SOURCES 
    main.cpp
    well_storage.cpp
    well_math.cpp
   )	

set(HEADERS
    well_storage.hpp
    well_math.hpp
   )

add_executable(${TARGET_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${TARGET_NAME} lapack blas m gfortran)
