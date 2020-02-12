cmake_minimum_required(VERSION 3.0)
project(oa3D)


SET(CMAKE_SYSTEM_NAME Linux)

option(GLFW_BUILD_DOCS ON)
option(GLFW_BUILD_EXAMPLES ON)
option(GLFW_BUILD_TESTS ON)
add_subdirectory(oa3D/vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS ON)
option(ASSIMP_BUILD_SAMPLES ON)
option(ASSIMP_BUILD_TESTS ON)
add_subdirectory(oa3D/vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(oa3D/vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 ")
else()
    # -lstdc++fs == experimental filesystem include header
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(oa3D/headers/
                    oa3D/vendor/assimp/include/
                    oa3D/vendor/bullet/src/
                    oa3D/vendor/glad/
                    oa3D/vendor/glad/include/
                    oa3D/vendor/glad/include/glad/
                    oa3D/vendor/glfw/include/
                    oa3D/vendor/glm/
                    oa3D/vendor/stb/
                    oa3D/vendor/imgui/
                    oa3D/vendor/imgui/include/
                    oa3D/vendor/
                    oa3D/src/entities/
                    )

file(GLOB VENDORS_SOURCES
    oa3D/vendor/glad/src/glad.c
    oa3D/vendor/imgui/*.cpp
    oa3D/vendor/zep/filesystem.cpp
    )

file(GLOB PROJECT_HEADERS
    oa3D/headers/*.h
    )

file(GLOB PROJECT_SOURCES oa3D/src/*.cpp
                          oa3D/src/*.c
                          oa3D/src/renderer/*.cpp
                          oa3D/src/renderer/*.c
                          oa3D/src/buffers/*.cpp
                          )

file(GLOB PROJECT_SHADERS oa3D/shaders/*.glsl
                          )

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          README.md
                         .gitignore
                         .gitmodules)

source_group("headers" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("src" FILES ${PROJECT_SOURCES})
source_group("vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES}
                               oa3D/src/renderer/RendererGL.cpp
                               oa3D/headers/RendererGL.h
                               oa3D/src/buffers/IndexBuffer.cpp
                               oa3D/headers/IndexBuffer.h
                               oa3D/src/buffers/VertexBuffer.cpp
                               oa3D/headers/VertexBuffer.h
                               oa3D/src/buffers/VertexArray.cpp
                               oa3D/headers/VertexArray.h
                               oa3D/headers/VertexBufferLayout.h
                               oa3D/headers/Mesh.h)


target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      LinearMath stdc++fs)
# Disabled for now, still not using
#BulletDynamics
#BulletCollision


set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/oa3D/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
