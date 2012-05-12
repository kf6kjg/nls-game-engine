# -*- cmake -*-

message("Entering ${CMAKE_CURRENT_SOURCE_DIR}/")

set(NLS_ENGINE_MODULE "GraphicsDLL")

if(WIN32)
	message("Adding ${NLS_ENGINE_MODULE}...")
	
	# get all the cxx files that need to be compiled
	file(GLOB ${CMAKE_CURRENT_SOURCE_DIR}_SOURCE
		"*.cpp"
	)
	
	add_library(${NLS_ENGINE_MODULE} SHARED ${${CMAKE_CURRENT_SOURCE_DIR}_SOURCE}) 
	
	add_dependencies(${NLS_ENGINE_MODULE} sharedbase)
	
	set(NLS_ENGINE_LIBS
		${DIRECTX_LIBRARY_DIR}/d3d9.lib
		${DIRECTX_LIBRARY_DIR}/d3dx9.lib
		${DIRECTX_LIBRARY_DIR}/dxerr.lib
		${LIBRARY_OUTPUT_PATH}/Debug/sharedbase.lib
		${LIBS}
	)
	
	message("Adding to ${NLS_ENGINE_MODULE} the libraries: ${NLS_ENGINE_LIBS}")
	target_link_libraries(${NLS_ENGINE_MODULE} ${NLS_ENGINE_LIBS})
	
else(WIN32)
	message("Skipping ${NLS_ENGINE_MODULE}...")
endif(WIN32)

message("Exiting ${CMAKE_CURRENT_SOURCE_DIR}/")
