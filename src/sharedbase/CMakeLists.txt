# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

## Configure the project
set(NLS_ENGINE_LIBRARY "sharedbase")

set(SOURCE_FILES
	# Specify all the cxx files that need to be compiled (in alphabetic order)
	"ComponentInterface.cpp"
	"Entity.cpp"
	"Envelope.cpp"
	"EventLogger.cpp"
	"MessageRouter.cpp"
	"OSInterface.cpp"
	"QuatMath.cpp"
)
set(HEADER_FILES
	# Specify all the header files that need to be displayed in the editor (in alphabetic order)
	"ComponentInterface.h"
	"Entity.h"
	"Entity_fwd.h"
	"Envelope.h"
	"Envelope_fwd.h"
	"EventLogger.h"
	"ModuleInterface.h"
	"ModuleScriptInterface.h"
	"OSInterface.h"
	"OSInterface_fwd.h"
	"QuatMath.h"
	"ScriptObjectInterface.h"
)

# Put the files into groups in the editor.
source_group("Source" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})

## Set up the project for compilation
message("Adding ${NLS_ENGINE_LIBRARY}...")

# Create the library (all files that should be shown in the editor have to be listed here)
add_library(${NLS_ENGINE_LIBRARY} STATIC ${SOURCE_FILES} ${HEADER_FILES})

#* * * * * * * * * * * * * * * * * * * * *

message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
