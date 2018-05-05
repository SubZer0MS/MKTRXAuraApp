#pragma once

#include "resource.h"

#define TRAYICONID	1	// ID number for the Notify Icon
#define SWM_TRAYMSG	WM_APP	// the message ID sent to our window

#define SWM_SHOW	WM_APP + 1	// show the window
#define SWM_HIDE	WM_APP + 2	// hide the window
#define SWM_EXIT	WM_APP + 3	// close the window

#define DEFAULT_BUFLEN 255
#define DEFAULT_PORT "27015"

#define PI 3.1415926
#define COLOR_STEP 1
#define SLEEP_FOR_TRANSITION 2
#define SLEEP_FOR_BETWEEN_TRANSITION 1000

// Global Variables:
HINSTANCE hInst;
NOTIFYICONDATA niData;
HMODULE hLib;
DWORD gColor;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

BOOL InitInstance(HINSTANCE, INT);
BOOL OnInitDialog(HWND);
VOID ShowContextMenu(HWND);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ListenForConnections(LPVOID);
VOID ChangeAuraColor(DWORD);

// import functions from the AURA SDK Dll
typedef void* MbLightControl;
typedef void* GPULightControl;
typedef void* ClaymoreKeyboardLightControl;
typedef void* RogMouseLightControl;

typedef DWORD(WINAPI* EnumerateMbControllerFunc)(MbLightControl handles[], DWORD size);
typedef DWORD(WINAPI* SetMbModeFunc) (MbLightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetMbColorFunc) (MbLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetMbColorFunc) (MbLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetMbLedCountFunc)(MbLightControl handle);

typedef DWORD(WINAPI* EnumerateGPUFunc)(GPULightControl handles[], DWORD size);
typedef DWORD(WINAPI* SetGPUModeFunc) (GPULightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetGPUColorFunc) (GPULightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetGPULedCountFunc)(GPULightControl handle);

typedef DWORD(WINAPI* CreateClaymoreKeyboardFunc)(ClaymoreKeyboardLightControl* handle);
typedef DWORD(WINAPI* SetClaymoreKeyboardModeFunc) (ClaymoreKeyboardLightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetClaymoreKeyboardColorFunc) (ClaymoreKeyboardLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetClaymoreKeyboardLedCountFunc)(ClaymoreKeyboardLightControl handle);

typedef DWORD(WINAPI* CreateRogMouseFunc)(RogMouseLightControl* handle);
typedef DWORD(WINAPI* SetRogMouseModeFunc) (RogMouseLightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetRogMouseColorFunc) (RogMouseLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* RogMouseLedCountFunc)(RogMouseLightControl handle);

EnumerateMbControllerFunc EnumerateMbController;
SetMbModeFunc SetMbMode;
SetMbColorFunc SetMbColor;
GetMbColorFunc GetMbColor;
GetMbLedCountFunc GetMbLedCount;

EnumerateGPUFunc EnumerateGPU;
SetGPUModeFunc SetGPUMode;
SetGPUColorFunc SetGPUColor;
GetGPULedCountFunc GetGPULedCount;

CreateClaymoreKeyboardFunc CreateClaymoreKeyboard;
SetClaymoreKeyboardModeFunc SetClaymoreKeyboardMode;
SetClaymoreKeyboardColorFunc SetClaymoreKeyboardColor;
GetClaymoreKeyboardLedCountFunc GetClaymoreKeyboardLedCount;

CreateRogMouseFunc CreateRogMouse;
SetRogMouseModeFunc SetRogMouseMode;
SetRogMouseColorFunc SetRogMouseColor;
RogMouseLedCountFunc RogMouseLedCount;