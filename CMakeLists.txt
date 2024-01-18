cmake_minimum_required(VERSION 3.25)
project(raytrace3d)

set(CMAKE_CXX_STANDARD 23)

add_executable(raytrace3d asst3.cpp)


# include engine into our project
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 3DZAVR)
