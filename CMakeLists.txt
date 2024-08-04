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

add_library(MapProjectorLib "src/map_projector/mgrs_map_projector.cpp")

# Add the sub.cpp file as a library
add_library(ConverterLib "src/converter_from_llh.cpp" "src/converter_to_llh.cpp")

# Create two executables
add_executable(PCDConverter "src/pcd_conversion.cpp")
add_executable(Lanelet2Converter "src/lanelet_conversion.cpp")
add_executable(StoppointsConverter "src/stoppoints_conversion.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -Wall")

# Link the libraries
target_link_libraries(PCDConverter ConverterLib ${PCL_LIBRARIES} yaml-cpp ${GeographicLib_LIBRARIES})
target_link_libraries(Lanelet2Converter ConverterLib ${LIBXML2_LIBRARIES} yaml-cpp ${GeographicLib_LIBRARIES})
target_link_libraries(StoppointsConverter ConverterLib yaml-cpp ${GeographicLib_LIBRARIES})

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

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test-MgrsMapProjector "test/test_mgrs_map_projector.cpp")
target_link_libraries(test-MgrsMapProjector gtest_main MapProjectorLib ${GeographicLib_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(test-MgrsMapProjector)
