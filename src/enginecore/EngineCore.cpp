/**
* \file
* \author       Adam Martin
* \date         2012-04-14
* \brief		A script context wrapper.
* \details      Wraps a script context with several script building and executing functions
* 
*/


#include "EngineCore.h"

// System Library Includes
#include <cassert>

// Application Library Includes

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "EventLoggerRegister.h"

// Forward Declarations

// Typedefs

/**
* \param[in] elog A pointer to the Eventlogger created in main.
* \param[in] dir The current working directory to load scripts from.
*/
EngineCore::EngineCore( EventLogger* elog, std::string dir ) : now(boost::chrono::steady_clock::now()), workingdir(dir), elog(elog), modmgr(&engine) {

}

/**
* \return True on a successful config.
*/
bool EngineCore::StartUp() {
	EventLoggerRegister(engine.GetasIScriptEngine());

	int as_status = 0;

	// Register the APIs available to config scripts.
	this->engine.BeginConfigGroup("config"); {
		this->modmgr.ConfigRegister();

		this->engine.LoadScriptFile(this->workingdir + "/config.as");
		ScriptExecutor* exec = engine.ScriptExecutorFactory();
		as_status = exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
		if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
			return this->isRunning = false;
		}
		delete exec;
	} this->engine.EndConfigGroup();

	// Register the APIs available to game play scripts.
	this->engine.BeginConfigGroup("gameplay"); {
		this->EntList.Register(engine.GetasIScriptEngine());
		this->engine.LoadScriptFile(this->workingdir + "/main.as");
		ScriptExecutor* exec = engine.ScriptExecutorFactory();
		as_status = exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
		if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
			return this->isRunning = false;
		}
		delete exec;
	} this->engine.EndConfigGroup();

	return this->isRunning = true;
}

/**
* \return True if the engine is running.
*/
bool EngineCore::IsRunning() {
	return this->isRunning;
}

void EngineCore::Update() {
	// Timing variables used in the update function for the main loop.
	this->oldnow = this->now;
	this->now = boost::chrono::steady_clock::now();
	this->duraction = this->now - this->oldnow;

	// Calls update for each core.
	this->modmgr.Update(this->duraction.count());
}

void EngineCore::Shutdown() {
	this->modmgr.Shutdown();
}
