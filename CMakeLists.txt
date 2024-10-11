cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
set(PROJECT_NAME CharacterEngine)
project("${PROJECT_NAME}" C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_definitions(-DWINVER=0x0501 -D_WIN32_WINNT=0x0501)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -Wno-microsoft-cast")
endif ()
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /Brepro")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /Brepro")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /Brepro")

add_compile_definitions(DIRECTINPUT_VERSION=0x0800 CURL_STATICLIB _CRT_SECURE_NO_WARNINGS $<$<CONFIG:Debug>:_DEBUG> _USE_MATH_DEFINES)
include_directories(include)
link_directories(lib)

# SokuLib
add_subdirectory(SokuLib)

# Module
add_library(
        "${PROJECT_NAME}"
        MODULE
        src/CharacterLoader/main.cpp
        src/CharacterLoader/version.rc
	src/CharacterLoader/cardSave.cpp
	src/CharacterLoader/cardSave.hpp
	src/CharacterLoader/CharacterModule.cpp
	src/CharacterLoader/CharacterModule.hpp
	src/CharacterLoader/checkSoku2.cpp
	src/CharacterLoader/checkSoku2.hpp
	src/CharacterLoader/error.cpp
	src/CharacterLoader/error.hpp
	src/CharacterLoader/giuroll.cpp
	src/CharacterLoader/giuroll.hpp
	src/CharacterLoader/scenes.cpp
	src/CharacterLoader/scenes.hpp
)
target_compile_options("${PROJECT_NAME}" PRIVATE /Zi)
target_include_directories("${PROJECT_NAME}" PRIVATE include SokuLib/directx "src/Common/")
target_link_libraries(
        "${PROJECT_NAME}"
        SokuLib
        shlwapi
        d3d9
        d3dx9
)

add_library(
	Soku2Loader MODULE
	src/Soku2Loader/main.cpp
	src/Soku2Loader/version.rc
)
target_compile_options(Soku2Loader PRIVATE /Zi)
target_include_directories(Soku2Loader PRIVATE include SokuLib/directx "src/Common/")
target_link_libraries(
	Soku2Loader
	SokuLib
	shlwapi
)

add_library(
	CharacterEngineLibrary STATIC
	src/Common/GameObjectList.hpp
	src/Common/RotatingBlockingObject.cpp
	src/Common/RotatingBlockingObject.hpp
	src/Common/BlockingObject.cpp
	src/Common/BlockingObject.hpp
	src/Common/BasicObject.cpp
	src/Common/BasicObject.hpp
	src/Common/Memory.cpp
	src/Common/Memory.hpp
	src/Common/AnglableObject.cpp
	src/Common/AnglableObject.hpp
)
target_link_libraries(
	CharacterEngineLibrary
	SokuLib
)
target_include_directories(
	CharacterEngineLibrary PUBLIC
	src/Common
)

add_subdirectory(src/Characters/tewi)