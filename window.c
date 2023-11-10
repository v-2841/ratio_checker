// gcc -w -mwindows test.c -o test.exe
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void DropFile(HDROP hdrop)
{
    // обработка 1 файла
    char fname[MAX_PATH];
    int cnt = DragQueryFile(hdrop, 0xffffffff, NULL, 0);
    for (int i = 0; i < cnt; i++)
    {
        DragQueryFile(hdrop, i, fname, MAX_PATH);
        printf("%s\n", fname);
    }
    DragFinish(hdrop);
}

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_DESTROY) PostQuitMessage(0);
    else if (message == WM_DROPFILES) DropFile(wparam);
    // else if (message == WM_PAINT)
    // {
    //     PAINTSTRUCT ps;
    //     HDC hdc = BeginPaint(hwnd, &ps);
    //     EndPaint(hwnd, &ps);
    // }
    else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        // обработка
    }
    WNDCLASSA wlc;
    memset(&wlc, 0, sizeof(WNDCLASSA));
    wlc.lpszClassName = "window";
    wlc.lpfnWndProc = WndProc;
    wlc.hbrBackground = CreateSolidBrush(RGB(255, 252, 231));
    RegisterClassA(&wlc);
    HWND hwnd;
    hwnd = CreateWindow("window", "Ratio checker", WS_OVERLAPPEDWINDOW,
                        10, 10, 640, 480, NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    DragAcceptFiles(hwnd, TRUE);
    MSG msg;

    HWND acid_name = CreateWindow("static", "Acid", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 10, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c120_name = CreateWindow("static", "C12:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 30, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c140_name = CreateWindow("static", "C14:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 50, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c160_name = CreateWindow("static", "C16:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 70, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c180_name = CreateWindow("static", "C18:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 90, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c181c_name = CreateWindow("static", "C18:1", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 110, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c181t_name = CreateWindow("static", "C18:1", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 130, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c182c_name = CreateWindow("static", "C18:2", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 150, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c182t_name = CreateWindow("static", "C18:2", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 170, 80, 20, hwnd, NULL, NULL, NULL);
    HWND ratio_name = CreateWindow("static", "Ratio", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 210, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio1_name = CreateWindow("static", "C16:0 : C12:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 230, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio2_name = CreateWindow("static", "C18:0 : C12:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 250, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio3_name = CreateWindow("static", "C18:1 : C14:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 270, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio4_name = CreateWindow("static", "C18:2 : C14:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 290, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio5_name = CreateWindow("static", "C18:1 + C18:2 : C12:0 + C14:0 + C16:0 + C18:0", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 310, 310, 20, hwnd, NULL, NULL, NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    return 0;
}