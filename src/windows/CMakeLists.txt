# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

## Configure the project
set(NLS_ENGINE_NATIVEAPP "win32")

set(SOURCE_FILES
	# Specify all the cxx files that need to be compiled (in alphabetic order)
	"win32.cpp"
	"main.cpp"
)
set(HEADER_FILES
	# Specify all the header files that need to be displayed in the editor (in alphabetic order)
	"win32.h"
)

# Put the files into groups in the editor.
source_group("Source" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

## Set up the project for compilation
message("Adding ${NLS_ENGINE_NATIVEAPP}...")

# Create the library (all files that should be shown in the editor have to be listed here)
add_executable(${NLS_ENGINE_NATIVEAPP} WIN32 ${SOURCE_FILES} ${HEADER_FILES})
set_property(TARGET ${NLS_ENGINE_NATIVEAPP} PROPERTY PROJECT_LABEL ".${NLS_ENGINE_NATIVEAPP}") # *HACK: Set the project label to be first in the list of VS projects so that it's the default for debugging.

# Specify dependencies
add_dependencies(${NLS_ENGINE_NATIVEAPP} "enginecore")

set(NLS_ENGINE_LIBS
	"${LIBRARY_OUTPUT_PATH}/enginecore.lib"
	"${LIBRARY_OUTPUT_PATH}/sharedbase.lib"
	"${LIBRARY_OUTPUT_PATH}/angelscript.lib"
	
	debug "${LIBRARY_OUTPUT_PATH}/libboost_chrono-mt-gd.lib"
	debug "${LIBRARY_OUTPUT_PATH}/libboost_date_time-mt-gd.lib"
	debug "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-mt-gd.lib"
	debug "${LIBRARY_OUTPUT_PATH}/libboost_system-mt-gd.lib"
	debug "${LIBRARY_OUTPUT_PATH}/libboost_thread-mt-gd.lib"
	
	optimized "${LIBRARY_OUTPUT_PATH}/libboost_chrono-mt.lib"
	optimized "${LIBRARY_OUTPUT_PATH}/libboost_date_time-mt.lib"
	optimized "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-mt.lib"
	optimized "${LIBRARY_OUTPUT_PATH}/libboost_system-mt.lib"
	optimized "${LIBRARY_OUTPUT_PATH}/libboost_thread-mt.lib"
)

if(NLS_ENGINE_LIBS)
	message("Adding to ${NLS_ENGINE_NATIVEAPP} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_NATIVEAPP} ${NLS_ENGINE_LIBS})
endif(NLS_ENGINE_LIBS)

#* * * * * * * * * * * * * * * * * * * * *

message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
