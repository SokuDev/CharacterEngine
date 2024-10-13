cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
set(PROJECT_NAME Tewi)
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
	Tewi.hpp
	Tewi.cpp
	TewiObjectFactory.cpp
	TewiObjectFactory.hpp
	objects/Hammer.cpp
	objects/Hammer.hpp
	objects/SpinningSmoke.cpp
	objects/SpinningSmoke.hpp
	objects/Rabbit.cpp
	objects/Rabbit.hpp
	objects/TaCRabbit.cpp
	objects/TaCRabbit.hpp
	objects/TrapHole.cpp
	objects/TrapHole.hpp
	objects/UnexpectedAssistanceObject/Reimu.cpp
	objects/UnexpectedAssistanceObject/Reimu.hpp
	objects/UnexpectedAssistanceObject/Marisa.cpp
	objects/UnexpectedAssistanceObject/Marisa.hpp
	objects/UnexpectedAssistanceObject/Sakuya.cpp
	objects/UnexpectedAssistanceObject/Sakuya.hpp
	objects/UnexpectedAssistanceObject/Alice.cpp
	objects/UnexpectedAssistanceObject/Alice.hpp
	objects/UnexpectedAssistanceObject/Patchouli.cpp
	objects/UnexpectedAssistanceObject/Patchouli.hpp
	objects/Bottle.cpp
	objects/Bottle.hpp
	objects/SpellTrapHole.cpp
	objects/SpellTrapHole.hpp
	objects/RabbitOrb.cpp
	objects/RabbitOrb.hpp
	objects/KillingRabbit.cpp
	objects/KillingRabbit.hpp
	objects/RabbitSolidarity.cpp
	objects/RabbitSolidarity.hpp
	objects/SetAndStrikeOrb.cpp
	objects/SetAndStrikeOrb.hpp
	objects/UBRabbit.cpp
	objects/UBRabbit.hpp
	objects/IllusionRabbit.cpp
	objects/IllusionRabbit.hpp
	objects/SubterraneanAnimism.cpp
	objects/SubterraneanAnimism.hpp
	objects/Bamboo.cpp
	objects/Bamboo.hpp
	objects/UnexpectedAssistanceObject/Youmu.cpp
	objects/UnexpectedAssistanceObject/Youmu.hpp
	objects/UnexpectedAssistanceObject/Remilia.cpp
	objects/UnexpectedAssistanceObject/Remilia.hpp
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