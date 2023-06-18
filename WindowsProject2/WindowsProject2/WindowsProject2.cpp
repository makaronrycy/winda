﻿// WindowsProject2.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "WindowsProject2.h"

#define MAX_LOADSTRING 100
#define FLOOR 4

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

class Elevator {
    int floor;
private:
    int max_weight;
    

};
class Person {
public:
    int weight;

};
void ButtonPress(int button_id,HWND hWnd) {
    InvalidateRect(hWnd, NULL, TRUE);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

    MSG msg;

    // Główna pętla komunikatów:
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


void PaintScenery(HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 0),5);
    for (int f = 0; f < 5; f++) {
        if (f % 2) {
            graphics.DrawLine(&pen, 600, 200 + 150 * f, 1000, 200 + 150 * f);
        }
        else {
            graphics.DrawLine(&pen, 0, 200 + 150 * f, 400, 200 + 150 * f);
        }
    }
    wchar_t buffer[256];
    wsprintfW(buffer, L"%d", GetTickCount64());
    TextOut(hdc, 0, 0, buffer, 8);
}
//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetTimer(hWnd, 1, 1000, NULL);
   for (int f = 0; f < 5; f++) {
       for (int button_i = 5; button_i > 0; button_i--){

           int floor = abs(f - 5);
           if (floor == button_i) continue;
           wchar_t buffer[256];
           wsprintfW(buffer, L"%d", button_i);
           HWND hwndButton = CreateWindow(
               L"BUTTON",  // Predefined class; Unicode assumed 
               buffer,      // Button text 
               WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
               10 + (f % 2) * 900,         // x position 
               200 +(f / 2) * 300 - button_i * 20 +(f%2)*150,         // y position 
               20,        // Button width
               20,        // Button height
               hWnd,     // Parent window
               (HMENU)(floor*10+button_i), //first digit origin, second digit destination
               (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
               NULL);      // Pointer not needed.
       }
       
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
int PressedButton = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);


            for (int i = 12; i < 55; i++) {
                if (wmId == i) ButtonPress(wmId, hWnd);
                PressedButton = wmId%10;
            }

            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...

            Graphics elevator(hdc);
            Pen red(Color(255, 255, 0, 0), 5);

            switch (PressedButton)
            {
            case (2):
                elevator.DrawLine(&red, 400, 650, 600, 650);  // dolna czesc
                elevator.DrawLine(&red, 400, 500, 600, 500);  // gorna czesc
                elevator.DrawLine(&red, 400, 650, 400, 500);  // lewa czesc
                elevator.DrawLine(&red, 600, 650, 600, 500);  // prawa czesc
                break;
            case 3:
                elevator.DrawLine(&red, 400, 500, 600, 500);  // dolna czesc
                elevator.DrawLine(&red, 400, 350, 600, 350);  // gorna czesc
                elevator.DrawLine(&red, 400, 500, 400, 350);  // lewa czesc
                elevator.DrawLine(&red, 600, 500, 600, 350);  // prawa czesc
                break;
            case 4:
                elevator.DrawLine(&red, 400, 350, 600, 350);  // dolna czesc
                elevator.DrawLine(&red, 400, 200, 600, 200);  // gorna czesc
                elevator.DrawLine(&red, 400, 350, 400, 200);  // lewa czesc
                elevator.DrawLine(&red, 600, 350, 600, 200);  // prawa czesc
                break;
            case 5:
                elevator.DrawLine(&red, 400, 200, 600, 200);  // dolna czesc
                elevator.DrawLine(&red, 400, 50, 600, 50);  // gorna czesc
                elevator.DrawLine(&red, 400, 200, 400, 50);  // lewa czesc
                elevator.DrawLine(&red, 600, 200, 600, 50);  // prawa czesc
                break;
            default:
                elevator.DrawLine(&red, 400, 800, 600, 800);  // dolna czesc
                elevator.DrawLine(&red, 400, 650, 600, 650);  // gorna czesc
                elevator.DrawLine(&red, 400, 800, 400, 650);  // lewa czesc
                elevator.DrawLine(&red, 600, 800, 600, 650);  // prawa czesc
                break;
            }

            //elevator.DrawLine(&red, 400, 800, 600, 800);  // dolna czesc
            //elevator.DrawLine(&red, 400, 650, 600, 650);  // gorna czesc
            //elevator.DrawLine(&red, 400, 800, 400, 650);  // lewa czesc
            //elevator.DrawLine(&red, 600, 800, 600, 650);  // prawa czesc
            //


            PaintScenery(hdc);
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

// Procedura obsługi komunikatów dla okna informacji o programie.
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