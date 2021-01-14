cmake_minimum_required(VERSION 3.17)
project(3dzavr)

set(CMAKE_CXX_STANDARD 20)

add_executable(3dzavr main.cpp Time.h Time.cpp Point4D.h Point4D.cpp Matrix4x4.h Matrix4x4.cpp Triangle.h Triangle.cpp Mesh.h Mesh.cpp Log.h Log.cpp World.h World.cpp Camera.h Camera.cpp Screen.h Screen.cpp Tdzavr.h Tdzavr.cpp)

if(${APPLE})
    include_directories(/usr/local/include)
else()

    set(SFML_DIR "D:/Libraries/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)

endif()

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()
##

target_link_libraries(3dzavr sfml-audio sfml-network sfml-graphics sfml-window sfml-system)