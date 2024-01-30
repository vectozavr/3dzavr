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

        linalg/Vec2D.h
        linalg/Vec2D.cpp
        linalg/Vec3D.h
        linalg/Vec3D.cpp
        linalg/Vec4D.h
        linalg/Vec4D.cpp
        linalg/Matrix4x4.h
        linalg/Matrix4x4.cpp

        objects/Camera.h
        objects/Camera.cpp
        objects/Object.h
        objects/Object.cpp
        objects/Group.h
        objects/Group.cpp
        objects/DirectionalLight.h
        objects/DirectionalLight.cpp

        objects/props/Color.h
        objects/props/Color.cpp
        objects/props/Texture.h
        objects/props/Texture.cpp
        objects/props/Material.h
        objects/props/Material.cpp

        objects/geometry/Triangle.h
        objects/geometry/Triangle.cpp
        objects/geometry/Plane.h
        objects/geometry/Plane.cpp
        objects/geometry/Sphere.h
        objects/geometry/Sphere.cpp
        objects/geometry/Mesh.h
        objects/geometry/Mesh.cpp

        physics/RigidBody.h
        physics/RigidBody.cpp
        physics/Simplex.h
        physics/HitBox.h
        physics/HitBox.cpp

        io/Image.h
        io/Image.cpp
        io/Screen.h
        io/Screen.cpp
        io/Keyboard.h
        io/Keyboard.cpp
        io/Mouse.h
        io/Mouse.cpp

        utils/Log.h
        utils/Log.cpp
        utils/Time.h
        utils/Time.cpp
        utils/Timer.h
        utils/Timer.cpp
        utils/EventHandler.h
        utils/EventHandler.cpp
        utils/ResourceManager.h
        utils/ResourceManager.cpp
        utils/FilePath.h
        utils/FilePath.cpp
        utils/Font.h
        utils/Font.cpp
        utils/ObjectController.h
        utils/ObjectController.cpp

        animation/Animation.h
        animation/Animation.cpp
        animation/Timeline.h
        animation/Timeline.cpp
        animation/Interpolation.h
        animation/Interpolation.cpp
        animation/ATranslate.h
        animation/AScale.h
        animation/ARotate.h
        animation/AWait.h
        animation/AFunction.h
        animation/AAttractToPoint.h
        animation/ARotateRelativePoint.h
        animation/ARotateLeft.h
        animation/Animations.h
        animation/AShowCreation.h
        animation/AShowUncreation.h
        animation/ARotateLeftUpLookAt.h
        animation/ADecompose.h)

target_include_directories(3DZAVR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

# LibPNG library
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(3DZAVR PUBLIC PNG::PNG)

# SDL Library
find_package(SDL2 REQUIRED)
# SDL_TTF (this one works for MacOS + Clang compiler)
#find_package(SDL2_ttf REQUIRED)
target_link_libraries(3DZAVR PUBLIC SDL2::SDL2)

# SDL_TTF (this one works for Linux + GCC compiler)
find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(3DZAVR PUBLIC ${SDL2TTF_LIBRARIES})
