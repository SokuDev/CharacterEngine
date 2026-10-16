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

find_program(
	SHADY_CLI shady-cli
	PATHS "${CMAKE_SOURCE_DIR}/shady"
)
# Signature: add_soku_assets(<TARGET_NAME> OUTPUT <BINARY_NAME> <FILES...> [ROOT <ROOT_DIR>])
function(add_soku_assets TARGET_NAME)
	# Parse arguments. OUTPUT and ROOT are single-value arguments.
	cmake_parse_arguments(ARG "" "OUTPUT;ROOT" "" ${ARGN})

	# Error checking for the new required OUTPUT argument
	if (NOT ARG_OUTPUT)
		message(FATAL_ERROR "add_soku_assets: Missing required argument 'OUTPUT'")
	endif ()

	# Automatically append .dat to the output binary if it isn't there
	if (NOT ARG_OUTPUT MATCHES "\\.dat$")
		set(ARG_OUTPUT "${ARG_OUTPUT}.dat")
	endif ()

	set(PROCESSING_OUTPUTS "")
	set(OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}")

	if (ARG_ROOT)
		get_filename_component(ARG_ROOT "${ARG_ROOT}" ABSOLUTE)
	endif ()

	foreach(SRC_FILE ${ARG_UNPARSED_ARGUMENTS})
		get_filename_component(ABS_SRC_FILE "${SRC_FILE}" ABSOLUTE)
		get_filename_component(FILE_EXT "${SRC_FILE}" EXT)

		if (ARG_ROOT)
			file(RELATIVE_PATH REL_FILE "${ARG_ROOT}" "${ABS_SRC_FILE}")
		else ()
			file(RELATIVE_PATH REL_FILE "${CMAKE_CURRENT_SOURCE_DIR}" "${ABS_SRC_FILE}")
		endif ()

		get_filename_component(REL_DIR "${REL_FILE}" DIRECTORY)

		# Dynamic Extension Mapping
		if (FILE_EXT STREQUAL ".xml")
			file(STRINGS "${ABS_SRC_FILE}" XML_HEAD LIMIT_COUNT 5)
			# In shady-cli, extensions depends on if the XML is a layout or a pattern.
			# Check the first tag to find out which it is.
			if ("${XML_HEAD}" MATCHES "<layout")
				set(OUT_EXT ".dat")
			else ()
				set(OUT_EXT ".pat")
			endif ()
		elseif (FILE_EXT STREQUAL ".txt")
			set(OUT_EXT ".cv0")
		elseif (FILE_EXT STREQUAL ".csv")
			set(OUT_EXT ".cv1")
		elseif (FILE_EXT STREQUAL ".png")
			set(OUT_EXT ".cv2")
		elseif (FILE_EXT STREQUAL ".wav")
			set(OUT_EXT ".cv3")
		elseif (FILE_EXT STREQUAL ".act")
			set(OUT_EXT ".pal")
		else ()
			set(OUT_EXT "${FILE_EXT}")
		endif ()

		get_filename_component(FILE_WE "${SRC_FILE}" NAME_WE)

		set(CONVERTED_FILE "${OUT_DIR}/${REL_DIR}/${FILE_WE}${OUT_EXT}")
		list(APPEND PROCESSING_OUTPUTS "${CONVERTED_FILE}")

		file(MAKE_DIRECTORY "${OUT_DIR}/${REL_DIR}")
		if (FILE_EXT STREQUAL "${OUT_EXT}")
			add_custom_command(
				OUTPUT "${CONVERTED_FILE}"
				COMMAND "${CMAKE_COMMAND}" -E copy "${ABS_SRC_FILE}" "${OUT_DIR}/${REL_DIR}"
				DEPENDS "${ABS_SRC_FILE}"
				COMMENT "Copy asset file ${REL_DIR}/${FILE_WE}${OUT_EXT}"
				VERBATIM
			)
		else ()
			add_custom_command(
				OUTPUT "${CONVERTED_FILE}"
				COMMAND "${SHADY_CLI}" convert "${ABS_SRC_FILE}" -o "${OUT_DIR}/${REL_DIR}" > /dev/null
				DEPENDS "${ABS_SRC_FILE}"
				COMMENT "Converting asset file ${REL_DIR}/${FILE_WE}${OUT_EXT}"
				VERBATIM
			)
		endif ()
	endforeach()

	set(FINAL_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${ARG_OUTPUT}")
	add_custom_command(
		OUTPUT "${FINAL_BINARY}"
		COMMAND "${SHADY_CLI}" pack -m data "${OUT_DIR}" -o "${FINAL_BINARY}" > /dev/null
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		DEPENDS ${PROCESSING_OUTPUTS}
		COMMENT "Packing assets into ${ARG_OUTPUT}"
		VERBATIM
	)

	# The target name and file dependency are now cleanly split
	add_custom_target(${TARGET_NAME} ALL DEPENDS "${FINAL_BINARY}")
endfunction()
function(target_character_assets TARGET_NAME)
	string(TOLOWER ${TARGET_NAME} OUTPUT_NAME)
	file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "data/*")
	add_soku_assets("${OUTPUT_NAME}_assets" OUTPUT "${OUTPUT_NAME}" ${ASSETS})
	add_dependencies("${TARGET_NAME}" "${OUTPUT_NAME}_assets")
endfunction()

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
	src/CharacterLoader/CharacterEngineABI.cpp
	src/CharacterLoader/CharacterEngineABI.hpp
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
	src/Common/CharacterEngine.hpp
	src/Common/CharacterEngineABI.cpp
	src/Common/CharacterEngineABI.hpp
	src/Common/Soku2Patches.inl
	src/Common/Soku2Patches.hpp
	src/Common/StackTrace.hpp
)
target_link_libraries(
	CharacterEngineLibrary
	SokuLib
)
target_include_directories(
	CharacterEngineLibrary PUBLIC
	src/Common
)
add_custom_target(Characters)
add_custom_target(All)
add_dependencies(All CharacterEngine Soku2Loader common_assets Characters)

file(GLOB V_GLOB CONFIGURE_DEPENDS LIST_DIRECTORIES true "src/Characters/*")
foreach(item ${V_GLOB})
	if (IS_DIRECTORY ${item})
		add_subdirectory(${item})
	endif ()
endforeach()

file(GLOB_RECURSE COMMON_ASSETS CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "lib/data/*")
add_soku_assets(common_assets OUTPUT common.dat ${COMMON_ASSETS} ROOT lib)
