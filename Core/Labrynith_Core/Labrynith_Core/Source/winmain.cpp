//////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	"WinMain.cpp"
//
//	Author			:	Ethan pendergraft
//	Based in part on:
//		-The previous WinMain.cpp by David Brown.
//
//	Last Modified	:	7/18/2011
//
//	Purpose			:	To provide a basic window framework for student games.
//
//////////////////////////////////////////////////////////////////////////////////////////////

/* 

	----- Prayer of Program -----

	I am the heart of my functions.
	Ciphers are my body, and calculation is my blood.
	I have written over ten-thousand lines.
	Unknown to termination.
	Nor known to initialization.
	I have withstood many errors to compile many programs.
	Yet, these addresses will never hold anything.
	So as I pray, 'Unlimited Code-Works'!

*/

#include <windows.h>	//	Needed for Windows Applications.

#include <vld.h>

#include <iostream>

#include <ctime>

#include "../resource.h"

#include "CGame.h"

//#pragma comment( linker, "/SUBSYSTEM:Console" )

const char* g_szWINDOW_CLASS_NAME	= "LabyrinthCore";			//	Window Class Name.

const char* g_szWINDOW_TITLE		= "Core Build 1.0";			//	Window Title.
const int	g_nWINDOW_WIDTH			= 1024;						//	Window Width.
const int	g_nWINDOW_HEIGHT		= 768;						//	Window Height.
const BOOL	g_bIS_WINDOWED			= true;		



//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//	This is the main message handler of the system.
	PAINTSTRUCT	ps;			//	Used in WM_PAINT.
	HDC			hdc;		//	Handle to a device context.

	//	What is the message 
	switch(msg)
	{
		//	To skip ALT pop up menu (system menu)
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
			return(0);
		break;
		
		//	Handle ALT+F4
		case WM_CLOSE:
		{
			// Sends us a WM_DESTROY
			DestroyWindow(hWnd);			
		}
		break;

		//	and lose/gain focus
		case WM_ACTIVATE:
		{
			//	gaining focus
			if (LOWORD(wParam) != WA_INACTIVE)
			{
				// unpause game code here
			}
			else // losing focus
			{
				// pause game code here
			}
		}
		break;

		case WM_CREATE: 
		{
			//	Do initialization here
			return(0);
		}
		break;

		case WM_PAINT:
		{
			//	Start painting
			hdc = BeginPaint(hWnd,&ps);

			//	End painting
			EndPaint(hWnd,&ps);
			return(0);
		}
		break;

		case WM_DESTROY: 
		{
			//	Kill the application			
			PostQuitMessage(0);
			return(0);
		}
		break;

		default:
		break;
	}

	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

//	Checks to see if the game was already running in another window.
//
//	NOTE:	Don't call this function if your game needs to have more
//			than one instance running on the same computer (i.e. client/server)
BOOL CheckIfAlreadyRunning(void)
{
	//	Find a window of the same window class name and window title
	HWND hWnd = FindWindow(g_szWINDOW_CLASS_NAME, g_szWINDOW_TITLE);

	//	If one was found
	if (hWnd)
	{
		//	If it was minimized
		if (IsIconic(hWnd))
			//	restore it
			ShowWindow(hWnd, SW_RESTORE);

		//	Bring it to the front
		SetForegroundWindow(hWnd);

		return TRUE;
	}

	//	No other copies found running
	return FALSE;
}

BOOL RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX	winClassEx;	//	This will describe the window class we will create.

	//	First fill in the window class structure
	winClassEx.cbSize			= sizeof(winClassEx);
	winClassEx.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc		= WindowProc;
	winClassEx.cbClsExtra		= 0;
	winClassEx.cbWndExtra		= 0;
	winClassEx.hInstance		= hInstance;
	winClassEx.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hIconSm			= NULL;
	winClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	winClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClassEx.lpszMenuName		= NULL; 
	winClassEx.lpszClassName	= g_szWINDOW_CLASS_NAME;

	//	Register the window class
	return RegisterClassEx(&winClassEx);
}

//	Creates and sizes the window appropriately depending on if 
//	the application is windowed or full screen.
HWND MakeWindow(HINSTANCE hInstance)
{
	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	if (g_bIS_WINDOWED)
	{
		dwWindowStyleFlags = WS_POPUP | WS_MINIMIZEBOX;
	}
	else
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
		ShowCursor(FALSE);	// Stop showing the mouse cursor
	}

	// Setup the desired client area size
	RECT rWindow;
	rWindow.left	= 0;
	rWindow.top		= 0;
	rWindow.right	= g_nWINDOW_WIDTH;
	rWindow.bottom	= g_nWINDOW_HEIGHT;

	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx(&rWindow, 
						dwWindowStyleFlags,
						FALSE, 
						WS_EX_APPWINDOW);
	
	// Calculate the width/height of that window's dimensions
	int nWindowWidth	= rWindow.right - rWindow.left;
	int nWindowHeight	= rWindow.bottom - rWindow.top;

	//	Create the window
	return CreateWindowEx(WS_EX_APPWINDOW,											//	Extended Style flags.
						  g_szWINDOW_CLASS_NAME,									//	Window Class Name.
						  g_szWINDOW_TITLE,											//	Title of the Window.
						  dwWindowStyleFlags,										//	Window Style Flags.
						  (GetSystemMetrics(SM_CXSCREEN)/2) - (nWindowWidth/2),		//	Window Start Point (x, y). 
						  (GetSystemMetrics(SM_CYSCREEN)/2) - (nWindowHeight/2),	//		-Does the math to center the window over the desktop.
						  nWindowWidth,												//	Width of Window.
						  nWindowHeight,											//	Height of Window.
						  NULL,														//	Handle to parent window.
						  NULL,														//	Handle to menu.
						  hInstance,												//	Application Instance.
						  NULL);													//	Creation parameters.
}

//////////////////////////
//		WinMain			//
//////////////////////////

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;	//	Generic message.
	HWND	hWnd;	//	Main Window Handle.

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// this function call will set a breakpoint at the location of a leaked block
	// set the parameter to the identifier for a leaked block
	//	_CrtSetBreakAlloc(187);

	// Don't let more than one instance of the application exist
	//
	// NOTE:	Comment out the following section of code if your game needs to have more
	//			than one instance running on the same computer (i.e. client/server)
	////////////////////////////////////////////////////////////////////////
	if (!hPrevInstance)
	{
		if (CheckIfAlreadyRunning())
		{
			hPrevInstance = hInstance;
			return FALSE;
		}
	}
	////////////////////////////////////////////////////////////////////////

	//	Register the window class
	if (!RegisterWindowClass(hInstance))
		return 0;

	//	Create the window
	hWnd = MakeWindow(hInstance);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//////////////////////////////////////////
	//	Initialize Game here
	//////////////////////////////////////////


	CGame* theGame = CGame::GetInstance();

	theGame->Initialize( hWnd, hInstance, g_nWINDOW_WIDTH, g_nWINDOW_HEIGHT, true );

	//////////////////////////////////////////

	//	Enter main event loop
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			//	Test if this is a quit
			if (msg.message == WM_QUIT)
				break;
		
			//	Translate any accelerator keys
			TranslateMessage(&msg);

			//	Send the message to the window proc
			DispatchMessage(&msg);
		}
		
		//////////////////////////////////
		//	Put Game Logic Here
		//////////////////////////////////
		
		if ( !theGame->Main() )
			PostQuitMessage(0);
		
		//////////////////////////////////
	}

	/////////////////////////////////////////
	//	Shutdown Game Here
	/////////////////////////////////////////
	
	theGame->Shutdown();

	/////////////////////////////////////////
	
	//	Unregister the window class
	UnregisterClass(g_szWINDOW_CLASS_NAME, hInstance);

	//	Return to Windows like this.
	return (int)(msg.wParam);
}

int main(void)
{
	WinMain(GetModuleHandle(NULL), NULL,
		GetCommandLine(), SW_SHOWDEFAULT);
	return 0;
}