# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

set(NLS_ENGINE_MODULE "ScriptDLL")

message("Adding ${NLS_ENGINE_MODULE}...")

# get all the cxx files that need to be compiled
file(GLOB ${CMAKE_CURRENT_SOURCE_DIR}_SOURCE
	"*.cpp"
)

add_library(${NLS_ENGINE_MODULE} SHARED ${${CMAKE_CURRENT_SOURCE_DIR}_SOURCE}) 

add_dependencies(${NLS_ENGINE_MODULE} AngelscriptLIB sharedbase)

#* * * * * * * * * * * * * * * * * * * * *

set(NLS_ENGINE_LIBS)

# OS-specific libraries
if(WIN32)
	#*HACK: The proper way to include the custom libs would be to create a finder cmake module for it.
	set(NLS_ENGINE_LIBS
		${DIRECTX_LIBRARY_DIR}/d3d9.lib
		${DIRECTX_LIBRARY_DIR}/d3dx9.lib
		${LIBRARY_OUTPUT_PATH}/Debug/AngelscriptLIB.lib
		${LIBRARY_OUTPUT_PATH}/Debug/sharedbase.lib
		${NLS_ENGINE_LIBS}
	)
endif(WIN32)

# General libraries
set(NLS_ENGINE_LIBS
	${NLS_ENGINE_LIBS}
	${LIBS}
)


if(NLS_ENGINE_LIBS)
	message("Adding to ${NLS_ENGINE_MODULE} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_MODULE} ${NLS_ENGINE_LIBS})
endif(NLS_ENGINE_LIBS)

message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
