#include <iostream>
#include <Windows.h>
#include <tchar.h>

TCHAR WinName[] = _T("WinFrame");

HINSTANCE hInst;

const int numberOfColumns = 4;
const int numberOfRows = 5;
int x = 0;
int y = 0;
int sx = 0;
int sy = 0;
char slist[numberOfRows * numberOfColumns][255];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyRegisterClass(hInstance);

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

ATOM MyRegisterClass(HINSTANCE hInstance)
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
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);

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

void DrawTable(HWND hWnd, int sx, int sy, LPPOINT lppt)
{
    PAINTSTRUCT ps;
    COLORREF color = RGB(71, 163, 64);
    HPEN hBrush = CreatePen(PS_INSIDEFRAME, 3, color);
    HDC hdc = BeginPaint(hWnd, &ps);
    SelectObject(hdc, hBrush);

    for (int i = 0; i < 5; i++)
    {
        MoveToEx(hdc, x, y, lppt);
        LineTo(hdc, x, sy);
        x += sx / 4;

    }
}

void Draw(HWND hWnd, int x, int y)
{
    int columnWidth, tableHeight = 0;
    RECT rect;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    columnWidth = sx / numberOfColumns;

    for (int i = 0; i < numberOfRows; i++)
    {
        int maxWidth = 0;

        for (int j = 0; j < numberOfColumns; j++)   
        {
            SetTextColor(hdc, RGB(255,0,0));

            LPCSTR str = (LPCSTR)slist[numberOfColumns * i + j];
            int length = strlen(slist[numberOfColumns * i + j]);


            SetRect(&rect, j * columnWidth + 1, tableHeight + 1, (j + 1) * columnWidth - 1, sy);
            int strHeight = DrawTextA(hdc, str, length, &rect, DT_WORDBREAK);
            if (strHeight > maxWidth)
            {
                maxWidth = strHeight;
            }
        }

        tableHeight += maxWidth;

        MoveToEx(hdc, 0, tableHeight, NULL);
        LineTo(hdc, sx, tableHeight);
    }

    for (int i = 1; i < numberOfColumns; i++)
    {
        int columnPosition = i * columnWidth;
        MoveToEx(hdc, columnPosition,0, NULL);
        LineTo(hdc, columnPosition, tableHeight);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            const TCHAR filePath[] = _T("D:/text.txt");
            HANDLE hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE == hFile) {
                MessageBox(hWnd, _T("File not found!"), _T("Error"), MB_OK);
            }
            else {
                const int num = 4 * 6;

                for (int i = 0; i < num; i++)
                {
                    char cBufferText[255];
                    DWORD dwByte;
                    DWORD numOfBytesToRead = rand() % 254;
                    ReadFile(hFile, cBufferText, numOfBytesToRead, &dwByte, NULL);
                    cBufferText[numOfBytesToRead] = 0;

                    strcpy_s(slist[i], sizeof(cBufferText), cBufferText);
                }

                CloseHandle(hFile);
            }
        }
        break;
        case WM_PAINT:
        {
            Draw(hWnd, sx, sy);
            break;
        }
        case WM_SIZE:
        {
            sx = LOWORD(lParam);
            sy = HIWORD(lParam);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}