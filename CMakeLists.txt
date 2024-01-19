cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(3dzavr_test)

add_executable(${CMAKE_PROJECT_NAME} asst3.cpp)


# include engine into our project
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 3DZAVR)
