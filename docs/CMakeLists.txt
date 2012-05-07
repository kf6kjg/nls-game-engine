# -*- cmake -*-

cmake_minimum_required(VERSION 2.8) 
project(NLS_ENGINE) 

if(NOT DEFINED NLS_ENGINE_ROOT) # This directory was called directly
	set(NLS_ENGINE_ROOT "${CMAKE_SOURCE_DIR}/..")
	
	# Set the directory for custom CMake modules
	set(CMAKE_MODULE_PATH "${NLS_ENGINE_ROOT}/cmake")
endif(NOT DEFINED NLS_ENGINE_ROOT)

include(Version)
include(Variables)


## Prep the Doxyfile
configure_file (
	"${NLS_ENGINE_ROOT}/Doxyfile.in"
	"${NLS_ENGINE_ROOT}/Doxyfile"
)


## Find Doxygen
find_program(DOXYGEN_EXECUTABLE
	NAMES doxygen
	PATHS
		"${NLS_ENGINE_ROOT}"
)
mark_as_advanced(FORCE DOXYGEN_EXECUTABLE)

if(${DOXYGEN_EXECUTABLE} STREQUAL "DOXYGEN_EXECUTABLE-NOTFOUND")
	message("*** ERROR: Doxygen executable not found, unable to do the requested task of generating the documentation. ***")
	return()
endif(${DOXYGEN_EXECUTABLE} STREQUAL "DOXYGEN_EXECUTABLE-NOTFOUND")


## Build the documentation.
message("*** Building Documentation... ***")

execute_process(
	COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
	WORKING_DIRECTORY "${NLS_ENGINE_ROOT}"
	RESULT_VARIABLE DOXYGEN_RESULT
)

if(NOT ${DOXYGEN_RESULT} EQUAL 0)
	message("Doxygen Error: ${DOXYGEN_RESULT}")
endif(NOT ${DOXYGEN_RESULT} EQUAL 0)
