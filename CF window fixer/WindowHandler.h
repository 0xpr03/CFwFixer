#include <windows.h>
#include <string>
#pragma once
class WindowHandler
{
public:
	WindowHandler(char * window_name);
	boolean validName();
	boolean Run(boolean strip);
	std::string testFunction();
	~WindowHandler();
private:
	void get_window_handle();
	void stripWindow();
	void minimizeWindow();
	char * windowName;
	HWND window_handle = NULL;
};

