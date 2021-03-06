/**
* \file
* \author Adam Martin
* \date 2011-10-28
* \brief Implements all win32 specific code
* 
*/

#include "win32.h"
#include <shlobj.h>

// Standard Includes

// Library Includes
#include <glm/glm.hpp>

// Local Includes
#include "../sharedbase/EventLogger.h"
#include "../enginecore/ScriptExecutor.h"
#include "../enginecore/ScriptEngine.h"

// Static class member initialization
ScriptEngine* g_engine; // *HACK, used for testing input in script.

void win32::Make() {
	if (!OSInterface::HasOS()) {
		OSInterface::SetOS(OSInterfaceSPTR(new win32()));
	}
}

// Class methods in the order they are defined within the class header

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boost::any win32::CreateGUIWindow( int width, int height, std::string title, WINDOW_FLAGS flags ) {
	ShowWindow( GetConsoleWindow(), SW_HIDE ); // We don't need to show the console

	HWND hwnd;
	WNDCLASSEX wc = { 0 };
	wc.lpszClassName = "GameWindow";
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = win32::Proc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)(CTLCOLOR_DLG + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.cbWndExtra = sizeof(long*);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(NULL, wc.lpszClassName, title.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | /*WS_THICKFRAME | WS_MAXIMIZEBOX |*/ WS_MINIMIZEBOX | CS_HREDRAW, 10, 10, width, height, NULL, NULL, wc.hInstance, NULL);
	if (!hwnd) {
		return 0;
	}
	UpdateWindow(hwnd);
	
	if (flags & WINDOW_INNER_SIZE) {
		RECT rect;
		rect.top = 0;
		rect.bottom = height;
		rect.left = 0;
		rect.right = width;
		AdjustWindowRectEx(&rect, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME| WS_MINIMIZEBOX | WS_MAXIMIZEBOX, false, 0);
		MoveWindow(hwnd, 0, 0, rect.right + abs(rect.left), rect.bottom + abs(rect.top), true);
	}
	
	ShowWindow(hwnd, SW_NORMAL);

	this->GUIHandle = hwnd;

	return hwnd;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::ShowInfo(std::string text, std::string caption) {
	MessageBox(nullptr, text.c_str(), caption.c_str(), MB_OK | MB_ICONINFORMATION);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::ShowWarning(std::string text, std::string caption) {
	MessageBox(nullptr, text.c_str(), caption.c_str(), MB_OK | MB_ICONWARNING);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::ShowError(std::string text, std::string caption) {
	MessageBox(nullptr, text.c_str(), caption.c_str(), MB_OK | MB_ICONERROR);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::RouteMessages() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
		if (msg.message == WM_QUIT) {
			this->running = false;
			break;
		} else if (msg.message == WM_KEYUP) {
			int as_status = 0;
			ScriptExecutor* exec = this->scriptEngine->ScriptExecutorFactory();
			as_status = exec->PrepareFunction(std::string("void keyUp(uint)"), std::string("enginecore"));
			as_status = exec->SetFunctionParam(0, msg.wParam);
			if (as_status < 0 || exec->ExecuteFunction() != asEXECUTION_FINISHED) {
				// If it fails, it might mean there isn't a function to handle keyUp(). This might not be a showstopper though.
			}
			delete exec;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
glm::vec2 GetMousePos(HWND hwnd, LPARAM lParam) {
	RECT client_rect; // client area rectangle 
	POINT client_UL; // client upper left corner 
	POINT client_LR; // client lower right corner
	POINTS client_mouse; // mouse position in client area
	
	glm::vec2 mouse_pos;
	
	// Capture mouse input.
	//SetCapture(hwnd);
	
	// Retrieve the screen coordinates of the client area, and convert them into client coordinates.
	GetClientRect(hwnd, &client_rect);
	client_UL.x = client_rect.left;
	client_UL.y = client_rect.top;
	
	// Add one to the right and bottom sides, because the coordinates retrieved by GetClientRect do not include the far left and lowermost pixels.
	client_LR.x = client_rect.right + 1; 
	client_LR.y = client_rect.bottom + 1; 
	ClientToScreen(hwnd, &client_UL); 
	ClientToScreen(hwnd, &client_LR);
	
	// Copy the client coordinates of the client area to the client_rect structure.
	// Confine the mouse cursor to the client area by passing the client_rect structure to the ClipCursor function. 
	//SetRect(&client_rect, client_UL.x, client_UL.y, client_LR.x, client_LR.y); 
	//ClipCursor(&client_rect);
	
	client_mouse = MAKEPOINTS(lParam);
	
	// Convert the mouse position into the correct coordinate system:
	// <0.0, 0.0> is at the upper-left of the client area,
	// <1.0, -1.0/aspect_ratio> is the lower-right of the client area.
	float client_width = float(client_LR.x - client_UL.x);
	mouse_pos.x = float(client_mouse.x) / client_width;
	mouse_pos.y = float(-client_mouse.y) / client_width; // Yes, the divisor here is correct: the Y is divided by the client width - this matches the Graphics coordinate system.
	
	return mouse_pos;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LRESULT CALLBACK win32::Proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	bool def = true; // Whether or not to call the default window procedure
	
	
	switch (msg) {
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_KEYLAST:
		{
			break;
		}
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
		{
			glm::vec2 mouse_pos = GetMousePos(hwnd, l);
			
			
		}
		break;
		case WM_MOUSEMOVE:
		{
			glm::vec2 mouse_pos = GetMousePos(hwnd, l);
			
			
		}
		break;
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		{
			glm::vec2 mouse_pos = GetMousePos(hwnd, l);
			
			
		}
		break;
		case WM_CREATE:
		break;
		case WM_DESTROY:
		case WM_CLOSE:
		{
			def = true;
			PostQuitMessage(0);
			
		}
		break;
		default:
			def = true;
		break;
	}
	
	return (def ? DefWindowProc(hwnd, msg, w, l) : 0);
}

EventLogger* win32::GetLogger() {
	return EventLogger::GetEventLogger();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string win32::GetPath(SYSTEM_DIRS::TYPE dir_id) {
	int csidl;
	
	switch (dir_id) {
		case SYSTEM_DIRS::USER:
			csidl = CSIDL_APPDATA;
		break;
		case SYSTEM_DIRS::DOCUMENTS:
			csidl = CSIDL_MYDOCUMENTS;
		break;
		case SYSTEM_DIRS::PICTURES:
			csidl = CSIDL_MYPICTURES;
		break;
		case SYSTEM_DIRS::MUSIC:
			csidl = CSIDL_MYMUSIC;
		break;
		case SYSTEM_DIRS::VIDEO:
			csidl = CSIDL_MYVIDEO;
		break;
		case SYSTEM_DIRS::DESKTOP:
			csidl = CSIDL_DESKTOPDIRECTORY;
		break;
		case SYSTEM_DIRS::EXECUTABLE: {
			std::string executable(_pgmptr);
			return executable.substr(0, executable.rfind('\\'));
		} break;
		default:
			return "";
		break;
	}
	
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(nullptr, csidl, nullptr, 0, path))) {
		return path;
	}
	else {
		return "";
	}
}

void win32::RegisterScriptEngine( ScriptEngine* const engine ) {
	this->scriptEngine = engine;
	asIScriptEngine* const asEngine = this->scriptEngine->GetasIScriptEngine();
	int ret = 0;

	// TODO: Move this block to OSInterface proper
	ret = asEngine->SetDefaultNamespace("Engine"); assert(ret >= 0);
	ret = asEngine->RegisterEnum("SYSTEM_DIRS"); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "USER", ::SYSTEM_DIRS::USER); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "DOCUMENTS", ::SYSTEM_DIRS::DOCUMENTS); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "PICTURES", ::SYSTEM_DIRS::PICTURES); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "MUSIC", ::SYSTEM_DIRS::MUSIC); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "VIDEO", ::SYSTEM_DIRS::VIDEO); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "DESKTOP", ::SYSTEM_DIRS::DESKTOP); assert(ret >= 0);
	ret = asEngine->RegisterEnumValue("SYSTEM_DIRS", "EXECUTABLE", ::SYSTEM_DIRS::EXECUTABLE); assert(ret >= 0);
	ret = asEngine->SetDefaultNamespace(""); assert(ret >= 0);


	ret = asEngine->RegisterObjectType("OSInterface", 0, asOBJ_REF | asOBJ_NOHANDLE ); assert(ret >= 0);
	ret = asEngine->RegisterGlobalProperty("OSInterface OS", this); assert(ret >= 0);
	ret = asEngine->RegisterObjectMethod("OSInterface", "string GetPath(SYSTEM_DIRS)", asMETHOD(win32, GetPath), asCALL_THISCALL); assert(ret >= 0);
	ret = asEngine->RegisterObjectMethod("OSInterface", "void ShowInfo(string, string)", asMETHOD(win32, ShowInfo), asCALL_THISCALL); assert(ret >= 0);
	ret = asEngine->RegisterObjectMethod("OSInterface", "void ShowWarning(string, string)", asMETHOD(win32, ShowWarning), asCALL_THISCALL); assert(ret >= 0);
	ret = asEngine->RegisterObjectMethod("OSInterface", "void ShowError(string, string)", asMETHOD(win32, ShowError), asCALL_THISCALL); assert(ret >= 0);
}
