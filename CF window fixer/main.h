#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <cstdlib>
#include "resource.h"
#include "WindowHandler.h"

#define CF_WINDOW		    "CROSSFIRE"//"Unbenannt - Editor"
#define ID_TRAY_APP_ICON    1001
#define ID_TRAY_EXIT        1002
#define WM_SYSICON          (WM_USER + 1)
#define WM_TRAY_READY       (WM_USER + 100)
#define WM_WINDOW_ERROR     (WM_USER + 101)
#define MSG_GREETING        "CF window fixer by ProcTrap\nPress Str + L in CF.\nPress Str + M to minimize CF"
#define MSG_WINDOW_ERROR    "No window found!"

