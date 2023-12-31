﻿// WindowsProject2.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "WindowsProject2.h"

#define MAX_LOADSTRING 100


const int ELEVATOR_LEFT = 400;
const int ELEVATOR_RIGHT = 600;
const int ELEVATOR_TOP = 650;
const int ELEVATOR_BOTTOM = 800;





// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
Elevator elevatorInst;
std::vector<Person> floorQueue[5];
// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const ULONGLONG interval = 10;

void ButtonPress(int button_id, HWND hWnd) {
    int destination = button_id % 10;
    int origin = button_id / 10;
    int x = 20*floorQueue[origin].size() + (origin % 2) * 900;
    int y = ELEVATOR_BOTTOM-(origin / 2) *2* DISTANCE_BETWEEN_FLOORS - (origin % 2) * DISTANCE_BETWEEN_FLOORS - 132/2;
    Person person(destination,origin,x,y);
    floorQueue[origin].push_back(person);
    elevatorInst.addToQueue(origin);
    ValidateRect(hWnd, NULL);
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
    Pen red(Color(255, 255, 0, 0), 5);
    
    for (int f = 0; f < 5; f++) {
        if (f % 2) {
            graphics.DrawLine(&pen, 600, 200 + DISTANCE_BETWEEN_FLOORS * f, 1000, 200 + DISTANCE_BETWEEN_FLOORS * f);
        }
        else {
            graphics.DrawLine(&pen, 0, 200 + DISTANCE_BETWEEN_FLOORS * f, 400, 200 + DISTANCE_BETWEEN_FLOORS * f);
        }
    }
    
    // dodac wektor "kolejke" ktorego wierzcholek bedzie rowny offset_y i jesli jego floor == destination to sciaga wierzcholek ze wektora. vektor bedzie sortowany wedlug zasady, jesli isAscending == true to wszystkie destynacje powyzej aktualnego poziomu dostaja pierwszenstwo nad tymi ktore chca jechac w dol itd.


    int offset_y = elevatorInst.GetPositionY();    
    graphics.DrawLine(&red, ELEVATOR_LEFT, ELEVATOR_BOTTOM - offset_y, ELEVATOR_RIGHT, ELEVATOR_BOTTOM - offset_y);
    graphics.DrawLine(&red, ELEVATOR_LEFT, ELEVATOR_TOP - offset_y, ELEVATOR_RIGHT, ELEVATOR_TOP - offset_y);
    graphics.DrawLine(&red, ELEVATOR_LEFT, ELEVATOR_BOTTOM - offset_y, ELEVATOR_LEFT, ELEVATOR_TOP - offset_y);
    graphics.DrawLine(&red, ELEVATOR_RIGHT, ELEVATOR_BOTTOM - offset_y, ELEVATOR_RIGHT, ELEVATOR_TOP - offset_y);

    for (int i = 0; i < 5; i++) {
        for (auto& peep : floorQueue[i]) {
            Bitmap PersonImg(L"person.png");
            Rect PersonSpace(peep.x, peep.y, PersonImg.GetWidth() / 2, PersonImg.GetHeight() / 2);
            graphics.DrawImage(&PersonImg, PersonSpace);
        }
    }
    
    for (auto& peep : elevatorInst.peopleInElevator) {
        Bitmap PersonImg(L"person.png");
        Rect PersonSpace(peep.x, peep.y, PersonImg.GetWidth() / 2, PersonImg.GetHeight() / 2);
        graphics.DrawImage(&PersonImg, PersonSpace);
    }
    TextOut(hdc, 0, 0, L"Aktualna Waga:", 14);

    wchar_t buffer[256];
    int weight = elevatorInst.GetWeight();
    if (!weight) TextOut(hdc, 105, 0, L"0", 1);
    else {
        wsprintfW(buffer, L"%d", weight);
        int digits = floor(log10(weight) + 1) + 1;
        TextOut(hdc, 105, 0, buffer, digits);
    }
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    SetTimer(hWnd, 1, interval, NULL);
    for (int f = 0; f <= 4; f++) {
        for (int button_i = 4; button_i >= 0; button_i--) {

           int floor = abs(f - 4);
           if (floor == button_i) continue;
           wchar_t buffer[256];
           wsprintfW(buffer, L"%d", button_i);
           HWND hwndButton = CreateWindow(
               L"BUTTON",  // Predefined class; Unicode assumed 
               buffer,      // Button text 
               WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
               10 + (f % 2) * 900,         // x position 
               200 + (f / 2) * 2 * DISTANCE_BETWEEN_FLOORS - (button_i+2) * 20 +(f%2)* DISTANCE_BETWEEN_FLOORS,// y position 
               20,        // Button width
               20,        // Button height
               hWnd,     // Parent window
               (HMENU)(floor*10+button_i), //first digit origin, second digit destination
               (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
               NULL);      // Pointer not needed.
       }
   }
    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"RESET",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        200,         // x position 
        0,// y position 
        50,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_RESET, //first digit origin, second digit destination
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

   ShowWindow(hWnd, 3);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static ULONGLONG lastActivity = GetTickCount64();
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            for (int i = 0; i < 44; i++) {
                if (wmId == i) ButtonPress(wmId, hWnd);
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
            case ID_RESET:
                elevatorInst.Clear();
                for (int i = 0; i < 5; i++) floorQueue[i].clear();
                lastActivity = 0;
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER: {
        elevatorInst.Movement(floorQueue);
        InvalidateRect(hWnd, NULL, TRUE);
        if (GetTickCount64() - lastActivity >= 5000 && elevatorInst.peopleInElevator.empty() && elevatorInst.queue.empty()) {
            int count = 0;
            for (int i = 0; i < 5; i++) if (floorQueue[i].empty()) count++;
            if (count == 4) elevatorInst.addToQueue(0);
        }
        return 0;
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
            // create memory DC and memory bitmap where we shall do our drawing

            HDC memDC = CreateCompatibleDC(hdc);

            // get window's client rectangle. We need this for bitmap creation.
            RECT rcClientRect;
            GetClientRect(hWnd, &rcClientRect);

            // now we can create bitmap where we shall do our drawing
            HBITMAP bmp = CreateCompatibleBitmap(hdc,
            rcClientRect.right - rcClientRect.left,
            rcClientRect.bottom - rcClientRect.top);

            // we need to save original bitmap, and select it back when we are done,
            // in order to avoid GDI leaks!
            HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);
            FillRect(memDC, &rcClientRect, (HBRUSH)(COLOR_WINDOW + 1));
            PaintScenery(memDC);
            
            BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left,
                rcClientRect.bottom - rcClientRect.top, memDC, 0, 0, SRCCOPY);

            // all done, now we need to cleanup
            SelectObject(memDC, oldBmp); // select back original bitmap
            DeleteObject(bmp); // delete bitmap since it is no longer required
            DeleteDC(memDC);   // delete memory DC since it is no longer required
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
        //return DefWindowProc(hWnd, message, wParam, lParam);
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
