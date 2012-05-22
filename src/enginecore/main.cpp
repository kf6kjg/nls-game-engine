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

int main() {
	EventLogger* elog = EventLogger::GetEventLogger(); // *TODO: Must fix the mem leak on exit
	EventLogger::module = "Main";
	elog->SetLogFile(NLS_ENGINE_DEFAULT_LOG_FILE);
	LOG(LOG_PRIORITY::FLOW, "Log file created!");

	ScriptEngine engine;
	EntityMap EntList;
	ModuleManager modmgr(&engine);

	// Register the APIs available to config scripts.
	engine.BeginConfigGroup("config");
	modmgr.ConfigRegister();
	EventLoggerRegister(engine.GetasIScriptEngine());
	engine.EndConfigGroup();

	// Register the APIs available to game play scripts.
	engine.BeginConfigGroup("gameplay");
	//EventLoggerRegister(engine.GetasIScriptEngine()); //*BUG: Fails to register in this second config group.  I think it's an AS problem. ~Ricky
	EntList.Register(engine.GetasIScriptEngine());
	engine.EndConfigGroup();

	engine.LoadScriptFile("config.as");
	ScriptExecutor* exec = engine.ScriptExecutorFactory();
	exec->PrepareFunction(std::string("void main()"), std::string("enginecore"));
	exec->ExecuteFunction();
	delete exec;

	// Timing variables used in the update function for the main loop.
	boost::chrono::steady_clock::time_point now, oldnow;
	boost::chrono::duration<double, boost::ratio<1,1>> duraction;
	now = boost::chrono::steady_clock::now();

	while (1) {
		oldnow = now;
		now = boost::chrono::steady_clock::now();
		duraction = now - oldnow;

		// Calls update for each core.
		modmgr.Update(duraction.count());
	}
	modmgr.Shutdown();

	return 0;
}
