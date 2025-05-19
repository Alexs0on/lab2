#include "framework.h"
#include "WindowsProject8.h"
#include <set>
#include <string>
#include <sstream>
#include <map>
#include <iostream>


using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND htext;
HWND hEdit;
HWND hButton;
HWND hOutput;
HFONT hFontOutput;



using Relation = set<pair<string, string>>;
using Domain = set<string>;

Relation parseRelationSet(const wstring& input) {
    Relation relation;
    wistringstream stream(input);
    wstring token;

    while (stream >> token) {
        if (token.length() != 2) {
            MessageBoxW(NULL, (L"Неверный формат пары: " + token).c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
            continue;
        }

        string first(1, (char)token[0]);
        string second(1, (char)token[1]);
        relation.insert({ first, second });
    }

    return relation;
}

Domain extractDomain(const Relation& rel) {
    Domain dom;
    for (auto& p : rel) {
        dom.insert(p.first);
        dom.insert(p.second);
    }
    return dom;
}


bool isReflexive(const Relation& rel, const Domain& dom) {
    for (const auto& elem : dom) {
        if (rel.find({ elem, elem }) == rel.end())
            return false;
    }
    return true;
}

bool isSymmetric(const Relation& rel) {
    for (const auto& pair : rel) {
        const string& a = pair.first;
        const string& b = pair.second;
        if (rel.find({ b, a }) == rel.end())
            return false;
    }
    return true;
}

bool isAntisymmetric(const Relation& rel) {
    for (const auto& pair : rel) {
        const string& a = pair.first;
        const string& b = pair.second;
        if (a != b && rel.find({ b, a }) != rel.end())
            return false;
    }
    return true;
}

bool isTransitive(const Relation& rel) {
    for (const auto& pair1 : rel) {
        const string& a = pair1.first;
        const string& b = pair1.second;
        for (const auto& pair2 : rel) {
            const string& c = pair2.first;
            const string& d = pair2.second;
            if (b == c && rel.find({ a, d }) == rel.end())
                return false;
        }
    }
    return true;
}

wstring getProperties(const Relation& rel) {
    Domain dom = extractDomain(rel);
    wstring result;

    result += L"Рефлексивность: " + wstring(isReflexive(rel, dom) ? L"Да" : L"Нет") + L"\r\n";
    result += L"Симметричность: " + wstring(isSymmetric(rel) ? L"Да" : L"Нет") + L"\r\n";
    result += L"Антисимметричность: " + wstring(isAntisymmetric(rel) ? L"Да" : L"Нет") + L"\r\n";
    result += L"Транзитивность: " + wstring(isTransitive(rel) ? L"Да" : L"Нет") + L"\r\n";

    return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT8);
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

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   hFontOutput = CreateFontW(
       26,
       0,
       0,
       0,
       FW_NORMAL,
       FALSE,
       FALSE,
       FALSE,
       DEFAULT_CHARSET,
       OUT_DEFAULT_PRECIS,
       CLIP_DEFAULT_PRECIS,
       DEFAULT_QUALITY,
       DEFAULT_PITCH | FF_DONTCARE,
       L"Segoe UI"
   );

   htext = CreateWindowW(L"STATIC", L"Введите пары через пробел пары элементов: ab ba aa bb", WS_CHILD | WS_VISIBLE | WS_BORDER,
       50, 10, 400, 30, hWnd, NULL, hInstance, NULL);

   hEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
       50, 50, 700, 30, hWnd, NULL, hInstance, NULL);

   hButton = CreateWindowW(L"BUTTON", L"Вычислить", WS_CHILD | WS_VISIBLE,
       50, 90, 100, 30, hWnd, (HMENU)1, hInstance, NULL);

   hOutput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
       50, 130, 1500, 400, hWnd, NULL, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SendMessage(hOutput, WM_SETFONT, (WPARAM)hFontOutput, TRUE);
   return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (LOWORD(wParam) == 1) {
            WCHAR buffer[1024];
            GetWindowTextW(hEdit, buffer, 1024);
            Relation rel = parseRelationSet(buffer);

            if (rel.empty()) {
                SetWindowTextW(hOutput, L"Отношение пусто или введено неверно.");
            }
            else {
                wstring result = getProperties(rel);
                SetWindowTextW(hOutput, result.c_str());
            }
        }
        break;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
