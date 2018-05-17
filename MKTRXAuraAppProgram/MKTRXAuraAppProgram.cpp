// MKTXGetHWInformation.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MKTRXAuraAppProgram.h"

#pragma comment (lib, "Ws2_32.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_STEALTHDIALOG);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) ||
			!IsDialogMessage(msg.hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DLG_DIALOG), NULL, (DLGPROC)DlgProc);
	if(!hWnd) {
		return FALSE;
	}

	ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

	ULONGLONG ullVersion = utils::GetDllVersion(_T("Shell32.dll"));
	if(ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
	{
		niData.cbSize = sizeof(NOTIFYICONDATA);
	}
	else
	{
		niData.cbSize = NOTIFYICONDATA_V2_SIZE;
	}

	niData.uID = TRAYICONID;
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	niData.hWnd = hWnd;
	niData.uCallbackMessage = SWM_TRAYMSG;

	niData.hIcon = (HICON)LoadImage(
		hInstance,
		MAKEINTRESOURCE(IDI_STEALTHDLG),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR
	);

	SIZE_T strlen = sizeof(niData.szTip) / sizeof(TCHAR);
	StringCchCopy(niData.szTip, strlen, utils::GetStringResource(IDI_TOOL_TIP_DEFAULT, strlen));

	Shell_NotifyIcon(NIM_ADD, &niData);

	if(niData.hIcon && DestroyIcon(niData.hIcon))
	{
		niData.hIcon = NULL;
	}

    hLib = LoadLibraryA("AURA_SDK.dll");

    if (hLib == nullptr)
    {
        utils::PrintError(_T("Failed to load AURA_SDK.dll"));
        return FALSE;
    }

    (FARPROC&)EnumerateMbController = GetProcAddress(hLib, "EnumerateMbController");
    (FARPROC&)SetMbMode = GetProcAddress(hLib, "SetMbMode");
    (FARPROC&)SetMbColor = GetProcAddress(hLib, "SetMbColor");
    (FARPROC&)GetMbColor = GetProcAddress(hLib, "GetMbColor");
    (FARPROC&)GetMbLedCount = GetProcAddress(hLib, "GetMbLedCount");

    (FARPROC&)EnumerateGPU = GetProcAddress(hLib, "EnumerateGPU");
    (FARPROC&)SetGPUMode = GetProcAddress(hLib, "SetGPUMode");
    (FARPROC&)SetGPUColor = GetProcAddress(hLib, "SetGPUColor");
    (FARPROC&)GetGPULedCount = GetProcAddress(hLib, "GetGPULedCount");

    (FARPROC&)CreateClaymoreKeyboard = GetProcAddress(hLib, "CreateClaymoreKeyboard");
    (FARPROC&)SetClaymoreKeyboardMode = GetProcAddress(hLib, "SetClaymoreKeyboardMode");
    (FARPROC&)SetClaymoreKeyboardColor = GetProcAddress(hLib, "SetClaymoreKeyboardColor");
    (FARPROC&)GetClaymoreKeyboardLedCount = GetProcAddress(hLib, "GetClaymoreKeyboardLedCount");
    (FARPROC&)EnumerateMbController = GetProcAddress(hLib, "EnumerateMbController");

    (FARPROC&)CreateRogMouse = GetProcAddress(hLib, "CreateRogMouse");
    (FARPROC&)SetRogMouseMode = GetProcAddress(hLib, "SetRogMouseMode");
    (FARPROC&)SetRogMouseColor = GetProcAddress(hLib, "SetRogMouseColor");
    (FARPROC&)RogMouseLedCount = GetProcAddress(hLib, "RogMouseLedCount");

    HANDLE hThread = CreateThread(NULL, 0, ListenForConnections, NULL, 0, NULL);

    if (hThread == NULL)
    {
        utils::PrintError(_T("CreateThread"));
        return FALSE;
    }

    CloseHandle(hThread);

	return TRUE;
}

DWORD WINAPI ListenForConnections(LPVOID lpParam)
{
    WSADATA wsaData;
    int iResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        goto cleanup;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        goto cleanup;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        goto cleanup;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        goto cleanup;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    gColor = 0x000000;
    ChangeAuraColor(gColor);

    // Receive until the peer shuts down the connection
    do
    {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            switch (*recvbuf)
            {
            case '1':
                ChangeAuraColor(0x00FF00);
                break;

            case '2':
                ChangeAuraColor(0x0000FF);
                break;

            case '3':
                ChangeAuraColor(0xFF0000);
                break;

            case '4':
                ChangeAuraColor(0x000000);
                break;

            default:
                ChangeAuraColor(0x000000);
                break;
            }
        }
        else if (iResult == 0)
        {
            printf("Connection closing...\n");
        }
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            goto cleanup;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        goto cleanup;
    }

cleanup:
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

VOID ChangeAuraColor(DWORD colorVal)
{
    BYTE pR = (BYTE)((gColor & (0xFF << (2 * 8))) >> 2 * 8);
    BYTE pG = (BYTE)((gColor & (0xFF << (1 * 8))) >> 1 * 8);
    BYTE pB = (BYTE)((gColor & (0xFF << (0 * 8))) >> 0 * 8);

    BYTE R = (BYTE)((colorVal & (0xFF << (2 * 8))) >> 2 * 8);
    BYTE G = (BYTE)((colorVal & (0xFF << (1 * 8))) >> 1 * 8);
    BYTE B = (BYTE)((colorVal & (0xFF << (0 * 8))) >> 0 * 8);

    DWORD count = 0;
    
    // Motherboard

    //count = EnumerateMbController(NULL, 0); // comment this out because it seems to be stopping the whole thing if the MB does not have AURA

    if (count > 0)
    {
        BYTE* color = nullptr;
        MbLightControl* mbLightCtrl = new MbLightControl[count];
        
        EnumerateMbController(mbLightCtrl, count);

        for (DWORD i = 0; i < count; i++)
        {
            SetMbMode(mbLightCtrl[i], 1);

            DWORD t = GetMbLedCount(mbLightCtrl[i]);

            color = new BYTE[t * 3];
            ZeroMemory(color, t * 3);

            for (size_t s = 0; s < t; s += 3)
            {
                color[s] = pR;
                color[s + 1] = pG;
                color[s + 2] = pB;

                while (color[s] > 0x00 || color[s + 1] > 0x00 || color[s + 2] > 0x00)
                {
                    color[s] -= color[s] > 0x00 ? COLOR_STEP : 0;
                    color[s + 1] -= color[s + 1] > 0x00 ? COLOR_STEP : 0;
                    color[s + 2] -= color[s + 2] > 0x00 ? COLOR_STEP : 0;

                    SetMbColor(mbLightCtrl[i], color, t * 3);

                    Sleep(SLEEP_FOR_TRANSITION);
                }

                Sleep(SLEEP_FOR_BETWEEN_TRANSITION);

                while (color[s] < R || color[s + 1] < G || color[s + 2] < B)
                {
                    color[s] += color[s] < R ? COLOR_STEP : 0;
                    color[s + 1] += color[s + 1] < G ? COLOR_STEP : 0;
                    color[s + 2] += color[s + 2] < B ? COLOR_STEP : 0;

                    SetMbColor(mbLightCtrl[i], color, t * 3);

                    Sleep(SLEEP_FOR_TRANSITION);
                }

                color[s] = R;
                color[s + 1] = G;
                color[s + 2] = B;

                SetMbColor(mbLightCtrl[i], color, t * 3);
            }

            delete[] color;
        }

        delete[] mbLightCtrl;
    }

    // GPU

    count = EnumerateGPU(NULL, 0);

    if (count > 0)
    {
        BYTE* color = nullptr;
        GPULightControl* gpuLightCtrl = new GPULightControl[count];

        EnumerateGPU(gpuLightCtrl, count);

        for (DWORD i = 0; i < count; i++)
        {
            SetGPUMode(gpuLightCtrl[i], 1);
            
            DWORD t = GetGPULedCount(gpuLightCtrl[i]);
            
            color = new BYTE[t * 3];
            ZeroMemory(color, t * 3);
            
            for (size_t s = 0; s < t; s += 3)
            {
                color[s] = pR;
                color[s + 1] = pG;
                color[s + 2] = pB;

                while (color[s] > 0x00 || color[s + 1] > 0x00 || color[s + 2] > 0x00)
                {
                    color[s] -= color[s] > 0x00 ? COLOR_STEP : 0;
                    color[s + 1] -= color[s + 1] > 0x00 ? COLOR_STEP : 0;
                    color[s + 2] -= color[s + 2] > 0x00 ? COLOR_STEP : 0;

                    SetGPUColor(gpuLightCtrl[i], color, t * 3);

                    Sleep(SLEEP_FOR_TRANSITION);
                }

                Sleep(SLEEP_FOR_BETWEEN_TRANSITION);

                while (color[s] < R || color[s + 1] < G || color[s + 2] < B)
                {
                    color[s] += color[s] < R ? COLOR_STEP : 0;
                    color[s + 1] += color[s + 1] < G ? COLOR_STEP : 0;
                    color[s + 2] += color[s + 2] < B ? COLOR_STEP : 0;

                    SetGPUColor(gpuLightCtrl[i], color, t * 3);

                    Sleep(SLEEP_FOR_TRANSITION);
                }

                color[s] = R;
                color[s + 1] = G;
                color[s + 2] = B;

                SetGPUColor(gpuLightCtrl[i], color, t * 3);
            }

            delete[] color;
        }
        
        delete[] gpuLightCtrl;
    }

    gColor = colorVal;
}

BOOL OnInitDialog(HWND hWnd)
{
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);

	if(hMenu)
	{
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("About"));
	}

	HICON hIcon = (HICON)LoadImage(
					hInst,
					MAKEINTRESOURCE(IDI_STEALTHDLG),
					IMAGE_ICON,
					0,
					0,
					LR_SHARED | LR_DEFAULTSIZE
				);

	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	return TRUE;
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	
	if(hMenu)
	{
		if(IsWindowVisible(hWnd))
		{
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, _T("Hide"));
		}
		else
		{
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, _T("Show"));
		}

		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));

		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch(message)
	{
	case SWM_TRAYMSG:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;

		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
			break;
		}
		break;

	case WM_SYSCOMMAND:
		if((wParam & 0xFFF0) == SC_MINIMIZE)
		{
			ShowWindow(hWnd, SW_HIDE);
			return 1;
		}
		else if(wParam == IDM_ABOUT)
		{
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
		}
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case SWM_SHOW:
			ShowWindow(hWnd, SW_RESTORE);
			break;

		case SWM_HIDE:
		case IDOK:
			ShowWindow(hWnd, SW_HIDE);
			break;

		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		}

		return TRUE;

	case WM_INITDIALOG:
		return OnInitDialog(hWnd);

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		niData.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, &niData);
        closesocket(ClientSocket);
        WSACleanup();
        FreeLibrary(hLib);
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}
