# -*- cmake -*-


# Returns:
#  LIB_${LIB_NAME}_DIR - the directory into which the source was installed.
#  LIB_ERR - the error string - if not set, then no problems were found.
#  LIB_RESULT - the result code of the last thing to process - useful when LIB_ERR is set.
function("load_library_source_from_web" LIB_NAME LIB_DIR LIB_URL LIB_MD5)
	# Create local variables
	get_filename_component(LIB_ARCHIVE ${LIB_URL} NAME) # Extract the file name and prepend the download path
	set(LIB_ARCHIVE "${LIBS_DOWNLOAD_PATH}/${LIB_ARCHIVE}")
	set(LIB_INSTALL_DIR "${LIBS_SOURCE_PATH}/${LIB_DIR}")
	
	# These two error reports are not unset
	set(LIB_ERR "")
	set(LIB_RESULT "")
	
	# Don't download if already downloaded
	if(EXISTS "${LIB_ARCHIVE}")
		message("Not redownloading library '${LIB_NAME}' archive.")
	else(EXISTS "${LIB_ARCHIVE}")
		# Download the library
		
		message("Downloading library '${LIB_NAME}' archive...")
		file(DOWNLOAD ${LIB_URL} ${LIB_ARCHIVE} STATUS LIB_RESULT SHOW_PROGRESS)
		
		message("Download of library '${LIB_NAME}' archive complete.")
		
		# If we had to re-download, likelyhood is that we need to decompress.
		set(LIB_${LIB_NAME}_ALREADY_EXTRACTED 0 FORCE)
	endif(EXISTS "${LIB_ARCHIVE}")
	
	# Verify MD5 of D/L'd archive in case it got corrupted
	file(MD5 "${LIB_ARCHIVE}" FILE_MD5)
	
	if(NOT "${FILE_MD5}" STREQUAL "${LIB_MD5}")
		file(REMOVE "${LIB_ARCHIVE}")
		message(FATAL_ERROR "Library archive failed verification: the expected MD5 sum did not match to received value of '${FILE_MD5}'.  Removing library and aborting.  Please re-run CMake.")
	endif(NOT "${FILE_MD5}" STREQUAL "${LIB_MD5}")
	
	# Prepare the directory
	execute_process(
		COMMAND "${CMAKE_COMMAND}" -E make_directory "${LIB_INSTALL_DIR}"
	)
	
	if("${LIB_${LIB_NAME}_ALREADY_EXTRACTED}")
		message("Library '${LIB_NAME}' has indicated that it already has been extracted, set LIB_${LIB_NAME}_ALREADY_EXTRACTED to off/false to force.")
	else("${LIB_${LIB_NAME}_ALREADY_EXTRACTED}")
		# Extract the library
		message("Extracting library '${LIB_NAME}' archive...")
		execute_process(
			COMMAND "${CMAKE_COMMAND}" -E tar xf "${LIB_ARCHIVE}"
			WORKING_DIRECTORY "${LIB_INSTALL_DIR}"
			RESULT_VARIABLE LIB_RESULT
		)
		
		if(NOT ${LIB_RESULT} MATCHES "^0")
			message(FATAL_ERROR "Extraction of '${LIB_ARCHIVE}' archive to '${LIB_INSTALL_DIR}' failed with result: '${LIB_RESULT}'")
		endif(NOT ${LIB_RESULT} MATCHES "^0")
		message("Extraction of library '${LIB_NAME}' archive complete.")
		
		set(LIB_${LIB_NAME}_ALREADY_EXTRACTED 1 CACHE BOOL
			"Library has already been extracted.  True will prevent the library from being extracted again, while false will force a re-extraction of the library from disk."
			FORCE
		)
	endif("${LIB_${LIB_NAME}_ALREADY_EXTRACTED}")
	
	# Create exported variables
	set(LIB_${LIB_NAME}_DIR "${LIB_INSTALL_DIR}" PARENT_SCOPE)
	
	unset(LIB_ARCHIVE)
	unset(LIB_INSTALL_DIR)
endfunction("load_library_source_from_web")

