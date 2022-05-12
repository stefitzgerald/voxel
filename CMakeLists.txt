cmake_minimum_required(VERSION 3.0.0)
project(voxel VERSION 0.1.0)

add_executable(
    voxel 
    src/main.cpp

    src/ogl/glError.h
    src/ogl/glError.cpp

    src/core/application.h
    src/core/application.cpp
    src/core/core_application.h
    src/core/core_application.cpp
    src/core/shader.h
    src/core/shader.cpp
)

set_property(TARGET voxel PROPERTY CXX_STANDARD 17)
target_compile_options(voxel PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(extern/glfw EXCLUDE_FROM_ALL)
add_subdirectory(extern/glew EXCLUDE_FROM_ALL)
add_subdirectory(extern/glm EXCLUDE_FROM_ALL)

target_link_libraries(
    voxel
    PRIVATE glfw
    PRIVATE libglew_static
    PRIVATE glm
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/asset.h
)

target_include_directories(
    voxel
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)