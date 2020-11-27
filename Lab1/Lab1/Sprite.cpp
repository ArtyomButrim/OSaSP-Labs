#include <Windows.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include "commctrl.h"

#define STRING_LENGTH 120

TCHAR WinName[] = _T("WinFrame");

HINSTANCE hInst;
HBITMAP hBitMap;
int left = 400;
int top = 200;
int size = 128;
bool isImage = false;
int sx, sy;

ATOM MyRegisterClass2(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MyRegisterClass2(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass2(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.hInstance = hInstance;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClass(&wc))
	{
		return 0;
	}

	return RegisterClass(&wc);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindow(WinName, _T("Каркас Windows-приложения"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void DrawRectangle(HDC hdc, HBRUSH hBrush, HWND hWnd)
{
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, left + size, top+size);
	ValidateRect(hWnd, NULL);
}
void MoveLeft(HWND hWnd)
{
	left = left - 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	if (left <= 0)
	{
		for (int i = 0; i < 8; i++)
		{
			left += 5;
			InvalidateRect(hWnd, NULL, true);
			Sleep(15);
			UpdateWindow(hWnd);
		}
	}
}

void MoveUp(HWND hWnd)
{
	top = top - 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	if (top <= 0)
	{
		for (int i = 0; i < 8; i++)
		{
			top += 5;
			InvalidateRect(hWnd, NULL, true);
			Sleep(15);
			UpdateWindow(hWnd);
		}
	}
}

void MoveRight(HWND hWnd)
{
	left += 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	if (left + size >= sx)
	{
		for (int i = 0; i < 8; i++)
		{
			left -= 5;
			InvalidateRect(hWnd, NULL, true);
			Sleep(15);
			UpdateWindow(hWnd);
		}
	}
}

void MoveDown(HWND hWnd)
{
	top += 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	if (top+size >= sy)
	{
		for (int i = 0; i < 8; i++)
		{
			top -= 5;
			InvalidateRect(hWnd, NULL, true);
			Sleep(15);
			UpdateWindow(hWnd);
		}
	}
}

void ShowImage(HDC hdc, HBITMAP hBitMap)
{
	BITMAP bitMap;
	HGDIOBJ oldBitMap;
	HDC hdcMem = CreateCompatibleDC(hdc);
	oldBitMap = SelectObject(hdcMem, hBitMap);
	GetObject(hBitMap, sizeof(bitMap), &bitMap);
	BitBlt(hdc, left, top, left + size, top + size, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitMap);
	DeleteDC(hdcMem);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			hBitMap = (HBITMAP)LoadImage(NULL, _T("D:\\test.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HDC hdc = BeginPaint(hWnd, &paint);
			if (isImage == false)
			{
				DrawRectangle(hdc, hBrush, hWnd);
			}
			else
			{
				ShowImage(hdc, hBitMap);
			}		
			EndPaint(hWnd, &paint);
			UpdateWindow(hWnd);
		}
		break;
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_LEFT:
				{
					MoveLeft(hWnd);
					break;
				}
				case VK_UP:
				{
					MoveUp(hWnd);
					break;
				}
				case VK_RIGHT:
				{
					MoveRight(hWnd);
					break;
				}
				case VK_DOWN:
				{
					MoveDown(hWnd);
					break;
				}
				case VK_SPACE:
				{
					if (isImage == false)
					{
						isImage = true;
					}
					else
					{
						isImage = false;
					}
					UpdateWindow(hWnd);
					break;
				}
			}
			break;
		case WM_MOUSEWHEEL:
		{
			short key = GET_KEYSTATE_WPARAM(wParam);
			short mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);

			if (key == MK_SHIFT)
			{
				if (mouseWheel < 0)
				{
					MoveRight(hWnd);
				}
				else
				{
					MoveLeft(hWnd);
				}
			}
			else
			{
				if (mouseWheel < 0)
				{
					MoveUp(hWnd);
				}
				else
				{
					MoveDown(hWnd);
				}
			}
		}
		break;
		case WM_SIZE:
		{
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}