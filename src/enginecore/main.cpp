/**
 * \file
 * \author Adam Martin
 * \brief *TODO:
 */

#include <cassert>
#include <boost/chrono.hpp>

#include "EntityList.h"
#include "../sharedbase/EventLogger.h"
#include "ModuleManager.h"
#include "ScriptEngine.h"
#include <EngineConfig.h>

int main() {
	EventLogger* elog = EventLogger::GetEventLogger(); // *TODO: Must fix the mem leak on exit
	EventLogger::module = "Main";
	elog->SetLogFile(NLS_ENGINE_DEFAULT_LOG_FILE);
	LOG(LOG_PRIORITY::FLOW, "Log file created!");

	ScriptEngine engine;
	EntityList EntList;
	ModuleManager modmgr(&engine);

	// Register the APIs available to config scripts.
	engine.BeginConfigGroup("config");
	modmgr.ConfigRegister();
	engine.EndConfigGroup();

	// Register the APIs available to game play scripts.
	engine.BeginConfigGroup("gameplay");
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
