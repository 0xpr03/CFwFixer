#include <windows.h>
#pragma once
class WindowHandler
{
public:
	WindowHandler(char * window_name);
	boolean validName();
	~WindowHandler();
private:
	void get_window_handle(char * name);
	HWND window_handle;
	boolean is_fixed;
};

