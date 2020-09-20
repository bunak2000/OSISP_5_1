#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

const wchar_t* const ClassName = L"PerfectLab";
	RECT Rect;
	HDC hdc, hCompatibleDC;
	HANDLE hBitmap, hOldBitmap;

	int X, Y;
	int SizeX, SizeY;
	int TimerDelay;
	int DirectionFlagX;
	int DirectionFlagY;
	int Speed;

void InitializeGlobalVariables(HWND hWnd) 
{
	SizeX = 100;
	SizeY = 100;
	TimerDelay = 10;
	Speed = 3;

	GetClientRect(hWnd, &Rect);
	X = rand() % (Rect.right - SizeX - 10) + 10;
	Y = rand() % (Rect.bottom - SizeY - 10) + 10;
	DirectionFlagX = rand() % 1;
	DirectionFlagY = rand() % 1;

	hBitmap = LoadImage(NULL, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		hCompatibleDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hCompatibleDC, hBitmap);

		GetClientRect(hWnd, &Rect);

		RECT rect;
		rect.left = X;
		rect.top = Y;
		rect.right = X + SizeX;
		rect.bottom = Y + SizeY;
		FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
		//StretchBlt(hdc, X, Y, SizeX, SizeY, hCompatibleDC, 0, 0, Rect.right, Rect.bottom, SRCCOPY);

		SelectObject(hCompatibleDC, hOldBitmap);
		DeleteDC(hCompatibleDC);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_KEYDOWN:
		GetClientRect(hWnd, &Rect);
		switch (wParam)
		{
		case VK_UP:
		{
			Y = Y - Speed;
			if (Y < 0) 
			{
				Y = 0;
			}
			break;
		}
		case VK_DOWN:
		{
			Y = Y + Speed;
			if (Y + SizeY > Rect.bottom)
			{
				Y = Rect.bottom - SizeY;
			}
			break;
		}
		case VK_LEFT:
		{
			X = X - Speed;
			if (X < 0)
			{
				X = 0;
			}
			break;
		}
		case VK_RIGHT:
		{
			X = X + Speed;
			if (X + SizeX > Rect.right)
			{
				X = Rect.right - SizeX;
			}
			break;
		}
		default:
			break;
		}
		Rect.left = X - Speed;
		Rect.right = X + SizeX + Speed;
		Rect.top = Y - Speed;
		Rect.bottom = Y + SizeY + Speed;
		InvalidateRect(hWnd, &Rect, true);
		return 0;

	case WM_TIMER:
		GetClientRect(hWnd, &Rect);	

		if (DirectionFlagX == 1)
		{
			X = X + Speed;
			if (SizeX + X > Rect.right)
			{
				X = Rect.right - SizeX;
				DirectionFlagX = 0;
			}
		}
		else
		{
			X = X - Speed;
			if (X < 0)
			{
				X = 0;
				DirectionFlagX = 1;
			}
		}

		if (DirectionFlagY == 1)
		{
			Y = Y + Speed;
			if (SizeY + Y > Rect.bottom)
			{
				Y = Rect.bottom - SizeY;
				DirectionFlagY = 0;
			}
		}
		else
		{
			Y = Y - Speed;
			if (Y < 0)
			{
				Y = 0;
				DirectionFlagY = 1;
			}
		}

		Rect.left = X - Speed;
		Rect.right = X + SizeX + Speed;
		Rect.top = Y - Speed;
		Rect.bottom = Y + SizeY + Speed;
		InvalidateRect(hWnd, &Rect, true);
		return 0;

	case WM_GETMINMAXINFO:
		{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 300;
		}
		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &Rect);
		if (SizeX + X > Rect.right)
		{
			X = Rect.right - SizeX;
		}
		if (SizeY + Y > Rect.bottom)
		{
			Y = Rect.bottom - SizeY;
		}
		InvalidateRect(hWnd, &Rect, true);
		return 0;



	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	HWND hMainWindow;
	WNDCLASSEX wcex;
	MSG msg;

	srand(time(NULL));



	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_HIGHLIGHT + 1);//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = ClassName;
	RegisterClassEx(&wcex);

	hMainWindow = CreateWindowEx(0, ClassName, L"Lab_1", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 300, 300, 1500, 1000, 0, 0, hInstance, NULL);

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	InitializeGlobalVariables(hMainWindow);
	SetTimer(hMainWindow, NULL, TimerDelay, (TIMERPROC)NULL);

	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}