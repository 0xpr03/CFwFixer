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

			is_fixed = false;
		}
		else {
			// code to fix it

			is_fixed = true;
		}
	}else {
		// code to fix new window

		if_fixed = true;
		old_handle = window_handle;
	}
	return false;
}

WindowHandler::~WindowHandler()
{
}

void WindowHandler::get_window_handle()
{
	window_handle = FindWindow(0, windowName);
}
