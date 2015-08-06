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
	void stripWindow();
	void redoWindow();
	char * windowName;
	HWND old_handle = NULL;
	HWND window_handle = NULL;
	boolean is_fixed = false;
};

