cmake_minimum_required(VERSION 3.5.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)

add_executable(main ../src/test.cpp ../src/find.cpp main.cpp ../src/pnp.cpp )

include_directories(${CMAKE_SOURCE_DIR}/include/)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(main ${OpenCV_LIBS})