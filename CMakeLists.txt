cmake_minimum_required(VERSION 3.21)

########################################################################################################################
## Define project
########################################################################################################################

project(
	InfinityUI
	VERSION 1.0
	LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

set(headers
	include/RE/G/GFxValue.h
	include/utils/GFxArray.h
	include/utils/GFxDisplayObject.h
	include/utils/GFxFunction.h
	include/utils/GFxObject.h
	include/utils/GFxLoggers.h
	include/utils/INISettingCollection.h
	include/utils/Logger.h
	include/utils/Setting.h
	include/utils/Trampoline.h
	include/utils/type_name.h
	include/API.h
	include/FullAPI.h
	include/GFxMoviePatcher.h
	include/Hooks.h
	include/pch.h
	include/Settings.h
)

set(sources
	source/RE/G/GFxValue.cpp
	source/utils/INISettingCollection.cpp
	source/utils/Logger.cpp
	source/GFxMoviePatcher.cpp
	source/Hooks.cpp
	source/main.cpp
	source/pch.cpp
	source/Settings.cpp
)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

########################################################################################################################
## Configure target DLL
########################################################################################################################
find_package(directxtk CONFIG REQUIRED)
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(binary_io REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		CommonLibSSE::CommonLibSSE
		spdlog::spdlog
)

if (MSVC)
	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF>"
	)
endif()

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		include/PCH.h
)

########################################################################################################################
## Automatic plugin deployment
########################################################################################################################

set(DEPLOYMENT_DIR "C:/Program Files (x86)/Steam/steamapps/common/Skyrim Special Edition")
set(DEPLOYMENT_DIR_AE "C:/Program Files (x86)/Steam/steamapps/common/Skyrim Anniversary Edition")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>
	"${DEPLOYMENT_DIR}/Data/SKSE/Plugins/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>
	"${DEPLOYMENT_DIR_AE}/Data/SKSE/Plugins/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
)