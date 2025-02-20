// nativize-webview2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "nativize-webview2.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Microsoft::Web::WebView2::Core;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CoreWebView2Controller webviewController {nullptr};
CoreWebView2 webview{nullptr};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                InitWebView(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NATIVIZEWEBVIEW2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NATIVIZEWEBVIEW2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NATIVIZEWEBVIEW2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NATIVIZEWEBVIEW2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    InitWebView(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (webviewController != nullptr) {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            webviewController.Bounds(Rect (
                static_cast<float>(bounds.left),
                static_cast<float>(bounds.top),
                static_cast<float>(bounds.right - bounds.left),
                static_cast<float>(bounds.bottom - bounds.top)
            ));
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

void InitWebView(HWND hWnd)
{
    auto options = CoreWebView2EnvironmentOptions();

    auto environment_created_handler = [hWnd](auto asyncInfo, auto asyncStatus) -> fire_and_forget {
        if (asyncStatus != AsyncStatus::Completed) {
            co_return;
        }

        auto environment = asyncInfo.GetResults();

        auto controller_created_handler = [hWnd](auto asyncInfo, auto asyncStatus) -> fire_and_forget {
            if (asyncStatus != AsyncStatus::Completed) {
                co_return;
            }

            auto controller = asyncInfo.GetResults();

            webviewController = controller;
            webview = webviewController.CoreWebView2();

            auto settings = webview.Settings();
            settings.IsScriptEnabled(true);
            settings.AreDefaultScriptDialogsEnabled(true);
            settings.IsWebMessageEnabled(true);

            RECT bounds;
            GetClientRect(hWnd, &bounds);
            webviewController.Bounds(Rect(
                static_cast<float>(bounds.left),
                static_cast<float>(bounds.top),
                static_cast<float>(bounds.right - bounds.left),
                static_cast<float>(bounds.bottom - bounds.top)
            ));

            webview.Navigate(L"https://www.github.com/nativize");
        };

        environment.CreateCoreWebView2ControllerAsync(CoreWebView2ControllerWindowReference::CreateFromWindowHandle(reinterpret_cast<uint64_t>(hWnd))).Completed(controller_created_handler);
    };

    CoreWebView2Environment::CreateWithOptionsAsync(L"", L"", options).Completed(environment_created_handler);
}