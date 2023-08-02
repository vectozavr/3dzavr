cmake_minimum_required(VERSION 3.17)
project(engine_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${CMAKE_PROJECT_NAME}
        # custom files:
        Source.cpp
        )

# include 3dzavr engine into our project
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 3DZAVR)
