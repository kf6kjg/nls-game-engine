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
#include "../sharedbase/OSInterface.h"

// Forward Declarations

// Typedefs

/**
* \param[in] os A SPTR to an instance of OSInterface. This is stored, and also used to get the working directory and EventLogger.
*/
EngineCore::EngineCore( OSInterfaceSPTR os ) : now(boost::chrono::steady_clock::now()), workingdir(os->GetPath(SYSTEM_DIRS::EXECUTABLE)), elog(os->GetLogger()), os(os) {

}

/**
* \return True on a successful config.
*/
bool EngineCore::StartUp() {
	EventLogger::RegisterScriptEngine(&engine);
	this->os->RegisterScriptEngine(&engine);

	int as_status = 0;

	// Register the APIs available to config scripts.
	this->engine.BeginConfigGroup("config"); {
		this->modmgr.RegisterScriptEngine(&engine);
		this->engine.LoadScriptFile(this->workingdir + "/config.as");
		ScriptExecutor* exec = engine.ScriptExecutorFactory();
		as_status = exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
		if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
			delete exec;
			this->engine.Shutdown();
			return false;
		}
		delete exec;
	} this->engine.EndConfigGroup();

	// Register the APIs available to game play scripts.
	this->engine.BeginConfigGroup("gameplay"); {
		this->EntList.RegisterScriptEngine(&engine);
		this->engine.LoadScriptFile(engine.GetGameScript());
		ScriptExecutor* exec = engine.ScriptExecutorFactory();
		as_status = exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
		if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
			delete exec;
			this->engine.Shutdown();
			return false;
		}
		delete exec;
	} this->engine.EndConfigGroup();

	return this->engine.IsRunning();
}

/**
* \return True if the script engine is running.
*/
bool EngineCore::IsRunning() {
	return this->engine.IsRunning();
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
