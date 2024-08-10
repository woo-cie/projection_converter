cmake_minimum_required(VERSION 3.30)

# Define the project and set the C++ standard
project(ProjectionConverter)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PCL 1.3 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(OpenMP REQUIRED)
find_package(libxml2 2.12 REQUIRED)

# find_package(GeographicLib REQUIRED)
find_path(GeographicLib_INCLUDE_DIRS NAMES GeographicLib/Config.h)
find_library(GeographicLib_LIBRARIES NAMES GeographicLib)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(include)

link_directories(${PCL_LIBRARY_DIRS})
link_directories(${LIBXML2_LIBRARIES})

add_definitions(${PCL_DEFINITIONS})

# Add the sub.cpp file as a library
add_library(MapProjectorLib "src/map_projector/mgrs_map_projector.cpp" "src/map_projector/tm_map_projector.cpp" "src/map_projector/map_projector.cpp")

# Create two executables
add_executable(PCDConverter "src/pcd_conversion.cpp")
add_executable(Lanelet2Converter "src/lanelet_conversion.cpp")
add_executable(StoppointsConverter "src/stoppoints_conversion.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -Wall")

# Link the libraries
target_link_libraries(PCDConverter MapProjectorLib ${PCL_LIBRARIES} yaml-cpp ${GeographicLib_LIBRARIES})
target_link_libraries(Lanelet2Converter MapProjectorLib ${LIBXML2_LIBRARIES} yaml-cpp ${GeographicLib_LIBRARIES})
target_link_libraries(StoppointsConverter MapProjectorLib yaml-cpp ${GeographicLib_LIBRARIES})

# For test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

add_executable(test-MgrsMapProjector "test/test_mgrs_map_projector.cpp")
target_link_libraries(test-MgrsMapProjector gtest_main MapProjectorLib ${GeographicLib_LIBRARIES})
gtest_discover_tests(test-MgrsMapProjector)

add_executable(test-TmMapProjector "test/test_tm_map_projector.cpp")
target_link_libraries(test-TmMapProjector gtest_main MapProjectorLib ${GeographicLib_LIBRARIES})
gtest_discover_tests(test-TmMapProjector)
