#include "../sharedbase/EventLogger.h"
#include "../sharedbase/OSInterface_fwd.h"

#ifdef _WIN32
#include "../windows/win32.h"
#else
#include "OSInterface.h" // Included by win32 already, but we need it for other OSes.
#endif
#include "EngineCore.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Main
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main() {
	// Get the OS-specific subsystem online.
#ifdef _WIN32
	win32::Make();
#endif
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
		engine.Update();
	}

	engine.Shutdown();
	return 0;
}
