add_library(
    polyline
    polyline.cpp
    polyline.hpp
)

target_include_directories(polyline PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
