// JaProj.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "JaProj.h"
#include<locale.h>
#include<commdlg.h>
#include<thread>
#include<vector>
#include<shobjidl.h> 
#include<stdlib.h>
#include<WinUser.h>
#include <commctrl.h>
#include<chrono>
#include <string> 
#include"EasyBMP.h"
//#include"EasyBMP_VariousBMPutilities.h"
#include"EasyBMP.cpp"
#include"EasyBMP_BMP.h"
#include"EasyBMP_DataStructures.h"
#include"..\Cpp\Sepia.h"
#pragma comment(lib,"comctl32.lib")
#define MAX_LOADSTRING 100
typedef struct dataForSepia{ // struktura przechowująca dane odnośnie wybranych opcji w oknie
    int cpp = 0; //dla przycisku Cpp
    int Asm = 0;// dla przycisku Asm
    int thread=0;// dla przechowania ilości wątków
    std::string ofile="";// do przechowania ścieżki do pliku, który poddajemy zmianom

}Data;
// Zmienne globalne:
Data data;                                      //struktura przechowująca szczegóły odnośnie formy uruchomienia programu
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
HWND userThread;
// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
long long Mfunction();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.
    

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JAPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JAPROJ));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JAPROJ));
    wcex.hCursor        = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JAPROJ);
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
      150, 50, 400, 350, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hButton1, hButton2;
    static HWND hEdit; // kontrolka edit do przechowywania tekstu
    HWND slider;
    HWND hLeftLabel;
    HWND hRightLabel;
   // HWND userThread;
    static OPENFILENAME ofn; // struktura związana z obsługą okna otwierania pliku i zapisywania do pliku

    static TCHAR FileName[MAX_PATH]; // zmienna, która przechowywać będzie nazwę pliku wraz z ścieżką
    static TCHAR TitleName[MAX_PATH]; // zmienna, która będzie przechowywała nazwę pliku
   
    //Do obsługi okna otwarcia pliku
    TitleName[0] = 0; // ustaiwam, żeby rzadnych śmieci nie było
    FileName[0] = 0; // to samo co poprzednio
    //do wyboru wątków
   
    
 
    switch (message)
    {
    case WM_CREATE:
     //wybór implementacji
        hButton1 = CreateWindowEx(LWS_TRANSPARENT, L"button", L"Cpp", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 150, 10, 70, 30, hWnd, (HMENU)IDC_CPP, NULL, NULL);
        hButton2 = CreateWindowEx(LWS_TRANSPARENT, L"button", L"ASM", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 250, 10, 70, 30, hWnd, (HMENU)IDC_ASM, NULL, NULL);
        //wybór illości watków
         userThread= CreateWindowW(L"Edit", L"1", WS_VISIBLE|WS_CHILD| WS_BORDER,100,55,20,30,hWnd,(HMENU)IDC_SCROLL, NULL, NULL);
        //przycisk do otwarcia pliku, na którym będzie zmiana
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        MoveWindow(hEdit, 0, 0, rcClient.right, rcClient.bottom - 30, TRUE);
        CreateWindowEx(0, L"BUTTON", L"Otwórz", WS_CHILD | WS_VISIBLE | WS_BORDER,
            5, rcClient.bottom - 27, 50, 25, hWnd, (HMENU)ID_PLIK_OTWORZ, hInst, NULL);
        //Przycisk do rozpoczęcia działania programu
        CreateWindowEx(0, L"button", L"Rozpocznij", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, rcClient.bottom - 27, 80, 25, hWnd, (HMENU)ID_ROZPOCZNIJ, hInst, NULL);
       // CreateT(hWnd);
    
    case WM_COMMAND:
        {

            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDC_CPP:
            {
                CheckRadioButton(hWnd, IDC_CPP, IDC_ASM, IDC_CPP);
                data.cpp = 1;
                data.Asm = 0;
            }
            break;
            case IDC_ASM:
            {
                CheckRadioButton(hWnd, IDC_CPP, IDC_ASM, IDC_ASM);
                data.Asm = 1;
                data.cpp = 0;
            }
            break;
            case ID_PLIK_OTWORZ:
            {
                char* filePath= new char[128];
                std::string file;
                HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                    COINIT_DISABLE_OLE1DDE);
                if (SUCCEEDED(hr))
                {
                    IFileOpenDialog* pFileOpen;

                    // Create the FileOpenDialog object.
                    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                    if (SUCCEEDED(hr))
                    {
                        // Show the Open dialog box.
                        hr = pFileOpen->Show(NULL);

                        // Get the file name from the dialog box.
                        if (SUCCEEDED(hr))
                        {
                            IShellItem* pItem;
                            hr = pFileOpen->GetResult(&pItem);
                            if (SUCCEEDED(hr))
                            {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                size_t charsConverted = 0;
                                wcstombs_s(&charsConverted, filePath,128, pszFilePath,128);
                                file = filePath;
                                if (file.rfind(".bmp") != file.npos)
                                { 
                                    CreateWindowW(L"static", L"\u2713", WS_VISIBLE | WS_CHILD | SS_CENTER, 70, 265, 20, 20, hWnd, NULL, NULL, NULL);
                                    if (SUCCEEDED(hr))
                                    {
                                        MessageBoxW(NULL, pszFilePath, L"Wybrano plik", MB_OK);
                                        data.ofile = file;
                                        CoTaskMemFree(pszFilePath);
                                    }
                                }
                                else
                                    MessageBoxW(NULL, L"Proszę podać obraz .bmp", L"Błędny plik!", MB_OK);
                                // Display the file name to the user.
                                
                                pItem->Release();
                            }
                        }
                        pFileOpen->Release();
                    }
                    CoUninitialize();
                }
                
            }
            break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_ROZPOCZNIJ:// reakcja na naciśnięcie przycisku Rozpocznij
            { 

                wchar_t text[3];
                GetWindowTextW(userThread,text,3);
                data.thread=_wtoi(text);
                if((data.Asm==0 && data.cpp==0)|| data.ofile==""||data.thread==0)
                    MessageBoxW(NULL, L"Proszę uzupełnić dane", L"Ups..", MB_OK);
                else if (data.thread < 1 || data.thread>64)
                    MessageBoxW(NULL, L"Zła liczba wątków", L"Ups..", MB_OK);
                else
                {
                     long double czas= Mfunction();
                    std::string grrr = std::to_string(czas);
                    std::wstring widestr = std::wstring(grrr.begin(), grrr.end());
                   const wchar_t* czasik = widestr.c_str();
                   if (czas == -1)
                   {
                       MessageBoxW(NULL, L"Liczba wątków przekracza rozmiar obrazu", L"", MB_OK);
                       break;
                   }
                    CreateWindowW(L"Static", czasik, WS_VISIBLE | WS_CHILD, 220, 100, 60, 30, hWnd, NULL, NULL, NULL);
                }
              

            }
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:/// wyświetlanie komunikatów na oknie
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc,5, 15,_T("Wybór implementacji:"), _tcslen(_T("Wybór implementacji:")));
            TextOut(hdc, 5, 60, _T("Ilość wątków:"), _tcslen(_T("Ilość wątków:")));
            TextOut(hdc, 5, 100, _T("Czas trwania w milisekundach:"), _tcslen(_T("Czas trwania w milisekundach:")));
            // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
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

long long Mfunction()
{
    
    HINSTANCE dllHandle = NULL;
    dllHandle = LoadLibrary(L"JAAsm.dll");
    MyProc1 procedura = (MyProc1)GetProcAddress(dllHandle, "MyProc1");

    setlocale(LC_CTYPE, "");
    BMP* bmp = new BMP();
    std::string str_obj(data.ofile);
    const char* file=&str_obj[0];
    bmp->ReadFromFile(file);
    if (data.thread > bmp->TellWidth())  // za duża liczba wątków w porównaniu do wielkości obrazu
        return -1;
    int range = bmp->TellWidth() / data.thread;// w zakresie znajduje się liczba wierszy którą przyjmuje dany wątek
    if (bmp->TellWidth() % data.thread)
        range += 1;//dodanie wiersza do każdego wątku, gdy dzielene z resztą- w ostanim wątku będzie o jeden mniej wiersz
    
   int rows = bmp->TellHeight();
   std::chrono::steady_clock::time_point end;// koniec pomiaru czasu
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();// początek mierzenia czasu
   
    if (data.Asm)
    {
        int counter = 1;
        std::vector<std::thread> threads;
        //utworzneie tablicy wątków, każdy z nich zmienia określony zakres tablicy. Indeks poczatkowy zmienianej komórki bitmapy jest w zmiennej k,
        //zmienna range określa ile wierszy będzie przetworzone orzez dany wątek
        //Dodatkowa zmienna counter pomaga w przesunięciu indeksu do którego wykonuje swoją prace dany wątek
        for (int k =0; k <bmp->TellWidth() ; k += range)
        {
            
            int progress = range * counter;// zmienna przechowuje indeks końcowej kolumny dla danego wątku
            

            //warunek w przypadku, gdzie progress przekroczy wielkość tablicy- stanie się tak kiedy po podzieleniu liczby kolumn na wątki będzie reszta
            if(progress>=bmp->TellWidth())
                threads.push_back(std::thread(procedura, rows, bmp->GetTable(),k,bmp->TellWidth()));

           else
           threads.push_back(std::thread(procedura, rows, bmp->GetTable(),k, progress));
                
                counter++;
          
        }
        for (auto& t : threads)
        {
            t.join();
        }
        end = std::chrono::steady_clock::now();
     }
     else if (data.cpp)
     {
        int counter = 1;
         std::vector<std::thread> threads;
         for (int k = 0; k < bmp->TellWidth(); k += range)
         {
             int progress = range * counter;// zmienna przechowuje indeks końcowej kolumny dla danego wątku


           //warunek w przypadku, gdzie progress przekroczy wielkość tablicy- stanie się tak kiedy po podzieleniu liczby kolumn na wątki będzie reszta
             if (progress >= bmp->TellWidth())
             // nie tu porównania takiego jak dla wątków dla asm, ponieważ to porównanie zostało zeralizowane w funkcji Sepia
             threads.push_back(std::thread(Sepia, bmp->GetTable(), k, bmp->TellWidth(), bmp->TellHeight()));
             else
                 threads.push_back(std::thread(Sepia, bmp->GetTable(), k, progress, bmp->TellHeight()));

             counter++;

         }
         for (auto& t : threads)
         {
             t.join();
         }
         end = std::chrono::steady_clock::now();
     }
    
    bmp->WriteToFile("sepia.bmp");// zapis zmienionego obrazu do pliku ustalonego z góry
    bmp->~BMP();
    long long time = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
    return time;
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
