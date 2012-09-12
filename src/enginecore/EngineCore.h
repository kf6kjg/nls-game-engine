/**
* \file
* \author	Adam Martin
* \date		2012-07-11
* \brief	EngineCore declaration.
* 
*/
#pragma once

// System Library Includes

// Application Library Includes
// *NOTE: We don't need hybrid error checking, and it's nice to not have to compile and link the chrono library, so specify it to be header-only.
#define BOOST_CHRONO_HEADER_ONLY
#define BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING 
#include <boost/chrono.hpp>

// Local Includes
#include "ModuleManager.h"
#include "ScriptEngine.h"
#include "EntityMap.h"
#include "OSInterface_fwd.h"

// Forward Declarations
class EventLogger;

// Typedefs

/**
* \brief	Wrapper class that modularizes the whole engine.
* \details	Used by OS specific main functions to start, stop, and update the engine.
*/
class EngineCore {
public:
	EngineCore(EventLogger* elog, std::string dir);
	EngineCore(OSInterfaceSPTR os);

	/**
	* \brief Starts the engine by calling config.as. After the config phase is done it loads the main gameplay script.
	*/
	bool StartUp();

	/**
	* \brief Returns if the engine is still running
	*/
	bool IsRunning();

	/**
	* \brief Calls update on all loaded modules
	*/
	void Update();

	/**
	* \brief Shuts down all modules
	*/
	void Shutdown();
private:
	bool isRunning;
	boost::chrono::steady_clock::time_point now, oldnow;
	boost::chrono::duration<double, boost::ratio<1,1>> duraction;
	std::string workingdir;

	EventLogger* elog;
	ScriptEngine engine;
	EntityMap EntList;
	ModuleManager modmgr;
	OSInterfaceSPTR os;
};
