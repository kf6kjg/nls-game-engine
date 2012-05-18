# -*- cmake -*-

# The name of our project is "NLS_ENGINE". CMakeLists files in this project can
# refer to the root source directory of the project as ${NLS_ENGINE_SOURCE_DIR} and
# to the root binary directory of the project as ${NLS_ENGINE_BINARY_DIR}.
cmake_minimum_required(VERSION 2.8.7)
project(NLS_ENGINE)

## Executable name
set(NLS_ENGINE_EXECUTABLE_NAME "nlsengine" CACHE STRING
	"Specify the name of the resulting engine executable."
)

## Commandline configuration (-D<name>:<type>=<value>)
	#! Task selection (generate engine build; generate docs; generate, build, and install libraries) are accomplished by selecting different folders when calling cmake.
	
	# Engine Modules
	if(NOT DEFINED ENGINE_MODULES)
		#! ENGINE_MODULES is a LIST of the module names (same as the folders in which the modules are located).
		
		set(ENGINE_MODULES "SoundSimple;PhysicsSimple" CACHE STRING
			"Choose your engine modules, each based on the name of the containing folder, seperated by semicolons."
		)
	endif(NOT DEFINED ENGINE_MODULES)
	
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
			"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
			FORCE)
	endif(NOT CMAKE_BUILD_TYPE)
# /Commandline configuration


## Set the directory for custom CMake modules
if(NOT DEFINED NLS_ENGINE_ROOT) # This directory was called directly
	set(NLS_ENGINE_ROOT "${CMAKE_SOURCE_DIR}/..")
	
	# Set the directory for custom CMake modules
	set(CMAKE_MODULE_PATH "${NLS_ENGINE_ROOT}/cmake")
endif(NOT DEFINED NLS_ENGINE_ROOT)

include(Version)
include(Variables)

set(NLS_ENGINE_BUILD_MODE 1)

# Load up libraries
include(GetAngelScript)
include(GetGLM)
include(GetBoost)


## Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${NLS_ENGINE_ROOT}/include/EngineConfig.h.in"
	"${NLS_ENGINE_ROOT}/include/EngineConfig.h"
)


## Setup compilation

# Add the "include" directory to the includes
include_directories("../include")



## Go find AngelScript
if(AngelScript_FOUND)
	message("Including AngelScript")
	include_directories(${AngelScript_INCLUDE_DIRS})
else(AngelScript_FOUND)
	# AngelScript was not found, cease processing
	message(FATAL_ERROR "Cannot continue without AngelScript.")
endif(AngelScript_FOUND)


## Go find Boost
if(Boost_FOUND)
	message("Including Boost")
	include_directories(${Boost_INCLUDE_DIRS})
else(Boost_FOUND)
	# Boost was not found, cease processing
	message(FATAL_ERROR "Cannot continue without Boost.")
endif(Boost_FOUND)


## Include GLM
if(GLM_FOUND)
	message("Including GLM")
	include_directories(${GLM_INCLUDE_DIRS})
else(GLM_FOUND)
	message(FATAL_ERROR "Cannot continue without GLM.")
endif(GLM_FOUND)


## Include DirectX 9 as currently EVERYTHING requires it.
if(WIN32)
	message("Including DX9")
	find_package(DirectX REQUIRED)
	include_directories(${DIRECTX_INCLUDE_DIR})
endif(WIN32)


## Add the shared base directory for compilation
add_subdirectory("sharedbase")

## Add the shared base files to the include path
include_directories("sharedbase")


## Add the primary program directory
add_subdirectory("enginecore")

## Go get the list of available modules
include(AvailableModules)

## Add selected modules
# *TODO: Clone selected optional modules into the src/modules/ folder.
foreach(MODULE ${ENGINE_MODULES})
	message("Adding the following module: ${MODULE}")
	
	set(MODULE_FOLDER "${NLS_ENGINE_ROOT}/src/${MODULE}")
	
	if(IS_DIRECTORY "${MODULE_FOLDER}")
		if(EXISTS "${MODULE_FOLDER}/CMakeLists.txt")
			add_subdirectory(${MODULE_FOLDER})
		else(EXISTS "${MODULE_FOLDER}/CMakeLists.txt")
			message(WARNING "ERR: Unable to configure module - no CMakeLists.txt in '${MODULE_FOLDER}'")
		endif(EXISTS "${MODULE_FOLDER}/CMakeLists.txt")
	else(IS_DIRECTORY "${MODULE_FOLDER}")
		message(WARNING "Module '${MODULE}' (expected to be located at '${MODULE_FOLDER}') either does not exist, or is not a folder!")
	endif(IS_DIRECTORY "${MODULE_FOLDER}")
endforeach(MODULE)
