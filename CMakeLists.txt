cmake_minimum_required(VERSION 3.10)

project(main)

find_package(OpenCV REQUIRED)

add_executable(main main.cpp my_filter.cpp find_min_area.cpp find_close_point find_far_point point_color.cpp get_center_point.cpp)

target_link_libraries(main ${OpenCV_LIBS})