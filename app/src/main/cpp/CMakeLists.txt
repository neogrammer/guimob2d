cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("${APP_LIB_NAME}")

set(PLATFORM Android CACHE STRING "" FORCE)
set(OPENGL_VERSION "ES 3.0" CACHE STRING "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raylib)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raymob)

# Android native glue
include_directories(${ANDROID_NDK}/sources/android/native_app_glue/)

# Only compile the Pong path for now.
# Do NOT compile WorkMapEditor / imgui files during the switchover.
set(SOURCES
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c

    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/Globs.cpp

    ${CMAKE_SOURCE_DIR}/Component.cpp
    ${CMAKE_SOURCE_DIR}/AIComponent.cpp
    ${CMAKE_SOURCE_DIR}/BoundingBoxComponent.cpp
    ${CMAKE_SOURCE_DIR}/CollisionComponent.cpp
    ${CMAKE_SOURCE_DIR}/InputComponent.cpp
    ${CMAKE_SOURCE_DIR}/SpriteComponent.cpp
    ${CMAKE_SOURCE_DIR}/TransformComponent.cpp

    ${CMAKE_SOURCE_DIR}/AISystem.cpp
    ${CMAKE_SOURCE_DIR}/CollisionSystem.cpp
    ${CMAKE_SOURCE_DIR}/InputSystem.cpp
    ${CMAKE_SOURCE_DIR}/RenderSystem.cpp
    ${CMAKE_SOURCE_DIR}/System.cpp

    ${CMAKE_SOURCE_DIR}/Game.cpp
    ${CMAKE_SOURCE_DIR}/Utils.cpp

    ${CMAKE_SOURCE_DIR}/imgui_raymob.cpp
    ${CMAKE_SOURCE_DIR}/WorkMapEditor.cpp

    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${APP_LIB_NAME} SHARED ${SOURCES})
target_link_options(${APP_LIB_NAME} PRIVATE "-Wl,-z,max-page-size=16384")

target_compile_definitions(${APP_LIB_NAME} PRIVATE PLATFORM_ANDROID IMGUI_IMPL_OPENGL_ES3)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${APP_LIB_NAME} PRIVATE _DEBUG DEBUG)
endif()

target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raylib")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raymob")

target_link_libraries(${APP_LIB_NAME} raylib raymoblib)