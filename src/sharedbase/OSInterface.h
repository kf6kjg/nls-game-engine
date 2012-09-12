/**
 * \file
 * \author Adam Martin
 * \date 2011-12-22
 */
#pragma once

// Standard Includes

// Library Includes
#include <boost/any.hpp>
#include <EngineConfig.h>

// Local Includes
#include "OSInterface_fwd.h"
#include "ModuleScriptInterface.h"

// Forward Declarations
class EventLogger;

// Typedefs

// Classes
/**
 * \brief Base interface for any OS specific code
 */
class OSInterface : public ModuleScriptInterface {
public: // Public static members
	static void SetOS(OSInterfaceSPTR);
	static OSInterfaceSPTR GetOSPointer();
	static bool HasOS();
	
public:
	virtual ~OSInterface() {}
	
	/// Creates the platform specific window and returns the handle as a boost::any
	virtual boost::any CreateGUIWindow(int, int, std::string, WINDOW_FLAGS = WINDOW_OUTER_SIZE) = 0;
	
	/// Displays a message alerting the user to some non-critical information.  Displays a confirmation button.  Parameters are: body text, window title.  DOES NOT LOG TO DISK.
	virtual void ShowInfo(std::string, std::string = NLS_I18N::TITLE_INFO) = 0;
	
	/// Displays a message alerting the user to a non-critical problem.  Displays a confirmation button.  Parameters are: body text, window title.  DOES NOT LOG TO DISK.
	virtual void ShowWarning(std::string, std::string = NLS_I18N::TITLE_WARNING) = 0;
	
	/// Displays a message alerting the user to a critical problem.  Displays a confirmation button.  Parameters are: body text, window title.  DOES NOT LOG TO DISK.
	virtual void ShowError(std::string, std::string = NLS_I18N::TITLE_CRITICAL) = 0;
	
	/// Handles message routing from the OS to the respective functions or methods
	virtual void RouteMessages() = 0;
	
	/// Returns the path to a known system- or user-specific location.
	virtual std::string GetPath(SYSTEM_DIRS::TYPE) = 0;

	/**
	* \brief Used to get an event logger. Most cases the default EventLogger will be used, but certain OSes (Android) have special types.
	*/
	virtual EventLogger* GetLogger() = 0;

	/**
	* \brief Implementation defined on a per-OS level as to whether the system is still running or not.
	*/
	virtual bool IsRunning() = 0;
	
protected:
	OSInterface() {}
	bool running; /**< If the OS is still running */
	
private:
	static OSInterfaceSPTR operatingSystem;
};
