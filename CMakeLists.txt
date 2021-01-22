cmake_minimum_required(VERSION 3.17)
project(3dzavr)

set(CMAKE_CXX_STANDARD 20)

add_executable(3dzavr physics_example.cpp utils/Time.h utils/Time.cpp utils/Point4D.h utils/Point4D.cpp utils/Matrix4x4.h utils/Matrix4x4.cpp Triangle.h Triangle.cpp Mesh.h Mesh.cpp utils/Log.h utils/Log.cpp World.h World.cpp Camera.h Camera.cpp Screen.h Screen.cpp Tdzavr.h Tdzavr.cpp Plane.h Plane.cpp CameraMesh.h CameraMesh.cpp)

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