#include <Windows.h>
#include <tchar.h>


#define STRING_LENGTH 120

TCHAR WinName[] = _T("WinFrame");

HINSTANCE hInst;
int left = 400;
int top = 200;
int size = 128;

ATOM MyRegisterClass2(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
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

	return 0;
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
}

void MoveUp(HWND hWnd)
{
	top = top - 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
}

void MoveRight(HWND hWnd)
{
	left += 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
}

void MoveDown(HWND hWnd)
{
	top += 10;
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hBitmap;
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HDC hdc = BeginPaint(hWnd, &paint);
			DrawRectangle(hdc, hBrush, hWnd);
			EndPaint(hWnd, &paint);
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
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}