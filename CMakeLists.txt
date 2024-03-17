cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(3dzavr)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${CMAKE_PROJECT_NAME} test_scenes/past_village.cpp)

find_package(SDL2 CONFIG COMPONENTS SDL2main)
if(TARGET SDL2::SDL2main)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main)
else()
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
endif()

# include engine into our project
add_subdirectory(engine)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 3DZAVR)
