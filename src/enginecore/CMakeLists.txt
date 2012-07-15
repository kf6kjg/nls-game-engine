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
	"EngineCore.cpp"
	"EntityMap.cpp"
	"EntityRegister.cpp"
	"EventLoggerRegister.cpp"
	"main.cpp"
	"ModuleManager.cpp"
	"ScriptEngine.cpp"
	"ScriptExecutor.cpp"
	"ScriptMath.cpp"

	"${LIBS_INCLUDE_PATH}/EngineConfig.cpp"
)
set(HEADER_FILES
	# Specify all the header files that need to be displayed in the editor (in alphabetic order)
	"EngineCore.h"
	"EntityMap.h"
	"EventLoggerRegister.h"
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
set_property(TARGET ${NLS_ENGINE_EXECUTABLE_NAME} PROPERTY PROJECT_LABEL "0${NLS_ENGINE_EXECUTABLE_NAME}") # *HACK: Set the project label to be first in the list of VS projects so that it's the default for debugging.

# Specify dependencies
add_dependencies(${NLS_ENGINE_EXECUTABLE_NAME} "angelscript")
add_dependencies(${NLS_ENGINE_EXECUTABLE_NAME} "sharedbase")

if(WIN32)
	add_dependencies(${NLS_ENGINE_EXECUTABLE_NAME} "win32")
endif(WIN32)


#* * * * * * * * * * * * * * * * * * * * *

set(NLS_ENGINE_LIBS)

# OS-specific libraries
if(WIN32)
	set(NLS_ENGINE_LIBS		
		"${LIBRARY_OUTPUT_PATH}/sharedbase.lib"
		"${LIBRARY_OUTPUT_PATH}/angelscript.lib"
		"${LIBRARY_OUTPUT_PATH}/win32.lib"
		
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
else(WIN32)
	set(NLS_ENGINE_LIBS
		"${LIBRARY_OUTPUT_PATH}/libsharedbase.a"
		"${LIBRARY_OUTPUT_PATH}/libAngelScript.a"
		
		debug "${LIBRARY_OUTPUT_PATH}/libboost_chrono-mt-d.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_date_time-mt-d.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-mt-d.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_system-mt-d.a"
		debug "${LIBRARY_OUTPUT_PATH}/libboost_thread-mt-d.a"
		
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_chrono-mt.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_date_time-mt.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_filesystem-mt.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_system-mt.a"
		optimized "${LIBRARY_OUTPUT_PATH}/libboost_thread-mt.a"
	)
endif(WIN32)

if(NLS_ENGINE_LIBS)
	message("Adding to ${NLS_ENGINE_EXECUTABLE_NAME} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_EXECUTABLE_NAME} ${NLS_ENGINE_LIBS})
endif(NLS_ENGINE_LIBS)


message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
