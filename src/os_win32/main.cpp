#include <windows.h>
#include "win32.h"
#include "../enginecore/EngineCore.h"
#include "../sharedbase/EventLogger.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd ) {
	win32::Make();
	OSInterfaceSPTR operating_system(OSInterface::GetOSPointer());

	std::string bin_dir(operating_system->GetPath(SYSTEM_DIRS::EXECUTABLE));

	// Load the event logger, logging to the config log location if running under debug or if the 
	EventLogger* elog = EventLogger::GetEventLogger(); // *TODO: Must fix the mem leak on exit
	EventLogger::module = "Main";
	elog->SetLogFile(bin_dir + "/" + NLS_ENGINE_DEFAULT_LOG_FILE);
	LOG(LOG_PRIORITY::FLOW, "Log file created!");

	EngineCore engine(elog, bin_dir);
	if (!engine.StartUp()) {
		LOG(LOG_PRIORITY::FLOW, "Engine startup failed.");
	}

	while(engine.IsRunning()) {
		operating_system->RouteMessages();
		engine.Update();
	}

	engine.Shutdown();
	return 0;
}
