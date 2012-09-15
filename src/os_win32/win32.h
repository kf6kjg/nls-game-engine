/**
 * \file
 * \author Adam Martin
 * \date 2011-12-22
 * \brief Win32 and windows specific code
*/
#pragma once

// Standard Includes
#include <string>

// Library Includes
#include <windows.h>

// Local Includes
#include "../sharedbase/OSInterface.h"
#include "../sharedbase/ModuleScriptInterface.h"

// Forward Declarations

class win32 : public OSInterface {
public:
	static void Make();
	
public:
	~win32() {
		PostQuitMessage(0);
	}
	
	void SetClientRect(int width, int height);
	
	bool IsRunning() {
		return this->running;
	}
	
	static LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	EventLogger* GetLogger();

	void Register(asIScriptEngine* const engine);
	
private:
	win32() { this->running = true; }
	
private: // Overrides
	virtual boost::any CreateGUIWindow(int, int, std::string, WINDOW_FLAGS = WINDOW_OUTER_SIZE);
	virtual void ShowInfo(std::string, std::string = NLS_I18N::TITLE_INFO);
	virtual void ShowWarning(std::string, std::string = NLS_I18N::TITLE_WARNING);
	virtual void ShowError(std::string, std::string = NLS_I18N::TITLE_CRITICAL);
	virtual void RouteMessages();
	virtual std::string GetPath(SYSTEM_DIRS::TYPE);

};
