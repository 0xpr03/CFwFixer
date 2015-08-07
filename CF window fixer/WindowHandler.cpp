#include "WindowHandler.h"

WindowHandler::WindowHandler(char * window_name)
{
	windowName = window_name;
}

boolean WindowHandler::validName()
{
	return window_handle == 0 ? false : true;
}

/** Retrurns true on window get error */
boolean WindowHandler::Run() {
	get_window_handle();
	if (window_handle == NULL)
		return true;

	if (old_handle == window_handle) {
		if (is_fixed) {
			// code fo unfix it
			redoWindow();
			is_fixed = false;
		}
		else {
			// code to fix it
			stripWindow();
			is_fixed = true;
		}
	}else {
		// code to fix new window
		stripWindow();
		is_fixed = true;
		old_handle = window_handle;
	}
	return false;
}

void WindowHandler::stripWindow() {
	SetWindowLongPtr(window_handle, GWL_STYLE, GetWindowLongPtr(window_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
	SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(window_handle, SW_SHOW);
}

void WindowHandler::redoWindow() {
	SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowLongPtr(window_handle, GWL_STYLE, GetWindowLongPtr(window_handle, GWL_STYLE));
	//RedrawWindow(window_handle, RDW_FRAMECHANGED);
	ShowWindow(window_handle, SW_SHOW);
}

const char * WindowHandler::testFunction() {
	get_window_handle();
	if (window_handle == NULL)
		return "no window";

	RECT rcWindow, rcClient;

	GetWindowRect(window_handle, &rcWindow);
	GetClientRect(window_handle, &rcClient);
	std::string s = "Window: (" + std::to_string(rcWindow.left) + " " + std::to_string(rcWindow.top) + ")  (" + std::to_string(rcWindow.right) + " " + std::to_string(rcWindow.bottom);
	s += ")\nClient: (" + std::to_string(rcClient.left) + " " + std::to_string(rcClient.top) + ") (" + std::to_string(rcClient.right) + " " + std::to_string(rcClient.bottom)+")";
	return s.c_str();
}

WindowHandler::~WindowHandler()
{
}

void WindowHandler::get_window_handle()
{
	window_handle = FindWindow(0, windowName);
}
