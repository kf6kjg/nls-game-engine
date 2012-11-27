/**
* \file
* \author Adam Martin
* \date 2011-10-23
* \brief ModuleManager declaration and dynamic library loading typedefs.
*/
#pragma once

// System Library Includes
#include <string>
#include <map>

// Application Library Includes

// Local Includes
#include "OSInterface_fwd.h"

// Forward Declarations
class EventLogger;
class ScriptEngine;
class ModuleInterface;

// Typedefs
typedef ModuleInterface* (*ModuleInstanceFactory)(OSInterfaceSPTR); /**< Used to find the address of the create system function. */

/**
* \brief Preprocessor selection based on OS. Used to use the correct include, and typedef for the handle.
*/
#ifdef _WIN32
#include <Windows.h>
typedef HMODULE DLLHANDLE;
#else
#include <dlfcn.h>
typedef void* DLLHANDLE;
#endif

namespace MODULE_STATUS {
	enum TYPE {
		NOT_FOUND, /**< Library file not found. */
		EXISTS, /**< Library file exists. */
		LOAD_ERROR, /**< Error loading the library. */
		START_ERROR, /**< Error starting the module. */
		LOADED, /**< Library loaded and module started. */
	};
}


/**
* \brief A manager class to load and start modules.
* \details A manager that can load/unload and start/stop modules at runtime through dynamically loaded
* libraries. The use of a common interface ModuleInterface allows us to have a uniform loading
* and starting procedure.
*/
class ModuleManager {
public:
	/**
	* \param engine A pointer to an instance of our script engine.
	*/
	ModuleManager() { }

	/**
	* \brief Loads a module.
	*/
	MODULE_STATUS::TYPE Load(const std::string &);
	
	/**
	* \brief Checks the status or existence a module/library.
	*/
	MODULE_STATUS::TYPE GetStatus(const std::string &);

	/**
	* \brief Unloads a module(s).
	*/
	void Unload(std::string);

	/**
	* \brief Calls the update method for all loaded modules.
	*/
	void Update(double = 0.0f);
	
	/**
	* \brief Used to control when unloading of all modules occurs.
	*/
	void Shutdown();

	/**
	* \brief Angelscript register for ModuleManager. Only registered for the config phase.
	*/
	void RegisterScriptEngine(ScriptEngine* const engine);
private:
	std::map<std::string, ModuleInterface*> modules; /**< A mapping of each module to its filename. */
	std::map<std::string, DLLHANDLE> libraries; /**< A mapping of each loaded library to a its filename */

	ScriptEngine* engine; /**< Pointer to the current script engine instance. Used during module loading. */
};
