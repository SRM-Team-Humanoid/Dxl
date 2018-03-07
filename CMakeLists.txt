cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories("/usr/local/include")
add_executable(
  sample 
  Dxl.cpp
  )
target_link_libraries(sample /usr/local/lib/libdxl_x64_cpp.so)
