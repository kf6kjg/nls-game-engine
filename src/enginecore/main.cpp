/**
 * \file
 * \author Adam Martin
 * \brief *TODO:
 */

#include <cassert>
#include <boost/chrono.hpp>

#include <EngineConfig.h>

#include "EventLoggerRegister.h"
#include "EntityMap.h"
#include "../sharedbase/EventLogger.h"
#include "ModuleManager.h"
#include "ScriptEngine.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Operational data
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool gIsRunning = true;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Main
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main() {
	EventLogger* elog = EventLogger::GetEventLogger(); // *TODO: Must fix the mem leak on exit
	EventLogger::module = "Main";
	elog->SetLogFile(NLS_ENGINE_DEFAULT_LOG_FILE);
	LOG(LOG_PRIORITY::FLOW, "Log file created!");

	ScriptEngine engine;
	EntityMap EntList;
	ModuleManager modmgr(&engine);

	int as_status = 0;

	EventLoggerRegister(engine.GetasIScriptEngine());

	// Register the APIs available to config scripts.
	engine.BeginConfigGroup("config"); {
		modmgr.ConfigRegister();
		
		engine.LoadScriptFile("config.as");
		ScriptExecutor* exec = engine.ScriptExecutorFactory();
		as_status = exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
		if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
			gIsRunning = false;
		}
		delete exec;
	} engine.EndConfigGroup();

	if (gIsRunning) {
		// Register the APIs available to game play scripts.
		engine.BeginConfigGroup("gameplay"); {
			EntList.Register(engine.GetasIScriptEngine());
		} engine.EndConfigGroup();

		// Timing variables used in the update function for the main loop.
		boost::chrono::steady_clock::time_point now, oldnow;
		boost::chrono::duration<double, boost::ratio<1,1>> duraction;
		now = boost::chrono::steady_clock::now();

		while (gIsRunning) {
			oldnow = now;
			now = boost::chrono::steady_clock::now();
			duraction = now - oldnow;

			// Calls update for each core.
			modmgr.Update(duraction.count());
		}
	}

	modmgr.Shutdown();

	return 0;
}
