cmake_minimum_required(VERSION 3.17)
project(3dzavr)

set(CMAKE_CXX_STANDARD 20)

include_directories(tdzavrlib)
add_library(tdzavrlib STATIC
        tdzavrlib/utils/Time.h
        tdzavrlib/utils/Time.cpp
        tdzavrlib/utils/Point4D.h
        tdzavrlib/utils/Point4D.cpp
        tdzavrlib/utils/Matrix4x4.h
        tdzavrlib/utils/Matrix4x4.cpp
        tdzavrlib/Triangle.h
        tdzavrlib/Triangle.cpp
        tdzavrlib/Mesh.h
        tdzavrlib/Mesh.cpp
        tdzavrlib/utils/Log.h
        tdzavrlib/utils/Log.cpp
        tdzavrlib/ResourceManager.h
        tdzavrlib/ResourceManager.cpp
        tdzavrlib/World.h
        tdzavrlib/World.cpp
        tdzavrlib/Camera.h
        tdzavrlib/Camera.cpp
        tdzavrlib/Screen.h
        tdzavrlib/Screen.cpp
        tdzavrlib/Tdzavr.h
        tdzavrlib/Tdzavr.cpp
        tdzavrlib/Plane.h
        tdzavrlib/Plane.cpp
        tdzavrlib/CameraMesh.h
        tdzavrlib/CameraMesh.cpp
        tdzavrlib/CameraController.h
        tdzavrlib/CameraController.cpp
        tdzavrlib/animation/Animatable.h
        tdzavrlib/animation/Animation.h
        tdzavrlib/animation/Interpolation.h
        tdzavrlib/animation/Animatable.cpp
        tdzavrlib/animation/Animation.cpp
        tdzavrlib/animation/ATranslate.h
        tdzavrlib/animation/ATranslateToPoint.h
        tdzavrlib/animation/AAttractToPoint.h
        tdzavrlib/animation/AScale.h
        tdzavrlib/animation/ADecompose.h
        tdzavrlib/animation/ARotate.h
        tdzavrlib/animation/ARotateRelativePoint.h
        tdzavrlib/animation/ARotateLeftUpLookAt.h
        tdzavrlib/animation/AShowCreation.h
        tdzavrlib/animation/AWait.h
        tdzavrlib/physics/RigidBody.cpp
        tdzavrlib/physics/RigidBody.h
        tdzavrlib/animation/ARotateToAngle.h
        tdzavrlib/animation/ARotateLeftUpLookAtToAngle.h
        tdzavrlib/physics/Simplex.h
        tdzavrlib/physics/Solver.cpp
        tdzavrlib/physics/Solver.h
        tdzavrlib/Object.cpp
        tdzavrlib/Object.h
        tdzavrlib/gui/Button.cpp
        tdzavrlib/gui/Button.h
        tdzavrlib/gui/Window.cpp
        tdzavrlib/gui/Window.h
        tdzavrlib/network/ClientUDP.cpp
        tdzavrlib/network/ClientUDP.h
        tdzavrlib/network/MsgType.cpp
        tdzavrlib/network/MsgType.h
        tdzavrlib/network/ReliableMsg.cpp
        tdzavrlib/network/ReliableMsg.h
        tdzavrlib/network/ServerUDP.cpp
        tdzavrlib/network/ServerUDP.h
        tdzavrlib/network/UDPConnection.cpp
        tdzavrlib/network/UDPConnection.h
        tdzavrlib/network/UDPSocket.cpp
        tdzavrlib/network/UDPSocket.h
        tdzavrlib/network/config.h
        tdzavrlib/animation/AFunction.h)

add_executable(3dzavr mountains_animation.cpp)

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

#find_library(3DZAVR 3dzavr lib)
target_link_libraries(3dzavr tdzavrlib sfml-audio sfml-network sfml-graphics sfml-window sfml-system)