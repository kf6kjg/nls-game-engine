# -*- cmake -*-

set(NLS_ENGINE_LIBRARY "CImg")

if(DEFINED NLS_ENGINE_LIBRARY_MODE)
	load_library_source_from_web(
		"${NLS_ENGINE_LIBRARY}"
		"${NLS_ENGINE_LIBRARY}" # no trailing slash
		"http://downloads.sourceforge.net/project/cimg/CImg-1.5.0_beta.zip"
		"fd7c4e257a448b7d72147598c4c9342e"
	)

	#* * * * * * * * * * * * * * * * * * * * *
	# Install header(s) in tree


	message("Making directory '${LIBS_INCLUDE_PATH}/cimg/'...")
	file(MAKE_DIRECTORY "${LIBS_INCLUDE_PATH}/cimg/")


	message("Copying CImg header...")
	execute_process(
		COMMAND "${CMAKE_COMMAND}" -E copy "${LIB_CImg_DIR}/CImg-1.5.0_beta/CImg.h" "${LIBS_INCLUDE_PATH}/cimg/"
		RESULT_VARIABLE COPY_RESULT
	)
	if(NOT "${COPY_RESULT}" MATCHES "^0")
		message(FATAL_ERROR "Copy of CImg header failed!")
	endif(NOT "${COPY_RESULT}" MATCHES "^0")
	message("Done.")

	set(CImg_FOUND 1)
endif(DEFINED NLS_ENGINE_LIBRARY_MODE)

if(EXISTS "${LIBS_INCLUDE_PATH}/cimg/")
	set(CImg_FOUND 1)
endif(EXISTS "${LIBS_INCLUDE_PATH}/cimg/")

