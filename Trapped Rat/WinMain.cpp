/***********************************************************************\
|																		|
|	File:			WinMain.cpp			  								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To run the basic window framework for games			|
|																		|
|	� 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>		// Win32 Application
#include <vld.h>			// Visual Leak Detector!!!
#include "Code\Core\GameData.h"
#include "resource.h"

//*********************************************************************//
// Preprocessor Constants
#define WINDOW_CLASS_NAME	((const wchar_t*)L"Trapped Rat")		// window class name
#define WINDOW_TITLE		((const wchar_t*)L"Trapped Rat")		// window title
#define WINDOW_WIDTH		((int)800)									// window width
#define WINDOW_HEIGHT		((int)600)									// window height


//*********************************************************************//
// Function Declarations
int WINAPI			WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
bool				IsAlreadyRunning(void);
HWND				MakeWindow(HINSTANCE hInstance);
LRESULT CALLBACK	WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



//*********************************************************************//
// WinMain
//	- application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Don't let more than one instance of the application exist
	if (IsAlreadyRunning() == true)
		return -1;


	// Create the window
	HWND hWnd = MakeWindow(hInstance);
	if (hWnd == 0)
	{
		MessageBoxW(HWND_DESKTOP, L"ERROR: Failed to create the Main Window.", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return -2;
	}


	// Display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	/////////////////////////////////////////////////////////////////////
	// Initialize game

	// Access the singleton
	GameData * game = GameData::GetInstance();
	game->Initialize();


	// Run the message loop
	MSG msg = {};
	while (true)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			// Quit the application?
			if (msg.message == WM_QUIT)
				break;

			// Send the message to the window proc
			DispatchMessageW(&msg);
		}
		else
		{
			/////////////////////////////////////////////////////////////
			// Run game
			if (!game->Update())
				break;
		}
	}


	/////////////////////////////////////////////////////////////////////
	// Terminate game

	game->Terminate();
	


	// Unregister the window class
	UnregisterClassW(WINDOW_CLASS_NAME, hInstance);

	// Return message's Quit code to the OS
	return (int)(msg.wParam);
}


//*********************************************************************//
// IsAlreadyRunning
//	- check if another instance of this application is running
//	- set focus to the other instance if it exists
bool IsAlreadyRunning(void)
{
	// Is there another window with the same class name & title?
	HWND hOtherWnd = FindWindowW(WINDOW_CLASS_NAME, WINDOW_TITLE);
	if (hOtherWnd != 0)
	{
		// Is it minimized?
		if (IsIconic(hOtherWnd) == TRUE)
			ShowWindow(hOtherWnd, SW_RESTORE);

		// Bring it to the foreground
		SetForegroundWindow(hOtherWnd);
		return true;
	}

	// No other instances are running
	return false;
}


//*********************************************************************//
// MakeWindow
//	- registers & creates the application's window
HWND MakeWindow(HINSTANCE hInstance)
{
	// Window class description
	WNDCLASSEXW	winClassEx;

	// Set window class attributes
	winClassEx.cbSize = sizeof(winClassEx);
	winClassEx.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc = WindowProc;
	winClassEx.cbClsExtra = 0;
	winClassEx.cbWndExtra = 0;
	winClassEx.hInstance = hInstance;
	winClassEx.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hIconSm = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hCursor = LoadCursorW(NULL, (LPWSTR)IDC_ARROW);
	winClassEx.hbrBackground = (HBRUSH)(1 + COLOR_WINDOW);
	winClassEx.lpszMenuName = NULL;
	winClassEx.lpszClassName = WINDOW_CLASS_NAME;

	// Register the window class
	ATOM atomClass = RegisterClassExW(&winClassEx);
	if (atomClass == 0)
		return 0;


	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE | WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX ;

	// Setup the desired client area size
	RECT rWindow;
	rWindow.left = 0;
	rWindow.top = 0;
	rWindow.right = WINDOW_WIDTH;
	rWindow.bottom = WINDOW_HEIGHT;

	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx(&rWindow, dwWindowStyleFlags, FALSE, WS_EX_APPWINDOW);

	// Calculate the the window's dimensions
	int nWindowWidth = rWindow.right - rWindow.left;
	int nWindowHeight = rWindow.bottom - rWindow.top;


	// Create the window
	HWND hWnd = CreateWindowExW(
		WS_EX_APPWINDOW,										// extended style flags
		WINDOW_CLASS_NAME,										// window class Name
		WINDOW_TITLE,											// title
		dwWindowStyleFlags,										// window style flags
		(GetSystemMetrics(SM_CXSCREEN) - nWindowWidth) / 2,		// position (x, y)
		(GetSystemMetrics(SM_CYSCREEN) - nWindowHeight) / 2,		//	- centered on the desktop
		nWindowWidth,											// width
		nWindowHeight,											// height
		HWND_DESKTOP,											// parent window handle
		NULL,													// menu handle
		hInstance,												// application instance
		NULL													// creation parameters
		);

	// Did that fail?
	if (hWnd == 0)
		UnregisterClassW(WINDOW_CLASS_NAME, hInstance);

	// Return the handle to the new window
	return hWnd;
}


//*********************************************************************//
// WindowProc
//	- window message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// What is the message?
	switch (msg)
	{
	case WM_CREATE:			// Window constructed
		break;

	case WM_CLOSE:			// Window closed
		DestroyWindow(hWnd);	// completely destroy the window
		break;

	case WM_DESTROY:		// Window destroyed
		PostQuitMessage(0);
		// completely quit the application
		break;

	case WM_SYSKEYUP:		// System menu (ALT pop-up)
	case WM_SYSCHAR:
		break;

	case WM_ACTIVATE:		// Window activated / deactivated
		if (LOWORD(wParam) != WA_INACTIVE)	//	gaining focus (unpause)
		{
			GameData::GetInstance()->AltTab();
		}
		else									//	losing focus (pause)
		{
		}
		break;

	case WM_PAINT:			// Window needs repainting
		ValidateRect(hWnd, nullptr);	// ignore painting
		break;


	default:				// Any unhandled messages
		return DefWindowProcW(hWnd, msg, wParam, lParam);	// handled by default proc
	}


	// Message has been handled by this proc.
	return 0;
}