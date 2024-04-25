cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

# create target
add_library(
        3DZAVR STATIC
        ScalarConsts.h
        Consts.h
        Engine.h
        Engine.cpp
        World.h
        World.cpp

        linalg/Vec2D.h
        linalg/Vec3D.h
        linalg/Vec4D.h
        linalg/Matrix3x3.h
        linalg/Matrix4x4.h

        objects/Camera.h
        objects/Camera.cpp
        objects/Object.h
        objects/Object.cpp
        objects/Group.h
        objects/Group.cpp

        components/Component.h

        components/TransformMatrix.h
        components/TransformMatrix.cpp

        components/geometry/Bounds.h
        components/geometry/Triangle.h
        components/geometry/Triangle.cpp
        components/geometry/Line.h
        components/geometry/Plane.h
        components/geometry/Plane.cpp
        components/geometry/TriangleMesh.h
        components/geometry/TriangleMesh.cpp
        components/geometry/LineMesh.h
        components/geometry/LineMesh.cpp

        components/lighting/LightSource.h
        components/lighting/DirectionalLight.h
        components/lighting/PointLight.h
        components/lighting/SpotLight.h

        components/props/Color.h
        components/props/Color.cpp
        components/props/Texture.h
        components/props/Texture.cpp
        components/props/Material.h
        components/props/Material.cpp

        components/physics/RigidObject.h
        components/physics/RigidObject.cpp
        components/physics/Simplex.h
        components/physics/HitBox.h
        components/physics/HitBox.cpp
        components/physics/GJK.h
        components/physics/GJK.cpp
        components/physics/EPA.h
        components/physics/EPA.cpp

        io/Image.h
        io/Image.cpp
        io/Screen.h
        io/Screen.cpp
        io/Keyboard.h
        io/Keyboard.cpp
        io/Mouse.h
        io/Mouse.cpp

        io/microui/microui.h
        io/microui/microui.c

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
        utils/WorldEditor.h
        utils/WorldEditor.cpp
        utils/stack_vector.h
        utils/math.h
        utils/math.cpp
        utils/monitoring.h
        utils/monitoring.cpp

        animation/Animation.h
        animation/Animation.cpp
        animation/Timeline.h
        animation/Timeline.cpp
        animation/Interpolation.h
        animation/Interpolation.cpp
        animation/Translate.h
        animation/Scale.h
        animation/Rotate.h
        animation/Wait.h
        animation/Function.h
        animation/AttractToPoint.h
        animation/RotateRelativePoint.h
        animation/RotateLeft.h
        animation/Animations.h
        animation/ShowCreation.h
        animation/ShowUncreation.h
        animation/RotateLeftUpLookAt.h
        animation/Decompose.h
        animation/TranslateToPoint.h
)

target_include_directories(3DZAVR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

include_directories(/usr/local/include)

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_error)

if(lto_supported)
    # I don't think you want to make debug builds harder to debug and longer to build
    set_property(TARGET 3DZAVR PROPERTY INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE)
    set_property(TARGET 3DZAVR PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "CMake can't enable LTO optimizations for your current compiler.\n${lto_error}")
endif()

# LibPNG library
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(3DZAVR PUBLIC PNG::PNG)

# SDL Library (works for Linux and MacOS)
find_package(SDL2 REQUIRED)
target_link_libraries(3DZAVR PUBLIC SDL2::SDL2)

# SDL_TTF (works for MacOS)
find_package(SDL2_ttf REQUIRED)
target_link_libraries(3DZAVR PUBLIC SDL2::SDL2 SDL2_ttf::SDL2_ttf)

# SDL_TTF (works for Linux)
#find_package(PkgConfig REQUIRED)
#pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
#include_directories(${SDL2TTF_INCLUDE_DIRS})
#target_link_libraries(3DZAVR PUBLIC ${SDL2TTF_LIBRARIES})
