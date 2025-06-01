cmake_minimum_required(VERSION 3.21)
project(3dAutomata LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(SOURCES
    src/main.cpp
    src/Run.cpp
    src/IndexBuffer.cpp
    src/Renderer.cpp
    src/Shader.cpp 
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    external/glad/src/glad.c
    src/internal/imgui/imgui.cpp
    src/internal/imgui/imgui_draw.cpp
    src/internal/imgui/imgui_tables.cpp
    src/internal/imgui/imgui_widgets.cpp
    src/internal/imgui/imgui_impl_glfw.cpp
    src/internal/imgui/imgui_impl_opengl3.cpp
)

add_executable(3dAutomata ${SOURCES})

target_include_directories(3dAutomata PRIVATE
    src 
    src/internal 
    src/internal/glm
    src/internal/imgui
    external/glad/include
    external/glfw/include
    src/include
)

target_compile_definitions(3dAutomata PRIVATE
    WIN32
    _DEBUG
    _CONSOLE
    GLM_ENABLE_EXPERIMENTAL
)

target_include_directories(3dAutomata PRIVATE
    external/glew/include
    external/glfw/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/dependencies/glfw/lib
)

target_link_libraries(3dAutomata PRIVATE
    opengl32
    ${CMAKE_SOURCE_DIR}/external/glfw/lib/libglfw3.a
)