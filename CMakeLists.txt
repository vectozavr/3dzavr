cmake_minimum_required(VERSION 3.17)
project(3dzavr)

set(CMAKE_CXX_STANDARD 20)

add_executable(3dzavr cube_animation.cpp tdzavrlib/utils/Time.h tdzavrlib/utils/Time.cpp tdzavrlib/utils/Point4D.h tdzavrlib/utils/Point4D.cpp tdzavrlib/utils/Matrix4x4.h tdzavrlib/utils/Matrix4x4.cpp tdzavrlib/Triangle.h tdzavrlib/Triangle.cpp tdzavrlib/Mesh.h tdzavrlib/Mesh.cpp tdzavrlib/utils/Log.h tdzavrlib/utils/Log.cpp tdzavrlib/World.h tdzavrlib/World.cpp tdzavrlib/Camera.h tdzavrlib/Camera.cpp tdzavrlib/Screen.h tdzavrlib/Screen.cpp tdzavrlib/Tdzavr.h tdzavrlib/Tdzavr.cpp tdzavrlib/Plane.h tdzavrlib/Plane.cpp tdzavrlib/CameraMesh.h tdzavrlib/CameraMesh.cpp tdzavrlib/CameraController.h tdzavrlib/CameraController.cpp tdzavrlib/Animatable.h tdzavrlib/Animation.h tdzavrlib/utils/Interpolation.h tdzavrlib/Animatable.cpp tdzavrlib/Animation.cpp)

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