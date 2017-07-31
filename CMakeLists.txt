cmake_minimum_required(VERSION 3.5)
project(graphvisualizer)

IF (WIN32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/windows")        
ELSE()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/linux")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
#imgui settings
        src/imgui/imgui.cpp
        src/imgui/imgui.h
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_internal.h
        src/imgui/imconfig.h
        src/imgui/stb_rect_pack.h
        src/imgui/stb_textedit.h
        src/imgui/stb_truetype.h
        src/imgui/imgui_impl_sdl_gl3.cpp
        src/imgui/imgui_impl_sdl_gl3.h
#end imgui

        src/glad.c
        src/system/input/InputManager.cpp src/system/input/InputManager.h

        src/graphic/SettingsSDL.cpp src/graphic/SettingsSDL.h
        src/graphic/SettingsOpengl.cpp src/graphic/SettingsOpengl.h
        src/helper/RandomNumberGenerator.h
        src/config/AppConfig.h
        src/helper/FileHelper.h
        src/helper/Logger.h

        src/graphic/Shader.cpp src/graphic/Shader.h
        src/graphic/ComputeShader.cpp src/graphic/ComputeShader.h
        src/graphic/Renderer.cpp src/graphic/Renderer.h
        src/models/InstancedModel.cpp src/models/InstancedModel.h
        src/models/drawable/Model.cpp src/models/drawable/Model.h
        src/graphic/camera/Camera.cpp src/graphic/camera/Camera.h

        src/models/drawable/IDrawableObject.h
        
        src/system/App.cpp src/system/App.h
        src/main.cpp
        # src/test.cpp

        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(external/glm/include)
include_directories(external/glad/include)
include_directories(external/sdl2/include)
include_directories(external/json/include)

IF (WIN32)

        file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/windows/build/graph/Debug")
        
        set(SDL2_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/sdl2win32/SDL2.lib")
        set(SDL2_DLL_DIR_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/external/sdl2win32/SDL2main.lib")
        
        file(COPY res DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/windows/build/graph/Debug)
        file(COPY SDL2.dll DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/windows/build/graph/Debug)

        target_link_libraries( ${PROJECT_NAME} opengl32 ${SDL2_DLL_DIR} ${SDL2_DLL_DIR_MAIN})
ELSE()
        file(COPY res DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/linux)
        file(COPY array.edges DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/linux)
        file(COPY settings.json DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/linux)
        
        target_link_libraries( ${PROJECT_NAME} GL dl SDL2)
ENDIF()