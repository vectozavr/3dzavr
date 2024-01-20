cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

# create target
add_library(
        3DZAVR STATIC
        Consts.h
        Engine.h
        Engine.cpp
        World.h
        World.cpp
        linalg/Vec4D.h
        linalg/Vec4D.cpp
        linalg/Vec3D.cpp
        linalg/Vec3D.h
        linalg/Vec2D.cpp
        linalg/Vec2D.h
        linalg/Matrix4x4.h
        linalg/Matrix4x4.cpp
        linalg/Color.cpp
        linalg/Color.h
        objects/Camera.h
        objects/Camera.cpp
        objects/Object.cpp
        objects/Object.h
        objects/DirectionalLight.cpp
        objects/DirectionalLight.h
        objects/geometry/Triangle.h
        objects/geometry/Triangle.cpp
        objects/geometry/Plane.h
        objects/geometry/Plane.cpp
        objects/geometry/Sphere.cpp
        objects/geometry/Sphere.h
        objects/geometry/Mesh.h
        objects/geometry/Mesh.cpp
        physics/RigidBody.cpp
        physics/RigidBody.h
        physics/Simplex.h
        physics/HitBox.cpp
        physics/HitBox.h
        io/Image.cpp
        io/Image.h
        io/Screen.h
        io/Screen.cpp
        io/Keyboard.cpp
        io/Keyboard.h
        io/Mouse.cpp
        io/Mouse.h
        utils/Log.h
        utils/Log.cpp
        utils/Time.h
        utils/Time.cpp
        utils/Timer.cpp
        utils/Timer.h
        utils/EventHandler.cpp
        utils/EventHandler.h
        utils/ResourceManager.h
        utils/ResourceManager.cpp
        utils/ObjectController.h
        utils/ObjectController.cpp
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
        objects/geometry/Texture.cpp objects/geometry/Texture.h)

target_include_directories(3DZAVR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

# LibPNG library
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(3DZAVR PUBLIC PNG::PNG)

# SDL Library
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_link_libraries(3DZAVR PUBLIC SDL2::SDL2 SDL2_ttf::SDL2_ttf)