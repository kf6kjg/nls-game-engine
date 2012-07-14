# -*- cmake -*-
#
# Definitions of variables used throughout the NLS Engine build process.
#
# Platform variables:
#
#   DARWIN  - Mac OS X
#   LINUX   - Linux
#   WINDOWS - Windows
#


# Relative and absolute paths to subtrees.

# Set up the destinations for the compiled output
set(EXECUTABLE_OUTPUT_PATH ${NLS_ENGINE_ROOT}/bin)
set(LIBRARY_OUTPUT_PATH ${NLS_ENGINE_ROOT}/lib)

set(LIBS_DOWNLOAD_PATH ${NLS_ENGINE_ROOT}/libraries CACHE PATH
	"Location of library source archives.")
mark_as_advanced(FORCE LIBS_DOWNLOAD_PATH)

set(LIBS_SOURCE_PATH ${NLS_ENGINE_ROOT}/lib_src CACHE PATH
	"Location of prebuilt libraries' sourcecode.")
mark_as_advanced(FORCE LIBS_SOURCE_PATH)

set(LIBS_INCLUDE_PATH ${NLS_ENGINE_ROOT}/include CACHE PATH
	"Location of prebuilt libraries' header files.")
mark_as_advanced(FORCE LIBS_INCLUDE_PATH)

set(LIBS_BINARY_PATH ${NLS_ENGINE_ROOT}/lib CACHE PATH
	"Location of prebuilt libraries.")
mark_as_advanced(FORCE LIBS_BINARY_PATH)


## Correct the build output location
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
# Second, for multi-config builds (e.g. msvc)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_PATH})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBRARY_OUTPUT_PATH})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBRARY_OUTPUT_PATH})
endforeach(OUTPUTCONFIG)

## Determine OS and architecture
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(WINDOWS ON BOOL FORCE)
	set(ARCH i686)
	set(WORD_SIZE 32)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(LINUX ON BOOL FORCE)
	
	# If someone has specified a word size, use that to determine the
	# architecture.  Otherwise, let the compiler specify the word size.
	# Using uname will break under chroots and other cross arch compiles. RC
	if (WORD_SIZE EQUAL 32)
		set(ARCH i686)
	elseif (WORD_SIZE EQUAL 64)
		set(ARCH x86_64)
	else (WORD_SIZE EQUAL 32)
		if(CMAKE_SIZEOF_VOID_P MATCHES 4)
			set(ARCH i686)
			set(WORD_SIZE 32)
		else(CMAKE_SIZEOF_VOID_P MATCHES 4)
			set(ARCH x86_64)
			set(WORD_SIZE 64)
		endif(CMAKE_SIZEOF_VOID_P MATCHES 4)
	endif (WORD_SIZE EQUAL 32)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(DARWIN ON BOOL FORCE)
	
	# If someone has specified a word size, use that to determine the
	# architecture.  Otherwise, let the compiler specify the word size.
	# Using uname will break under chroots and other cross arch compiles. RC
	if (WORD_SIZE EQUAL 32)
		set(ARCH i686)
	elseif (WORD_SIZE EQUAL 64)
		set(ARCH x86_64)
	else (WORD_SIZE EQUAL 32)
		if(CMAKE_SIZEOF_VOID_P MATCHES 4)
			set(ARCH i686)
			set(WORD_SIZE 32)
		else(CMAKE_SIZEOF_VOID_P MATCHES 4)
			set(ARCH x86_64)
			set(WORD_SIZE 64)
		endif(CMAKE_SIZEOF_VOID_P MATCHES 4)
	endif (WORD_SIZE EQUAL 32)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Get compiler and version
if (LINUX OR DARWIN)
	find_file(COMPILER_CHECK_SCRIPT "get_compiler_version.sh" ./ ../ ../..)
	if (NOT COMPILER_CHECK_SCRIPT)
		message(FATAL_ERROR "Unable to find get_compiler_version.sh")
	endif (NOT COMPILER_CHECK_SCRIPT)
	
	execute_process(
		COMMAND ${COMPILER_CHECK_SCRIPT} -c -s ${CMAKE_CXX_COMPILER}
		OUTPUT_VARIABLE COMPILER_DETECTED
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	execute_process(
		COMMAND ${COMPILER_CHECK_SCRIPT} -v -s ${CMAKE_CXX_COMPILER}
		OUTPUT_VARIABLE COMPILER_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif (LINUX OR DARWIN)
