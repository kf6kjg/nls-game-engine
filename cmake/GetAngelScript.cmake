# -*- cmake -*-

set(AS_ADDONS autowrapper scriptany scriptarray scriptbuilder scriptdictionary scriptmath scriptstdstring aswrapper)
#aswrapper
#autowrapper
#contextmgr
#debugger
#scriptany
#scriptarray
#scriptbuilder
#scriptdictionary
#scriptfile
#scripthandle
#scripthelper
#scriptmath
#scriptstdstring
#serializer

if(DEFINED NLS_ENGINE_LIBRARY_MODE OR DEFINED NLS_ENGINE_BUILD_MODE)
	include(LoadLibraries) # Needed to get the function it provides, so that the LIB_AngelScript_DIR variable would be set and the AS library can be set up for building.
	
	# Get the main AS library
	load_library_source_from_web(
		"AngelScript"
		"AngelScript" # no trailing slash
		"http://www.angelcode.com/angelscript/sdk/files/angelscript_2.23.1.zip"
		"9d1efe3f9d1c9c89e78eb71187b60659"
	)
	
	# Get the extra addon
	load_library_source_from_web(
		"AngelScript_addon_aswrapper"
		"AngelScript/sdk/add_on" # no trailing slash
		"http://www.angelcode.com/angelscript/extras/aswrapper.zip"
		"2948d9b7da58267bb60cc69d8c405a71"
	)
endif(DEFINED NLS_ENGINE_LIBRARY_MODE OR DEFINED NLS_ENGINE_BUILD_MODE)

if(DEFINED NLS_ENGINE_LIBRARY_MODE)
	#* * * * * * * * * * * * * * * * * * * * *
	# Compile
	
	message("Making directory '${LIBS_INCLUDE_PATH}/angelscript/'...")
	file(MAKE_DIRECTORY "${LIBS_INCLUDE_PATH}/angelscript/")
	
	
	message("Copying AngelScript header...")
	execute_process(
		COMMAND "${CMAKE_COMMAND}" -E copy "${LIB_AngelScript_DIR}/sdk/angelscript/include/angelscript.h" "${LIBS_INCLUDE_PATH}/angelscript/"
		RESULT_VARIABLE COPY_RESULT
	)
	if(NOT "${COPY_RESULT}" MATCHES "^0")
		message(SEND_ERROR "Copy of AS header (\"${LIB_AngelScript_DIR}/sdk/angelscript/include/angelscript.h\" \"${LIBS_INCLUDE_PATH}/angelscript/\") failed!")
	endif(NOT "${COPY_RESULT}" MATCHES "^0")
	message("Done.")
	
	if("${AS_ADDONS}" STRGREATER "")
		message("Copying AngelScript add-on headers...")
		foreach(loop_addon ${AS_ADDONS})
			file(GLOB AS_HEADERS
				"${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/*.h"
			)
			message("\tCopying headers for AngelScript add-on '${loop_addon}'...")
			if(NOT "${AS_HEADERS}" STRGREATER "")
				message(WARNING "\tNo headers found...  Looking in '${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/*.h'")
			endif(NOT "${AS_HEADERS}" STRGREATER "")
			foreach(loop_file ${AS_HEADERS})
				execute_process(
					COMMAND "${CMAKE_COMMAND}" -E copy ${loop_file} "${LIBS_INCLUDE_PATH}/angelscript/"
					RESULT_VARIABLE COPY_RESULT
				)
				if(NOT "${COPY_RESULT}" MATCHES "^0")
					message(WARNING "\t\tCopy of add-on header '${loop_file}' failed!")
				endif(NOT "${COPY_RESULT}" MATCHES "^0")
			endforeach(loop_file)
		endforeach(loop_addon)
		message("Done.")
	endif("${AS_ADDONS}" STRGREATER "")
endif(DEFINED NLS_ENGINE_LIBRARY_MODE)

if(DEFINED NLS_ENGINE_BUILD_MODE)
	# NOTE: This library creation process is based on the CMakeLists.txt included with AngelScript.  Changes there have to be propogated here.
	
	message("Setting up AngelScript for compilation as a static library.")
	
	# Get all the cxx files that need to be compiled.  Yes, globbing isn't the best technique for CMake.  That said, this code is unlikely to gain any per-user specific changes, and so this technique should be pretty safe.
	file(GLOB_RECURSE AngelScript_SOURCE
		"${LIB_AngelScript_DIR}/sdk/angelscript/source/as_*.cpp"
	)
	file(GLOB_RECURSE AngelScript_SOURCE_HEADERS
		"${LIB_AngelScript_DIR}/sdk/angelscript/source/as_*.h"
		"${LIB_AngelScript_DIR}/sdk/angelscript/include/angelscript.h"
	)
	foreach(loop_addon ${AS_ADDONS})
		file(GLOB ADDON_SOURCE_GLOB
			"${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/as*.cpp"
			"${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/script*.cpp"
		)
		set(AngelScript_ADDON_SOURCE ${AngelScript_ADDON_SOURCE} ${ADDON_SOURCE_GLOB})
		
		file(GLOB ADDON_HEADER_GLOB
			"${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/as*.h"
			"${LIB_AngelScript_DIR}/sdk/add_on/${loop_addon}/script*.h"
		)
		set(AngelScript_ADDON_SOURCE_HEADERS ${AngelScript_ADDON_SOURCE_HEADERS} ${ADDON_HEADER_GLOB})
	endforeach(loop_addon)
	source_group("Source" FILES ${AngelScript_SOURCE})
	source_group("Source Headers" FILES ${AngelScript_SOURCE_HEADERS})
	source_group("Addon Source" FILES ${AngelScript_ADDON_SOURCE})
	source_group("Addon Headers" FILES ${AngelScript_ADDON_SOURCE_HEADERS})
	
	add_definitions("-D_CRT_SECURE_NO_WARNINGS -DANGELSCRIPT_EXPORT -D_LIB -DAS_DEBUG")
	
	# Fix x64 issues on Linux
	if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
		add_definitions("-fPIC")
	endif()
	 
	add_library(AngelScript STATIC ${AngelScript_SOURCE} ${AngelScript_SOURCE_HEADERS} ${AngelScript_ADDON_SOURCE} ${AngelScript_ADDON_SOURCE_HEADERS})
	
	if(MSVC)
		set_target_properties(AngelScript PROPERTIES COMPILE_FLAGS "/MP")
	endif(MSVC)
endif(DEFINED NLS_ENGINE_BUILD_MODE)


if(EXISTS "${LIBS_INCLUDE_PATH}/angelscript")
	set(AngelScript_INCLUDE_DIRS "${LIBS_INCLUDE_PATH}/angelscript")
	set(AngelScript_LIBRARIES "")
	#if(WINDOWS)
	#	file(GLOB AngelScript_LIBRARIES
	#		"${LIBS_BINARY_PATH}/angelscript*.lib"
	#	)
	#else(WINDOWS)
	#	file(GLOB AngelScript_LIBRARIES
	#		"${LIBS_BINARY_PATH}/libAngelScript*.a"
	#	)
	#endif(WINDOWS)
	set(AngelScript_FOUND 1)
endif(EXISTS "${LIBS_INCLUDE_PATH}/angelscript")
