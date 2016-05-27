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
boolean WindowHandler::Run(boolean strip) {
	get_window_handle();
	if (window_handle == NULL)
		return true;

	if (strip)
		stripWindow();
	else
		minimizeWindow();
	return false;
}

void WindowHandler::stripWindow() {

	SetWindowLongPtr(window_handle, GWL_STYLE, GetWindowLongPtr(window_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);

	//TODO: add this for multi screens support
	/*MonitorFromWindow(window_handle,MONITOR_DEFAULTTOPRIMARY);

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);*/

	//MoveWindow(window_handle,0,0,rcClient.right,rcClient.bottom,true);
	//SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, cxScreen, cyScreen, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, cxScreen, cyScreen, SWP_FRAMECHANGED | SWP_DRAWFRAME);
	ShowWindow(window_handle, SW_SHOW);
}

void WindowHandler::minimizeWindow() {
	SetWindowPos(window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowLongPtr(window_handle, GWL_STYLE, GetWindowLongPtr(window_handle, GWL_STYLE));
	ShowWindow(window_handle, SW_MINIMIZE);
}

std::string WindowHandler::testFunction() {
	RECT rcWindow, rcClient;
	get_window_handle();
	if (window_handle == NULL)
		return "error";

	GetWindowRect(window_handle, &rcWindow);
	GetClientRect(window_handle, &rcClient);
	std::string s = "Window: (" + std::to_string(rcWindow.left) + " | " + std::to_string(rcWindow.top) + ")  (" + std::to_string(rcWindow.right) + " | " + std::to_string(rcWindow.bottom);
	s += ")\nClient: (" + std::to_string(rcClient.left) + " | " + std::to_string(rcClient.top) + ") (" + std::to_string(rcClient.right) + " | " + std::to_string(rcClient.bottom)+")";
	return s;
}

WindowHandler::~WindowHandler()
{
}

void WindowHandler::get_window_handle()
{
	window_handle = FindWindow(0, windowName);
}
