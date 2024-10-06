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
        src/Characters/tewi/Tewi.hpp
        src/Characters/tewi/Tewi.cpp
	src/Characters/tewi/TewiObjectFactory.cpp
	src/Characters/tewi/TewiObjectFactory.hpp
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
	src/Characters/tewi/objects/Hammer.cpp
	src/Characters/tewi/objects/Hammer.hpp
	src/Characters/tewi/objects/SpinningSmoke.cpp
	src/Characters/tewi/objects/SpinningSmoke.hpp
	src/Characters/tewi/objects/Rabbit.cpp
	src/Characters/tewi/objects/Rabbit.hpp
	src/Characters/tewi/objects/TaCRabbit.cpp
	src/Characters/tewi/objects/TaCRabbit.hpp
	src/Characters/tewi/objects/TrapHole.cpp
	src/Characters/tewi/objects/TrapHole.hpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Reimu.cpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Reimu.hpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Marisa.cpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Marisa.hpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Sakuya.cpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Sakuya.hpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Alice.cpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Alice.hpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Patchouli.cpp
	src/Characters/tewi/objects/UnexpectedAssistanceObject/Patchouli.hpp
	src/Characters/tewi/objects/Bottle.cpp
	src/Characters/tewi/objects/Bottle.hpp
	src/Characters/tewi/objects/SpellTrapHole.cpp
	src/Characters/tewi/objects/SpellTrapHole.hpp
	src/Characters/tewi/objects/RabbitOrb.cpp
	src/Characters/tewi/objects/RabbitOrb.hpp
	src/Characters/tewi/objects/KillingRabbit.cpp
	src/Characters/tewi/objects/KillingRabbit.hpp
	src/Characters/tewi/objects/RabbitSolidarity.cpp
	src/Characters/tewi/objects/RabbitSolidarity.hpp
	src/Characters/tewi/objects/SetAndStrikeOrb.cpp
	src/Characters/tewi/objects/SetAndStrikeOrb.hpp
	src/Characters/tewi/objects/UBRabbit.cpp
	src/Characters/tewi/objects/UBRabbit.hpp
	src/Characters/tewi/objects/IllusionRabbit.cpp
	src/Characters/tewi/objects/IllusionRabbit.hpp
	src/Characters/tewi/objects/SubterraneanAnimism.cpp
	src/Characters/tewi/objects/SubterraneanAnimism.hpp
	src/Characters/tewi/objects/Bamboo.cpp
	src/Characters/tewi/objects/Bamboo.hpp
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