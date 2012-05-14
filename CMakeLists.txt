# -*- cmake -*-

# The name of our project is "NLS_ENGINE". CMakeLists files in this project can
# refer to the root source directory of the project as ${NLS_ENGINE_SOURCE_DIR} and
# to the root binary directory of the project as ${NLS_ENGINE_BINARY_DIR}.
cmake_minimum_required(VERSION 2.8.7)
project(NLS_ENGINE)

message("NOTE: Calling CMake from this directory rebuilds EVERYTHING.  If you want to rebuild ONLY the source, execute CMake in the src directory.")

## Configure paths
	set(NLS_ENGINE_ROOT "${CMAKE_SOURCE_DIR}")
	
	# Set the directory for custom CMake modules
	set(CMAKE_MODULE_PATH "${NLS_ENGINE_ROOT}/cmake")
## /Configure paths


# Create libraries
add_subdirectory("lib_src")

# Compile source
add_subdirectory("src")

# Generate documentation
add_subdirectory("docs")
