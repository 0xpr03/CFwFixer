#include "main.h"

/**
 *CF window fixer (c) Aron Heinecke 2015, all rights reserved
 */

/*variables*/
UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
HHOOK KBHook;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Crossfire window fixer © Aron Heinecke");
char szClassName[] = "tray window";
bool first_paint = true;
WindowHandler wHandler = WindowHandler::WindowHandler(CF_WINDOW);


/*procedures  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
char* GetLastErrorString(void);
void print_message(UINT flag, int timeout, const char * title, const char * mesage);
void InitNotifyIconData();



int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
{
	/* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */
	WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	wincl.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 503;

	/* The class is registered, let's create the program*/
	Hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		szClassName,       /* Title Text */
		0, /* default window */
		0,       /* Windows decides the position */
		0,       /* where the window ends up on the screen */
		0,                 /* The programs width */
		0,                 /* and height in pixels */
		HWND_MESSAGE,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		NULL,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
		);
	/*Initialize the NOTIFYICONDATA structure only once*/
	InitNotifyIconData();
	/* Make the window visible on the screen */
	ShowWindow(Hwnd, nCmdShow);

	// store address of hook proc
	HOOKPROC lpfnHookProc = &LowLevelKeyboardProc;
	// try to set hook
	KBHook = SetWindowsHookEx(WH_KEYBOARD_LL, lpfnHookProc, GetModuleHandle(NULL), 0);
	// if hook was not installed, get the last error code, and string and
	//     show them both in a messagebox()
	if (KBHook == NULL)
	{
		char lpBuffer[50];
		char* errorString = GetLastErrorString();
		strcat_s(lpBuffer, errorString);
		MessageBox(HWND_DESKTOP, lpBuffer, "keyboard hook could not be installed!", MB_OK);
	}

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	// if there is a valid hook procedure, uninstall it
	if (KBHook)
	{
		// if unhookwindowshookex fails, show the last error as a string
		if (!UnhookWindowsHookEx(KBHook))
		{
			char* errorString = GetLastErrorString();
			MessageBox(HWND_DESKTOP, errorString, "Last Error", MB_OK);
		}
	}

	return messages.wParam;
}
/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == WM_TASKBAR && !IsWindowVisible(Hwnd))
	{
		//minimize();
		return 0;
	}

	switch (message)                  /* handle the messages */
	{
	case WM_ACTIVATE:
		Shell_NotifyIcon(NIM_ADD, &notifyIconData);
		break;
	case WM_CREATE:
		Hmenu = CreatePopupMenu();
		AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit CF window fixer"));
		PostMessage(hwnd, WM_TRAY_READY, 0, 0);
		break;
	case WM_TRAY_READY:
		print_message(NIIF_INFO,5000,"Started","CF window fixer is ready\nPress Str + L in CF.\nby ProcTrap");
		break;
	case WM_WINDOW_ERROR:
		print_message(NIIF_ERROR,3000,"ERROR", "No window found!");
		break;
	case WM_SYSICON:
	{
		if (lParam == WM_LBUTTONUP)
		{
			
			//restore();
		}
		else if (lParam == WM_RBUTTONDOWN)
		{
			// Get current mouse position.
			POINT curPoint;
			GetCursorPos(&curPoint);

			UINT clicked = TrackPopupMenu(Hmenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hwnd, NULL);

			SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
			if (clicked == ID_TRAY_EXIT)
			{
				// quit the application.
				Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
				PostQuitMessage(0);
			}
		}
	}
	break;
	case WM_NCHITTEST:
	{
		UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		else
			return uHitTest;
	}
	case WM_CLOSE:

		//minimize() ;
		return 0;
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT __stdcall LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	DWORD CTRL_key = 0;

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16;
		dwMsg += hooked_key.flags << 24;
		char lpszKeyName[1024] = { 0 };
		lpszKeyName[0] = '[';

		int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;
		lpszKeyName[i] = ']';

		int key = hooked_key.vkCode;
		CTRL_key = GetAsyncKeyState(VK_CONTROL);

		if (key >= 'A' && key <= 'Z')
		{

			if (GetAsyncKeyState(VK_SHIFT) >= 0) key += 32;

			if (CTRL_key != 0 && key == 'l')
			{
				if (wHandler.Run()) {
					PostMessage(Hwnd, WM_WINDOW_ERROR, 0, 0);
				}
				CTRL_key = 0;
			}

			if (CTRL_key != 0 && key == 't')
			{
				print_message(NIIF_INFO, 5000, "test", wHandler.testFunction().c_str());
			}
			CTRL_key = 0;

		}
	}
	return CallNextHookEx(KBHook, nCode, wParam, lParam);
}

void print_message(UINT flag,int timeout,const char * title, const char * mesage) {
	notifyIconData.uFlags = NIF_INFO;
	strcpy_s(notifyIconData.szInfoTitle, title); // Title
	strcpy_s(notifyIconData.szInfo, mesage); // Copy Tip
	notifyIconData.uTimeout = timeout;
	notifyIconData.dwInfoFlags = flag;

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}

char* GetLastErrorString(void)
{
	LPTSTR pszMessage;
	DWORD dwLastError = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pszMessage,
		0, NULL);
	return pszMessage;
}

void InitNotifyIconData()
{
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = Hwnd;
	notifyIconData.uID = ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
	notifyIconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
	strcpy_s(notifyIconData.szTip, sizeof(szTIP), szTIP);
}


