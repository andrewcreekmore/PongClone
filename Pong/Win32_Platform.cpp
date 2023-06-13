#include <windows.h>
#include "Win32_Platform.h"
#include "Platform_Common.cpp"
#include "Utilities.h"
#include "Renderer.cpp"
#include "Game.cpp"


LRESULT CALLBACK GameWindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) 
	{
		case WM_CLOSE:
		{
			bRoundActive = false;
			ShowCursor(true);

			if (MessageBox(hwnd, "Really quit?", "Pong", MB_OKCANCEL) == IDOK)
			{ DestroyWindow(hwnd); }
	
			bRoundActive = true;
			ShowCursor(false);
		}
		break;

		case WM_DESTROY:
		{ bGameRunning = false; }
		PostQuitMessage(0);
		break;

		case WM_SIZE: 
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.width = rect.right - rect.left;
			renderState.height = rect.bottom - rect.top;

			int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);

			if (renderState.memory) // free existing prior to allocation of new memory
			{ VirtualFree(renderState.memory, 0, MEM_RELEASE); }

			renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
			renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
			renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
			renderState.bitmapInfo.bmiHeader.biPlanes = 1;
			renderState.bitmapInfo.bmiHeader.biBitCount = 32;
			renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
		} 
		break;

		default:
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ShowCursor(false); // hide cursor inside window

	// create Game Window class
	WNDCLASS GameWindowClass = {};
	GameWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	GameWindowClass.lpszClassName = "Game Window Class";
	GameWindowClass.lpfnWndProc = GameWindowCallback; // callback function

	// register Game Window class
	RegisterClass(&GameWindowClass);

	// create Game Window
	HWND gameWindow = CreateWindow(GameWindowClass.lpszClassName, "Pong by Andrew Creekmore", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	parentWindow = gameWindow;
	{
		// make game window full-screen
		SetWindowLong(gameWindow, GWL_STYLE, GetWindowLong(gameWindow, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO monitorInfo = { sizeof(monitorInfo) };
		GetMonitorInfo(MonitorFromWindow(gameWindow, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);
		SetWindowPos(gameWindow, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	HDC hdc = GetDC(gameWindow); // device context

	// create input struct
	Input input = {};

	// determine seconds per frame: 
	// delta time = (time @ frame end - time @ frame beginning) / CPU cycle count frequency 
	float deltaTime = 0.01666f; // to be calculated and set at end of each frame

	// establish time @ frame beginning
	LARGE_INTEGER frameBeginTime; 	
	QueryPerformanceCounter(&frameBeginTime);

	// determine CPU cycle frequency (in seconds)
	float performanceFrequency;
	{
		LARGE_INTEGER performance;
		QueryPerformanceFrequency(&performance);
		performanceFrequency = (float)performance.QuadPart;
	}

	// GAME LOOP
	while (bGameRunning)
	{
		// input
		MSG message;

		// clear bHasChanged flag each frame
		for (int i = 0; i < KEY_COUNT; i++)
		{ input.keyState[i].bHasChanged = false; }

		while (PeekMessage(&message, gameWindow, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					unsigned int vkCode = static_cast<unsigned int>(message.wParam);
					bool bIsKeyDown = ((message.lParam & (static_cast<long long>(1) << 31)) == 0);

					// helper function
					#define processKey(key, vk) \
					case vk: \
					{ \
						input.keyState[key].bHasChanged = bIsKeyDown != input.keyState[key].bIsDown; \
						input.keyState[key].bIsDown = bIsKeyDown; \
					} \
					break;

					// call helper with passed key
					switch (vkCode)
					{
						processKey(KEY_UP, VK_UP);
						processKey(KEY_DOWN, VK_DOWN);
						processKey(KEY_LEFT, VK_LEFT);
						processKey(KEY_RIGHT, VK_RIGHT);
						processKey(KEY_W, 'W');
						processKey(KEY_S, 'S');
						processKey(KEY_A, 'A');
						processKey(KEY_D, 'D');
						processKey(KEY_ENTER, VK_RETURN);
						processKey(KEY_SPACE, VK_SPACE);
						processKey(KEY_ESC, VK_ESCAPE);
					}
				}
				break;

				default:
					TranslateMessage(&message);
					DispatchMessage(&message);
			}
		}

		// simulate
		simulateGame(&input, deltaTime);;

		// render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		// finish determining deltaTime:
		// establish time @ frame end
		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		// calculate: seconds per frame = (end time - begin time) / seconds
		deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime; // setup for next frame
	}
};