# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

## Configure the project
if (NOT DEFINED NLS_ENGINE_EXECUTABLE_NAME)
	set(NLS_ENGINE_EXECUTABLE_NAME "nlsengine" CACHE STRING
		"Name of the compiled executable"
	)
endif (NOT DEFINED NLS_ENGINE_EXECUTABLE_NAME)

set(NLS_ENGINE_LIBRARY "enginecore")

set(SOURCE_FILES
	# Specify all the cxx files that need to be compiled (in alphabetic order)
	"EngineCore.cpp"
	"EntityMap.cpp"
	"EntityRegister.cpp"
	"EventLoggerRegister.cpp"
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
message("Adding ${NLS_ENGINE_LIBRARY}...")

# Create the executable (all files that should be shown in the editor have to be listed here)
add_library(${NLS_ENGINE_LIBRARY} STATIC ${SOURCE_FILES} ${HEADER_FILES})

# Specify dependencies
add_dependencies(${NLS_ENGINE_LIBRARY} "angelscript")
add_dependencies(${NLS_ENGINE_LIBRARY} "sharedbase")

#* * * * * * * * * * * * * * * * * * * * *

set(NLS_ENGINE_LIBS)

# OS-specific libraries
if(WIN32)
	# *NOTE: We no longer need to link boost in here as we are building a static library. THe final build will need to link in boost however.
	set(NLS_ENGINE_LIBS
		#"${LIBRARY_OUTPUT_PATH}/sharedbase.lib"
		#"${LIBRARY_OUTPUT_PATH}/angelscript.lib"
	)
else(WIN32)
	set(NLS_ENGINE_LIBS
		#"${LIBRARY_OUTPUT_PATH}/libsharedbase.a"
		#"${LIBRARY_OUTPUT_PATH}/libAngelScript.a"
	)
endif(WIN32)

if(NLS_ENGINE_LIBS)
	message("Adding to ${NLS_ENGINE_LIBRARY} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_LIBRARY} ${NLS_ENGINE_LIBS})
endif(NLS_ENGINE_LIBS)


message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
