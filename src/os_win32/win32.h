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
#include "../sharedbase/Envelope_fwd.h"

// Forward Declarations

class win32 : public OSInterface {
public:
	static void Make();
	
public:
	~win32() {
		PostQuitMessage(0);
	}
	void Quit(EnvelopeSPTR e);
	
	void SetClientRect(int width, int height);
	
	static LRESULT CALLBACK Proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
private:
	win32() : freq(0.0f), elapsed(0.0f), handle(nullptr) { }
	
private: // Overrides
	virtual boost::any CreateGUIWindow(int, int, std::string, WINDOW_FLAGS = WINDOW_OUTER_SIZE);
	virtual void ShowInfo(std::string, std::string = NLS_I18N::TITLE_INFO);
	virtual void ShowWarning(std::string, std::string = NLS_I18N::TITLE_WARNING);
	virtual void ShowError(std::string, std::string = NLS_I18N::TITLE_CRITICAL);
	virtual void RouteMessages();
	virtual std::string GetPath(SYSTEM_DIRS::TYPE);
	virtual void SetupTimer();
	virtual double GetElapsedTime();
	
private:
	LARGE_INTEGER count;
	double freq, elapsed;
	HWND handle;
};
