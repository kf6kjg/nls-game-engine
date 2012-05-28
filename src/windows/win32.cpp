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
#include "../sharedbase/Envelope.h"
#include "../sharedbase/EventLogger.h"

// Static class member initialization

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

	this->handle = hwnd;

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
			break;
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
			
		}
		break;
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
			def = false;
			
			
		}
		break;
		default:
		break;
	}
	
	return (def ? DefWindowProc(hwnd, msg, w, l) : 0);
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
	if (SUCCEEDED(SHGetFolderPath(this->handle, csidl, nullptr, 0, path))) {
		return path;
	}
	else {
		return "";
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::SetupTimer() {
	LARGE_INTEGER tps;
	QueryPerformanceFrequency(&tps);
	this->freq = 1.0f / (double)tps.QuadPart; // Determine the amount of counts per second
	QueryPerformanceCounter(&this->count); // Get the starting count
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double win32::GetElapsedTime() {
	LARGE_INTEGER old = this->count;
	QueryPerformanceCounter(&this->count); // Get the current count.
	this->elapsed = (this->count.QuadPart - old.QuadPart) * this->freq; // Find the difference of the ticks and multiply it by the counts per second. Elapsed is measured in seconds.
	return this->elapsed;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void win32::Quit(EnvelopeSPTR e) {
	DestroyWindow(this->handle);
}
