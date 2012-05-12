# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

## Configure the project
if (NOT DEFINED NLS_ENGINE_EXECUTABLE_NAME)
	set(NLS_ENGINE_EXECUTABLE_NAME "nlsengine" CACHE STRING
		"Name of the compiled executable"
	)
endif (NOT DEFINED NLS_ENGINE_EXECUTABLE_NAME)

set(SOURCE_FILES
	# Specify all the cxx files that need to be compiled (in alphabetic order)
	"D3DXMathTypesRegister.cpp"
	"EntityList.cpp"
	"EntityRegister.cpp"
	"main.cpp"
	"ModuleManager.cpp"
	"ScriptEngine.cpp"
	"ScriptExecutor.cpp"

	"${LIBS_INCLUDE_PATH}/EngineConfig.cpp"
)
set(HEADER_FILES
	# Specify all the header files that need to be displayed in the editor (in alphabetic order)
	"EntityList.h"
	"ModuleManager.h"
	"ScriptEngine.h"
	"ScriptExecutor.h"
	"sptrtypes.h"

	"${LIBS_INCLUDE_PATH}/EngineConfig.h"
)

# Put the files into groups in the editor.
source_group("Source" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

## Set up the project for compilation
message("Adding ${NLS_ENGINE_EXECUTABLE_NAME}...")

# Create the executable (all files that should be shown in the editor have to be listed here)
add_executable(${NLS_ENGINE_EXECUTABLE_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Specify dependencies
add_dependencies(${NLS_ENGINE_EXECUTABLE_NAME} "angelscript")
add_dependencies(${NLS_ENGINE_EXECUTABLE_NAME} "sharedbase")

#* * * * * * * * * * * * * * * * * * * * *

set(NLS_ENGINE_LIBS)

# OS-specific libraries
if(WIN32)
	set(NLS_ENGINE_LIBS
		"${DIRECTX_LIBRARY_DIR}/d3d9.lib"
		"${DIRECTX_LIBRARY_DIR}/d3dx9.lib"
		
		"${LIBRARY_OUTPUT_PATH}/sharedbase.lib"
		"${LIBRARY_OUTPUT_PATH}/angelscript.lib"
		
		debug "${LIBRARY_OUTPUT_PATH}/libboost_chrono-vc100-mt-gd-1_47.lib"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_date_time-vc100-mt-gd-1_47.lib"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-vc100-mt-gd-1_47.lib"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_system-vc100-mt-gd-1_47.lib"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_thread-vc100-mt-gd-1_47.lib"
		
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_chrono-vc100-mt-1_47.lib"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_date_time-vc100-mt-1_47.lib"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-vc100-mt-1_47.lib"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_system-vc100-mt-1_47.lib"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_thread-vc100-mt-1_47.lib"
	)
else(WIN32)
	set(NLS_ENGINE_LIBS
		"${LIBRARY_OUTPUT_PATH}/sharedbase.a"
		"${LIBRARY_OUTPUT_PATH}/angelscript.a"
		
		debug "${LIBRARY_OUTPUT_PATH}/libboost_chrono-vc100-mt-gd-1_47.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_date_time-vc100-mt-gd-1_47.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-vc100-mt-gd-1_47.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_system-vc100-mt-gd-1_47.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_thread-vc100-mt-gd-1_47.a"
		
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_chrono-vc100-mt-1_47.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_date_time-vc100-mt-1_47.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-vc100-mt-1_47.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_system-vc100-mt-1_47.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_thread-vc100-mt-1_47.a"
	)
endif(WIN32)

if(NLS_ENGINE_LIBS)
	message("Adding to ${NLS_ENGINE_EXECUTABLE_NAME} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_EXECUTABLE_NAME} ${NLS_ENGINE_LIBS})
endif(NLS_ENGINE_LIBS)


message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
