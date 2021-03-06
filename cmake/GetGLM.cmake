# -*- cmake -*-

if(DEFINED NLS_ENGINE_LIBRARY_MODE OR DEFINED NLS_ENGINE_BUILD_MODE)
	load_library_source_from_web(
		"GLM"
		"GLM" # no trailing slash
		"http://sourceforge.net/projects/ogl-math/files/glm-0.9.3.4/glm-0.9.3.4.zip"
		"a0cea0871bd39f2d628c12f1181faffb"
	)
endif(DEFINED NLS_ENGINE_LIBRARY_MODE OR DEFINED NLS_ENGINE_BUILD_MODE)
	
if(DEFINED NLS_ENGINE_LIBRARY_MODE)
	message("Making directory '${LIBS_INCLUDE_PATH}/glm/'...")
	file(MAKE_DIRECTORY "${LIBS_INCLUDE_PATH}/glm/")
	
	message("Copying GLM headers...")
	execute_process( # Execute process is used over file(COPY) as the latter has no documented error reporting facilities.
		COMMAND "${CMAKE_COMMAND}" -E copy_directory "${LIB_GLM_DIR}/glm-0.9.3.4/glm" "${LIBS_INCLUDE_PATH}/glm/glm"
		RESULT_VARIABLE COPY_RESULT
	)
	if(NOT "${COPY_RESULT}" MATCHES "^0")
		message(FATAL_ERROR "Copy of GLM headers failed!  Result: ${COPY_RESULT}")
	endif(NOT "${COPY_RESULT}" MATCHES "^0")
	
	file(
		COPY
			"${LIB_GLM_DIR}/glm-0.9.3.4/copying.txt"
		DESTINATION "${LIBS_INCLUDE_PATH}/glm/"
	)
	
	message("Copying GLM headers done.")
endif(DEFINED NLS_ENGINE_LIBRARY_MODE)

if(EXISTS "${LIBS_INCLUDE_PATH}/glm/glm/")
	if(DEFINED NLS_ENGINE_BUILD_MODE)
		message("Calling GLM CMake")
		add_subdirectory("${LIB_GLM_DIR}/glm-0.9.3.4/glm" "${LIB_GLM_DIR}/glm-0.9.3.4/glm")
		message("GLM CMake done.")
	endif(DEFINED NLS_ENGINE_BUILD_MODE)
	
	set(GLM_INCLUDE_DIRS "${LIBS_INCLUDE_PATH}/glm")
	set(GLM_FOUND 1)
endif(EXISTS "${LIBS_INCLUDE_PATH}/glm/glm/")

