cmake_minimum_required(VERSION 3.13)

project(MaiCra)

add_definitions(-std=c++11)

SET(CMAKE_CXX_STANDARD_LIBRARIES "-lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -ldl -lXinerama -lXcursor -lSDL2")

add_executable(
        MaiCra
        main.cpp
        libs/glad/glad.c
        Game/Game.cpp Game/Game.h
        Window/Window.cpp Window/Window.h
        Shader/ShaderLoader.cpp Shader/ShaderLoader.h
        Cube/Cube.cpp Cube/Cube.h
        TextureLoader/TextureLoader.cpp TextureLoader/TextureLoader.h
        Renderer/Renderer.cpp Renderer/Renderer.h
        Camera/Camera.cpp Camera/Camera.h
        Resources/cubes/cubeTypes.h
        Resources/textures/textureTypes.h
        gameSettings.h gameSettings.cpp
        Control/Control.cpp Control/Control.h
        Resources/cubes/cubeTypes.cpp Resources/textures/textureTypes.cpp

)

