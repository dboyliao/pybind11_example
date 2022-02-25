cmake_minimum_required(VERSION 3.0.0)
project(pybind11_numpy VERSION 0.1.0)

# Add pybind11
add_subdirectory(thirdparty/pybind11)

# Add OpenCV
find_package(OpenCV 4.0 QUIET)
if(NOT OpenCV_FOUND)
  find_package(OpenCV 3.0 QUIET)
  if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV > 3.0 not found.")
  endif()
endif()
message("Found OpenCV Version: ${OpenCV_VERSION}")
set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE Sources 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cc"
)
pybind11_add_module(
  pybind11_numpy SHARED ${Sources}
)
target_include_directories(pybind11_numpy PUBLIC include)
target_link_libraries(pybind11_numpy PRIVATE ${OpenCV_LIBS})
set_target_properties(pybind11_numpy
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/pybind11_example"
)