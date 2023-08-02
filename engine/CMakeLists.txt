cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

# create target
add_library(
        3DZAVR STATIC
        Consts.h
        Engine.h
        Engine.cpp
        World.h
        World.cpp
        Camera.h
        Camera.cpp
        Object.cpp
        Object.h
        linalg/Vec4D.h
        linalg/Vec4D.cpp
        linalg/Vec3D.cpp
        linalg/Vec3D.h
        linalg/Vec2D.cpp
        linalg/Vec2D.h
        linalg/Matrix4x4.h
        linalg/Matrix4x4.cpp
        geometry/Triangle.h
        geometry/Triangle.cpp
        geometry/Plane.h
        geometry/Plane.cpp
        geometry/Mesh.h
        geometry/Mesh.cpp
        physics/RigidBody.cpp
        physics/RigidBody.h
        physics/Simplex.h
        physics/HitBox.cpp
        physics/HitBox.h
        io/Screen.h
        io/Screen.cpp
        io/Keyboard.cpp
        io/Keyboard.h
        io/Mouse.cpp
        io/Mouse.h
        io/SoundController.cpp
        io/SoundController.h
        utils/Log.h
        utils/Log.cpp
        utils/Time.h
        utils/Time.cpp
        utils/Timer.cpp
        utils/Timer.h
        utils/ResourceManager.h
        utils/ResourceManager.cpp
        utils/ObjectController.cpp
        utils/ObjectController.h
        utils/EventHandler.cpp
        utils/EventHandler.h
        animation/Animation.h
        animation/Timeline.cpp
        animation/Timeline.h
        animation/Interpolation.h
        animation/Animation.cpp
        animation/ATranslate.h
        animation/AScale.h
        animation/ARotate.h
        animation/AWait.h
        animation/AFunction.h
        animation/AAttractToPoint.h
        animation/ARotateRelativePoint.h
        animation/ARotateLeft.h
        animation/Interpolation.cpp
        animation/Animations.h
        animation/AShowCreation.h
        animation/AShowUncreation.h
        animation/ARotateLeftUpLookAt.h
        animation/ADecompose.h
        network/ClientUDP.cpp
        network/ClientUDP.h
        network/MsgType.cpp
        network/MsgType.h
        network/ReliableMsg.cpp
        network/ReliableMsg.h
        network/ServerUDP.cpp
        network/ServerUDP.h
        network/UDPConnection.cpp
        network/UDPConnection.h
        network/UDPSocket.cpp
        network/UDPSocket.h
        gui/Button.cpp
        gui/Button.h
        gui/Window.cpp
        gui/Window.h
)

target_include_directories(3DZAVR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

# SFML part
if(APPLE OR UNIX)
    include_directories(/usr/local/include)
else()
    set(SFML_DIR "C:/Libraries/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    target_include_directories(3DZAVR PUBLIC ${SFML_INCLUDE_DIR})
endif()

target_link_libraries(3DZAVR PUBLIC sfml-audio sfml-network sfml-graphics sfml-window sfml-system)

# OpenGL part
if (APPLE)
    set(GLEW_H /usr/local/Cellar/glew/2.1.0/include/GL)
    set(GLFW_H /usr/local/Cellar/glfw/3.2.1/include/GLFW)
    target_include_directories(3DZAVR PUBLIC ${GLEW_H} ${GLFW_H})

    set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
    set(GLFW_LINK /usr/local/Cellar/glfw/3.2.1/lib/libglfw.3.dylib)
    link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

    target_link_libraries(3DZAVR PUBLIC "-framework OpenGL")
    target_link_libraries(3DZAVR PUBLIC "-framework GLUT")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)

    target_link_libraries(3DZAVR PUBLIC ${OPENGL_LIBRARIES})
    target_link_libraries(3DZAVR PUBLIC ${GLUT_LIBRARY})
endif()
