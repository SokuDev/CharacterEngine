cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
set(PROJECT_NAME CharacterEngine)
project("${PROJECT_NAME}" C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_definitions(-DWINVER=0x0501 -D_WIN32_WINNT=0x0501)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -Wno-microsoft-cast")
endif ()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /Brepro")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /Brepro")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /Brepro")

# SokuLib
add_subdirectory(SokuLib)

# Module
add_library(
        "${PROJECT_NAME}"
        MODULE
        src/main.cpp
        src/version.rc
        src/ProxyCharacter.hpp
        src/log.hpp
        src/tewi/Tewi.hpp
        src/tewi/Tewi.cpp
        src/Common/GameObjectList.hpp
        src/tewi/TewiObject.cpp
        src/tewi/TewiObject.hpp
        src/tewi/TewiObjectFactory.cpp
        src/tewi/TewiObjectFactory.hpp
        src/Common/BlockingObject.cpp
        src/Common/BlockingObject.hpp
        src/Common/DefaultFactory.cpp
        src/Common/DefaultFactory.hpp
        src/tewi/Hammer.cpp
        src/tewi/Hammer.hpp
        src/Common/BasicObject.cpp
        src/Common/BasicObject.hpp
        src/Common/Memory.cpp
        src/Common/Memory.hpp
)
target_compile_options("${PROJECT_NAME}" PRIVATE /Zi)
target_compile_definitions("${PROJECT_NAME}" PRIVATE DIRECTINPUT_VERSION=0x0800 CURL_STATICLIB _CRT_SECURE_NO_WARNINGS $<$<CONFIG:Debug>:_DEBUG>)
target_include_directories("${PROJECT_NAME}" PRIVATE include SokuLib/directx "src/Common/")
target_link_directories("${PROJECT_NAME}" PRIVATE lib)
target_link_libraries(
        "${PROJECT_NAME}"
        SokuLib
        shlwapi
        d3d9
        d3dx9
)