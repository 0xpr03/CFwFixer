#include "WindowHandler.h"

WindowHandler::WindowHandler(char * window_name)
{
	get_window_handle(window_name);

}

boolean WindowHandler::validName()
{
	window_handle == 0 ? false : true;
}


WindowHandler::~WindowHandler()
{
}

void WindowHandler::get_window_handle(char * name)
{
	window_handle = FindWindow(0, name);
}
