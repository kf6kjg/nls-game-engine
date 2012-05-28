/**
* \file
* \author	Adam Martin
* \date		2012-04-14
* \brief	ScriptEngine declaration.
* 
*/
#pragma once

// System Library Includes

// Application Library Includes
#include <angelscript.h>
#include <angelscript/scriptbuilder.h>

// Local Includes
#include "ScriptExecutor.h"

// Forward Declarations

// Typedefs

/**
* \brief Return values for script loading and module building
*/
namespace SCRIPT_STATUS {
	enum TYPE {
		FAILED_LOADING, ///< Failed to load the file.
		FAILED_BUILDING, ///< Failed to build the module.
		LOAD_OK ///< File loaded and built without issue.
	};
}

/**
* \brief	A class to control the basic operation of the script engine.
* \details	Handles script engine initialization, shutdown, and a context pool.
* Scripts are loaded and built by calling the appropriate methods for the given source.
* Registering a script interface is accomplished by getting the engine pointer via
* GetasIScriptEngine and calling the correct registration methods from it.
*/
class ScriptEngine {
public: // Static Methods
	/**
	* \brief Registers math types and their helper functions.
	*/
	static void RegisterMathTypes(asIScriptEngine* const);
	
public: // Structors
	/**
	* \brief Creates an instance of Angelscript's engine and initializes it.
	* Additionally it registers some basic add-ons provide by Angelscript for strings,
	* arrays, math functions, any type, and dictionary data type. Lastly a default executor
	* is made with a context, and a default module "enginecore".
	*/
	ScriptEngine();
	~ScriptEngine();

public: // API Methods
	/**
	* \brief Creates a ScriptExecutor object that can be used to execute scripts functions.
	*/
	ScriptExecutor* ScriptExecutorFactory();

	/**
	* \brief Returns a pointer to the asIScriptEngine.
	*/
	asIScriptEngine* const GetasIScriptEngine();

	/**
	* \brief Loads a script from a file.
	*/
	SCRIPT_STATUS::TYPE LoadScriptFile(const std::string &);

	/**
	* \brief Begins the registration for a specific config group.
	*/
	bool BeginConfigGroup(const std::string &);

	/**
	* \brief Ends the registration for a specific config group.
	*/
	bool EndConfigGroup();

	/**
	* \brief Removes a specific config group.
	*/
	bool RemoveConfigGroup(const std::string &);

	/**
	* \brief Called by script to set userDataFolder.
	*/
	void SetUserDataFolder(const std::string &);

	/**
	* \brief Called by script to set the log file used by EventLogger.
	*/
	void SetLogFile(const std::string &);

	/**
	* \brief Set the name of the main game script.
	*/
	void SetGameScript(const std::string &) { }

private: // Internal methods
	/**
	* \brief A message callback used by Angelscript to give feedback about warning and errors.
	*/
	void MessageCallback(const asSMessageInfo *msg);

	/**
	* \brief A message callback used by Angelscript for critical exceptions that cause the engine to crash.
	*/
	void ExceptionCallback(asIScriptContext* ctx);

	/**
	* \brief Causes any currently executing script to stop.
	* 
	* Calls Abort() on the current script context, but set no flags.
	*/
	static void AbortExecution();

private: // Data
	asIScriptEngine *engine; ///< The script engine.
	ScriptExecutor scriptexec; ///< The script executor.
	CScriptBuilder builder; ///< Used for building scripts from files.

	std::string userDataFolder; ///< Location where user data is stored such as saves or profiles.
	std::string gameplayScript; ///< The gameplay phase script.
};
