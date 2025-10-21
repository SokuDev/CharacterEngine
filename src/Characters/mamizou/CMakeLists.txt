cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
set(PROJECT_NAME Mamizou)
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

# Module
add_library(
	"${PROJECT_NAME}" MODULE
	version.rc
	entry_point.cpp
	Mamizou.hpp
	Mamizou.cpp
	MamizouObjectFactory.cpp
	MamizouObjectFactory.hpp
	objects/Smoke.cpp
	objects/Smoke.hpp
	objects/YoukaiShadow.cpp
	objects/YoukaiShadow.hpp
	objects/YoukaiShadow2.cpp
	objects/YoukaiShadow2.hpp
	objects/FrogShadow.cpp
	objects/FrogShadow.hpp
	objects/Gui/TimerDebuffGui.cpp
	objects/Gui/TimerDebuffGui.hpp
	objects/Gui/TimerSpellGui.cpp
	objects/Gui/TimerSpellGui.hpp
	objects/Gui/TimerGui.cpp
	objects/Gui/TimerGui.hpp
	objects/Gui/StackGui.cpp
	objects/Gui/StackGui.hpp
	objects/LeafSpawner.cpp
	objects/LeafSpawner.hpp
	objects/SmokeBullet.cpp
	objects/SmokeBullet.hpp
	objects/Umbrella2C.cpp
	objects/Umbrella2C.hpp
	objects/BirdShadow.cpp
	objects/BirdShadow.hpp
	objects/BucketYoukai.cpp
	objects/BucketYoukai.hpp
	objects/Umbrella5C.cpp
	objects/Umbrella5C.hpp
	objects/CrabYoukai.cpp
	objects/CrabYoukai.hpp
	objects/LanternYoukai.cpp
	objects/LanternYoukai.hpp
	objects/RegularSmoke.cpp
	objects/RegularSmoke.hpp
	objects/HotPot.cpp
	objects/HotPot.hpp
	objects/Gui/CardSelect.cpp
	objects/Gui/CardSelect.hpp
	objects/DemonGate.cpp
	objects/DemonGate.hpp
)
target_compile_options("${PROJECT_NAME}" PRIVATE /Zi)
target_compile_definitions("${PROJECT_NAME}" PRIVATE OBJECT_MEMORY_SIZE=8)
target_compile_definitions("${PROJECT_NAME}" PRIVATE DIRECTINPUT_VERSION=0x0800 CURL_STATICLIB _CRT_SECURE_NO_WARNINGS $<$<CONFIG:Debug>:_DEBUG> _USE_MATH_DEFINES)
target_link_libraries(
	"${PROJECT_NAME}"
	SokuLib
	d3dx9
	CharacterEngineLibrary
)
