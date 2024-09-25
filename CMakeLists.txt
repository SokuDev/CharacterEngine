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
	src/tewi/TewiObjectFactory.cpp
	src/tewi/TewiObjectFactory.hpp
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
	src/tewi/objects/Hammer.cpp
	src/tewi/objects/Hammer.hpp
	src/tewi/objects/SpinningSmoke.cpp
	src/tewi/objects/SpinningSmoke.hpp
	src/tewi/objects/Rabbit.cpp
	src/tewi/objects/Rabbit.hpp
	src/tewi/objects/TaCRabbit.cpp
	src/tewi/objects/TaCRabbit.hpp
	src/tewi/objects/TrapHole.cpp
	src/tewi/objects/TrapHole.hpp
	src/tewi/objects/UnexpectedAssistanceObject/Reimu.cpp
	src/tewi/objects/UnexpectedAssistanceObject/Reimu.hpp
	src/tewi/objects/UnexpectedAssistanceObject/Marisa.cpp
	src/tewi/objects/UnexpectedAssistanceObject/Marisa.hpp
	src/tewi/objects/UnexpectedAssistanceObject/Sakuya.cpp
	src/tewi/objects/UnexpectedAssistanceObject/Sakuya.hpp
	src/tewi/objects/UnexpectedAssistanceObject/Alice.cpp
	src/tewi/objects/UnexpectedAssistanceObject/Alice.hpp
	src/tewi/objects/UnexpectedAssistanceObject/Patchouli.cpp
	src/tewi/objects/UnexpectedAssistanceObject/Patchouli.hpp
	src/tewi/objects/Bottle.cpp
	src/tewi/objects/Bottle.hpp
	src/tewi/objects/SpellTrapHole.cpp
	src/tewi/objects/SpellTrapHole.hpp
	src/tewi/objects/RabbitOrb.cpp
	src/tewi/objects/RabbitOrb.hpp
	src/tewi/objects/KillingRabbit.cpp
	src/tewi/objects/KillingRabbit.hpp
	src/tewi/objects/RabbitSolidarity.cpp
	src/tewi/objects/RabbitSolidarity.hpp
	src/tewi/objects/SetAndStrikeOrb.cpp
	src/tewi/objects/SetAndStrikeOrb.hpp
	src/tewi/objects/UBRabbit.cpp
	src/tewi/objects/UBRabbit.hpp
	src/tewi/objects/IllusionRabbit.cpp
	src/tewi/objects/IllusionRabbit.hpp
)
target_compile_options("${PROJECT_NAME}" PRIVATE /Zi)
target_compile_definitions("${PROJECT_NAME}" PRIVATE DIRECTINPUT_VERSION=0x0800 CURL_STATICLIB _CRT_SECURE_NO_WARNINGS $<$<CONFIG:Debug>:_DEBUG> OBJECT_MEMORY_SIZE=8 _USE_MATH_DEFINES)
target_include_directories("${PROJECT_NAME}" PRIVATE include SokuLib/directx "src/Common/")
target_link_directories("${PROJECT_NAME}" PRIVATE lib)
target_link_libraries(
        "${PROJECT_NAME}"
        SokuLib
        shlwapi
        d3d9
        d3dx9
)