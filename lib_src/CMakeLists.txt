# -*- cmake -*-

# The name of our project is "NLS_ENGINE". CMakeLists files in this project can 
# refer to the root source directory of the project as ${NLS_ENGINE_SOURCE_DIR} and 
# to the root binary directory of the project as ${NLS_ENGINE_BINARY_DIR}. 
cmake_minimum_required(VERSION 2.8.7) 
project(NLS_ENGINE) 


if(NOT DEFINED NLS_ENGINE_ROOT) # This directory was called directly
	set(NLS_ENGINE_ROOT "${CMAKE_SOURCE_DIR}/..")
	
	# Set the directory for custom CMake modules
	set(CMAKE_MODULE_PATH "${NLS_ENGINE_ROOT}/cmake")
	
	# Redirect output directories one up.
	set(CMAKE_SOURCE_DIR "${NLS_ENGINE_ROOT}/..")
	set(PROJECT_BINARY_DIR "${NLS_ENGINE_ROOT}/..")
endif(NOT DEFINED NLS_ENGINE_ROOT)

include(Version)
include(Variables)

set(NLS_ENGINE_LIBRARY_MODE 1)

## Load libraries
	# Start the library loading process
	include(LoadLibraries)
	
	# Load up libraries
	include(GetAngelScript)
	include(GetBoost)
## /Load libraries
