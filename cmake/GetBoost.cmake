# -*- cmake -*-

# For each component you specify in find_package(), the following (UPPER-CASE)
# variables are set.  You can use these variables if you would like to pick and
# choose components for your targets instead of just using Boost_LIBRARIES.
#
#   Boost_${COMPONENT}_FOUND            True IF the Boost library "component" was found.
#
#   Boost_${COMPONENT}_LIBRARY          Contains the libraries for the specified Boost
#                                       "component" (includes debug and optimized keywords
#                                       when needed).
# Variables defined by this module:
#
#   Boost_FOUND                         System has Boost, this means the include dir was
#                                       found, as well as all the libraries specified in
#                                       the COMPONENTS list.
#
#   Boost_INCLUDE_DIRS                  Boost include directories: not cached
#
#   Boost_INCLUDE_DIR                   This is almost the same as above, but this one is
#                                       cached and may be modified by advanced users
#
#   Boost_LIBRARIES_DEBUG               Link to these to use the Boost libraries that you
#                                       specified: not cached
#
#   Boost_LIBRARIES_RELEASE             Link to these to use the Boost libraries that you
#                                       specified: not cached
#
#   Boost_LIBRARY_DIRS                  The path to where the Boost library files are.
#
#   Boost_VERSION                       The version number of the boost libraries that
#                                       have been found, same as in version.hpp from Boost
#
#   Boost_LIB_VERSION                   The version number in filename form as
#                                       it's appended to the library filenames
#
#   Boost_MAJOR_VERSION                 major version number of boost
#   Boost_MINOR_VERSION                 minor version number of boost
#   Boost_SUBMINOR_VERSION              subminor version number of boost
#

set(NLS_ENGINE_LIBRARY "Boost")

#set(BOOST_LIBS "all")
#    - chrono
#    - date_time
#    - exception
#    - filesystem
#    - graph
#    - graph_parallel
#    - iostreams
#    - math
#    - mpi
#    - program_options
#    - python
#    - random
#    - regex
#    - serialization
#    - signals
#    - system
#    - test
#    - thread
#    - wave
set(BOOST_LIBS "date_time,filesystem,system,thread,chrono" CACHE STRING
	"Comma-seperated list of boost library names"
	FORCE
)

set(BOOST_INCLUDE_DIR "${LIBS_INCLUDE_PATH}")

set(BOOST_BUILD_DEBUG 1 CACHE BOOL
	"Set to true to rebuild boost's debug libraries."
)
set(BOOST_BUILD_RELEASE 1 CACHE BOOL
	"Set to true to rebuild boost's release libraries."
)


if(DEFINED NLS_ENGINE_LIBRARY_MODE)
	set(BOOST_URL "http://downloads.sourceforge.net/project/boost/boost/1.50.0/boost_1_50_0.tar.bz2")
	set(BOOST_MD5 "52dd00be775e689f55a987baebccc462")

	load_library_source_from_web(
		"${NLS_ENGINE_LIBRARY}"
		"${NLS_ENGINE_LIBRARY}" # no trailing slash
		"${BOOST_URL}"
		"${BOOST_MD5}"
	)

	#* * * * * * * * * * * * * * * * * * * * *
	# Compile

	# *TODO: Maybe move the compilation out into the compile-time instead of here at config time!

	# Detect if compilation has already happened and don't do it again.
	if(NOT ${BOOST_BUILD_DEBUG} AND NOT ${BOOST_BUILD_RELEASE})
		message("Boost appears to already have been built; skipping build.  Set BOOST_BUILD_DEBUG or BOOST_BUILD_RELEASE to rebuild the selected version.")
	else(NOT ${BOOST_BUILD_DEBUG} AND NOT ${BOOST_BUILD_RELEASE})
		message("*** Bootstrapping Boost ***")
		
		if(WIN32)
			# Windows
			set(BOOST_BOOTSTRAP_COMMAND "${LIB_Boost_DIR}/boost_1_50_0/bootstrap.bat")
			set(BOOST_BOOTSTRAP_OPTIONS "")
		else(WIN32)
			# Linux and OSX
			set(BOOST_BOOTSTRAP_COMMAND "${LIB_Boost_DIR}/boost_1_50_0/bootstrap.sh")
			set(BOOST_BOOTSTRAP_OPTIONS "--libdir=\"${LIBS_BINARY_PATH}\"" " " "--includedir=\"${BOOST_INCLUDE_DIR}\"" " " "--with-libraries=${BOOST_LIBS}")
			if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
				set(BOOST_BOOTSTRAP_OPTIONS ${BOOST_BOOTSTRAP_OPTIONS} " " "--with-toolset=clang")
			endif(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
		endif(WIN32)
		
		message("Executing: " "${BOOST_BOOTSTRAP_COMMAND}")
		execute_process(
			COMMAND "${BOOST_BOOTSTRAP_COMMAND}" ${BOOST_BOOTSTRAP_OPTIONS}
			WORKING_DIRECTORY "${LIB_Boost_DIR}/boost_1_50_0"
			RESULT_VARIABLE BOOST_RESULT
		)
		if(NOT "${BOOST_RESULT}" MATCHES "^0")
			message(SEND_ERROR "Error bootstrapping Boost!  Result was: ${BOOST_RESULT}")
			return()
		endif(NOT "${BOOST_RESULT}" MATCHES "^0")
		
		message("*** Compiling Boost ***")
		
		if(WIN32)
			# Windows
			set(BOOST_COMPILE_COMMAND "${LIB_Boost_DIR}/boost_1_50_0/b2.exe")
			
			set(BOOST_COMPILE_LIBRARY_SELECTION "")
			string(REPLACE "," ";" BOOST_LIBS_LIST ${BOOST_LIBS})
			foreach(library ${BOOST_LIBS_LIST})
				set(BOOST_COMPILE_LIBRARY_SELECTION "${BOOST_COMPILE_LIBRARY_SELECTION} --with-${library}")
			endforeach(library)
			
			set(BOOST_COMPILE_TOOLSET "msvc")
			
			# File options and format pulled from bootstrap.sh
			file(WRITE "${LIB_Boost_DIR}/boost_1_50_0/project-config.jam"
				"# Boost.Build Configuration\n"
				"# Automatically generated by cmake\n"
				"\n"
				"import option ;\n"
				"import feature ;\n"
				"\n"
				"# Compiler configuration. This definition will be used unless you already have defined some toolsets in your user-config.jam file.\n"
				"if ! ${BOOST_COMPILE_TOOLSET} in [ feature.values <toolset> ]\n"
				"{\n"
				"    using ${BOOST_COMPILE_TOOLSET} ; \n"
				"}\n"
				"\n"
				"project : default-build <toolset>${BOOST_COMPILE_TOOLSET} ;\n"
				"\n"
				"# List of --with-<library> and --without-<library> options. If left empty, all libraries will be built. Options specified on the command line completely override this variable.\n"
				"libraries = ${BOOST_COMPILE_LIBRARY_SELECTION} ;\n"
				"\n"
				"# These settings are equivalent to corresponding command-line options.\n"
#				"option.set prefix : $PREFIX ;\n"
#				"option.set exec-prefix : $EPREFIX ;\n"
				"option.set libdir : \"${LIBS_BINARY_PATH}\" ;\n"
				"option.set includedir : \"${BOOST_INCLUDE_DIR}\" ;\n"
				"\n"
				"# Stop on first error\n"
				"option.set keep-going : false ;\n"
			)
			set(BOOST_COMPILE_OPTIONS "")
		else(WIN32)
			# Linux and OSX
			set(BOOST_COMPILE_COMMAND "${LIB_Boost_DIR}/boost_1_50_0/b2")
			
			if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
				set(BOOST_COMPILE_OPTIONS cxxflags="-I -std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++") # If you change this, make sure to change /src/CMakeLists.txt
			else(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
				set(BOOST_COMPILE_OPTIONS "")
			endif(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
		endif(WIN32)
		
		if("${BOOST_BUILD_DEBUG}" OR "${BOOST_BUILD_RELEASE}")
			execute_process(
				COMMAND "${BOOST_COMPILE_COMMAND}" "clean"
				WORKING_DIRECTORY "${LIB_Boost_DIR}/boost_1_50_0"
			)
		endif("${BOOST_BUILD_DEBUG}" OR "${BOOST_BUILD_RELEASE}")
		
		if("${BOOST_BUILD_DEBUG}")
			file(GLOB FOUND_FILES 
				"${LIBS_BINARY_PATH}/libboost_*-mt-gd.lib"
				"${LIBS_BINARY_PATH}/libboost_*-mt-d.dylib"
				"${LIBS_BINARY_PATH}/libboost_*-mt-d.a"
			)
			if(FOUND_FILES)
				file(REMOVE ${FOUND_FILES})
			endif(FOUND_FILES)
			
			message("Building boost debug version")
			execute_process(
				COMMAND "${BOOST_COMPILE_COMMAND}" ${BOOST_COMPILE_OPTIONS} "--layout=tagged" "runtime-debugging=on" "variant=debug" "install"
				WORKING_DIRECTORY "${LIB_Boost_DIR}/boost_1_50_0"
				OUTPUT_VARIABLE BOOST_COMPILE_ERROR
				ERROR_VARIABLE  BOOST_COMPILE_ERROR
				RESULT_VARIABLE BOOST_RESULT
			)
			if(NOT "${BOOST_RESULT}" MATCHES "^0")
				message(SEND_ERROR "Error compiling and installing Boost!  Result was: ${BOOST_RESULT}")
				message("Error data:")
				message(${BOOST_COMPILE_ERROR})
				message(FATAL_ERROR "Unable to continue")
			endif(NOT "${BOOST_RESULT}" MATCHES "^0")
			
			set(BOOST_BUILD_DEBUG 0 CACHE BOOL
				"Set to true to rebuild boost's debug libraries."
				FORCE
			)
		endif("${BOOST_BUILD_DEBUG}")
		
		if("${BOOST_BUILD_RELEASE}")
			file(GLOB FOUND_FILES 
				"${LIBS_BINARY_PATH}/libboost_*-mt.lib"
				"${LIBS_BINARY_PATH}/libboost_*-mt.dylib"
				"${LIBS_BINARY_PATH}/libboost_*-mt.a"
			)
			if(FOUND_FILES)
				file(REMOVE ${FOUND_FILES})
			endif(FOUND_FILES)
			
			message("Building boost release version")
			execute_process(
				COMMAND "${BOOST_COMPILE_COMMAND}" ${BOOST_COMPILE_OPTIONS} "--layout=tagged" "variant=release" "install"
				WORKING_DIRECTORY "${LIB_Boost_DIR}/boost_1_50_0"
				OUTPUT_VARIABLE BOOST_COMPILE_ERROR
				ERROR_VARIABLE  BOOST_COMPILE_ERROR
				RESULT_VARIABLE BOOST_RESULT
			)
			if(NOT "${BOOST_RESULT}" MATCHES "^0")
				message(SEND_ERROR "Error compiling and installing Boost!  Result was: ${BOOST_RESULT}")
				message("Error data:")
				message(${BOOST_COMPILE_ERROR})
				message(FATAL_ERROR "Unable to continue")
			endif(NOT "${BOOST_RESULT}" MATCHES "^0")
			
			set(BOOST_BUILD_RELEASE 0 CACHE BOOL
				"Set to true to rebuild boost's release libraries."
				FORCE
			)
		endif("${BOOST_BUILD_RELEASE}")
	endif(NOT ${BOOST_BUILD_DEBUG} AND NOT ${BOOST_BUILD_RELEASE})
endif(DEFINED NLS_ENGINE_LIBRARY_MODE)

if(EXISTS "${BOOST_INCLUDE_DIR}/boost")
	set(Boost_FOUND 1)
	set(Boost_INCLUDE_DIRS "${BOOST_INCLUDE_DIR}/boost")
	set(Boost_INCLUDE_DIR "${Boost_INCLUDE_DIRS}")
	set(Boost_LIBRARIES "")
	if(WINDOWS)
		file(GLOB Boost_LIBRARIES_DEBUG
			"${LIBS_BINARY_PATH}/libboost_*-mt-gd.lib"
		)
		file(GLOB Boost_LIBRARIES_RELEASE
			"${LIBS_BINARY_PATH}/libboost_*-mt.lib"
		)
	else(WINDOWS) # Unixes (Linux, etc.)
		file(GLOB Boost_LIBRARIES_DEBUG
			"${LIBS_BINARY_PATH}/libboost_*-mt-d.a"
		)
		file(GLOB Boost_LIBRARIES_RELEASE
			"${LIBS_BINARY_PATH}/libboost_*-mt.a"
		)
	endif(WINDOWS)

	set(Boost_LIBRARY_DIRS "${LIB_Boost_DIR}/lib")
	set(Boost_VERSION     "1.50.0")
	set(Boost_LIB_VERSION "") # Nothing is appended to custom-built versions.
	set(Boost_MAJOR_VERSION     1)
	set(Boost_MINOR_VERSION    50)
	set(Boost_SUBMINOR_VERSION  0)
endif(EXISTS "${BOOST_INCLUDE_DIR}/boost")
