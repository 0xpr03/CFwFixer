#include <windows.h>
#pragma once
class WindowHandler
{
public:
	WindowHandler(char * window_name);
	boolean validName();
	boolean Run();
	~WindowHandler();
private:
	void get_window_handle();
	char * windowName;
	HWND old_handle;
	HWND window_handle;
	boolean is_fixed;
};

