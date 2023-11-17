// gcc -mwindows -static -m32 ratio_checker_gui.c resource.res -o "Ratio Checker.exe" -l gdi32 -l comdlg32
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <tchar.h>

#define VERSION "1.2"
#define MAX_LINE_LENGTH 512
#define IDI_ICON1 101
#define PRECISION 2
#define IDM_OPEN_BUTTON 101
#define IDM_ONLINE_HELP 102
#define IDM_ABOUT 103
#define IDM_EXIT 999
#define MAX_SAMPLE_NAME_LENGTH 32
#define MAX_NUM_LENGTH 32

HWND sample_name_hwnd;
char sample_name[MAX_SAMPLE_NAME_LENGTH] = "";
double c120 = 0.0, c140 = 0.0, c160 = 0.0, c180 = 0.0, c181 = 0.0, c182 = 0.0;
HWND c120_area, c140_area, c160_area, c180_area, c181_area, c182_area;
double ratio1 = 0.0, ratio2 = 0.0, ratio3 = 0.0, ratio4 = 0.0, ratio5 = 0.0;
HWND ratio1_result, ratio2_result, ratio3_result, ratio4_result, ratio5_result;
char ratio1_pass[16], ratio2_pass[16], ratio3_pass[16], ratio4_pass[16],
    ratio5_pass[16];
HWND ratio1_comment, ratio2_comment, ratio3_comment, ratio4_comment,
    ratio5_comment;
char buffer[MAX_NUM_LENGTH];

void calculateRatios()
{
    ratio1 = (c120 != 0) ? c160 / c120 : 0;
    ratio2 = (c120 != 0) ? c180 / c120 : 0;
    ratio3 = (c140 != 0) ? c181 / c140 : 0;
    ratio4 = (c140 != 0) ? c182 / c140 : 0;
    ratio5 = (c120 + c140 + c160 + c180 != 0) ? (c181 + c182) / (
        c120 + c140 + c160 + c180) : 0;
}

int findAreaColumn(const char *line)
{
    const char *delim = "\t";
    char *token = strtok((char *)line, delim);
    int column = 0;
    while (token != NULL)
    {
        if (strcmp(token, "Area") == 0)
            return column;
        token = strtok(NULL, delim);
        column++;
    }
    return -1;
}

double extractArea(const char *line, int areaColumn)
{
    const char *delim = "\t";
    char *token = strtok((char *)line, delim);
    int column = 0;
    double area = 0.0;
    while (token != NULL)
    {
        if (column == areaColumn)
        {
            area = atof(token);
            break;
        }
        token = strtok(NULL, delim);
        column++;
    }
    return area;
}

void extractName(const char *line)
{
    const char *delim = "\t";
    char *token = strtok((char *)line, delim);
    while (token != NULL)
    {
        if (strcmp(token, "Sample Name") == 0)
        {
            token = strtok(NULL, delim);
            if (token != NULL)
                strncpy(sample_name, token, MAX_SAMPLE_NAME_LENGTH - 1);
        }
        token = strtok(NULL, delim);
    }
}

void readFile(char *fileAdress)
{
    c120 = c140 = c160 = c180 = c181 = c182 = 0;
    FILE *file = fopen(fileAdress, "r");
    char line[MAX_LINE_LENGTH];
    sample_name[0] = '\0';
    int areaColumn = -1;
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        extractName(line);
        if (strlen(sample_name) != 0)
            break;
    }
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        areaColumn = findAreaColumn(line);
        if (areaColumn != -1)
            break;
    }
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        if (strstr(line, "C12:0"))
            c120 = extractArea(line, areaColumn);
        else if (strstr(line, "C14:0"))
            c140 = extractArea(line, areaColumn);
        else if (strstr(line, "C16:0"))
            c160 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:0"))
            c180 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:1 trans"))
            c181 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:1 cis"))
            c181 += extractArea(line, areaColumn);
        else if (strstr(line, "C18:2 trans"))
            c182 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:2 cis"))
        {
            c182 += extractArea(line, areaColumn);
            break;
        }
    }
    fclose(file);
}

char *doubleToChar(double value)
{
    double roundedValue = round(value * pow(10, PRECISION)) / pow(
        10, PRECISION);
    _snprintf(buffer, sizeof(buffer) / sizeof(buffer[0]), "%.*f",
              PRECISION, roundedValue);
    return buffer;
}

void setResults()
{
    calculateRatios();

    SetWindowText(sample_name_hwnd, sample_name);

    SetWindowText(c120_area, doubleToChar(c120));
    SetWindowText(c140_area, doubleToChar(c140));
    SetWindowText(c160_area, doubleToChar(c160));
    SetWindowText(c180_area, doubleToChar(c180));
    SetWindowText(c181_area, doubleToChar(c181));
    SetWindowText(c182_area, doubleToChar(c182));

    SetWindowText(ratio1_result, doubleToChar(ratio1));
    SetWindowText(ratio2_result, doubleToChar(ratio2));
    SetWindowText(ratio3_result, doubleToChar(ratio3));
    SetWindowText(ratio4_result, doubleToChar(ratio4));
    SetWindowText(ratio5_result, doubleToChar(ratio5));

    strcpy(ratio1_pass,
           (ratio1 >= 5.8 && ratio1 <= 14.5) ? "Pass" : "Not pass");
    strcpy(ratio2_pass, (ratio2 >= 1.9 && ratio2 <= 5.9) ? "Pass" : "Not pass");
    strcpy(ratio3_pass, (ratio3 >= 1.6 && ratio3 <= 3.6) ? "Pass" : "Not pass");
    strcpy(ratio4_pass, (ratio4 >= 0.1 && ratio4 <= 0.5) ? "Pass" : "Not pass");
    strcpy(ratio5_pass, (ratio5 >= 0.4 && ratio5 <= 0.7) ? "Pass" : "Not pass");
    SetWindowText(ratio1_comment, ratio1_pass);
    SetWindowText(ratio2_comment, ratio2_pass);
    SetWindowText(ratio3_comment, ratio3_pass);
    SetWindowText(ratio4_comment, ratio4_pass);
    SetWindowText(ratio5_comment, ratio5_pass);
}

void DropFile(HDROP hdrop)
{
    char fname[MAX_PATH];
    DragQueryFile(hdrop, 0, fname, MAX_PATH);
    readFile(fname);
    setResults();
    DragFinish(hdrop);
}

void openFile(HWND hwnd)
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = sizeof(fileName);
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        readFile(fileName);
        setResults();
        SetFocus(hwnd);
    }
}

void showAboutDialog(HWND hwnd)
{
    char aboutMessage[MAX_LINE_LENGTH];
    sprintf(
        aboutMessage, "Ratio Checker v%s\n\n"
                      "Copyright (c) 2023 Vitaliy Pavlov\n"
                      "Source code: https://github.com/v-2841/ratio_checker",
        VERSION);
    MessageBox(hwnd, aboutMessage, "About", MB_OK);
}

void localHelp()
{
    ShellExecute(NULL, "open", "help.html", NULL, NULL, SW_SHOWNORMAL);
}

void menu(HWND hwnd)
{
    HMENU hMenu = CreateMenu();

    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, IDM_OPEN_BUTTON,
               "Open Data File...\tCtrl+O");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, "Exit\tAlt+F4");

    HMENU hHelpMenu = CreatePopupMenu();
    AppendMenu(hHelpMenu, MF_STRING, IDM_ONLINE_HELP, "Online manual\tF1");
    AppendMenu(hHelpMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hHelpMenu, MF_STRING, IDM_ABOUT, "About Ratio Checker...");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "Help");

    SetMenu(hwnd, hMenu);
}

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_CREATE)
        menu(hwnd);
    else if (message == WM_DROPFILES)
        DropFile((HDROP)wparam);
    else if (message == WM_COMMAND)
    {
        if (LOWORD(wparam) == IDM_OPEN_BUTTON)
            openFile(hwnd);
        else if (LOWORD(wparam) == IDM_ABOUT)
            showAboutDialog(hwnd);
        else if (LOWORD(wparam) == IDM_ONLINE_HELP)
            localHelp();
        else if (LOWORD(wparam) == IDM_EXIT)
            PostQuitMessage(0);
    }
    else if (message == WM_KEYDOWN)
    {
        if (LOWORD(wparam) == VK_F1)
            localHelp();
        else if (GetKeyState(VK_CONTROL) & 0x8000 && wparam == 'O')
            openFile(hwnd);
    }
    else if (message == WM_DESTROY)
        PostQuitMessage(0);
    else
        return DefWindowProcA(hwnd, message, wparam, lparam);
}

int main(int argc, char *argv[])
{
    LPRECT rctScr;
    rctScr = malloc(sizeof(*rctScr));
    GetClientRect(GetDesktopWindow(), rctScr);

    WNDCLASSA wlc;
    memset(&wlc, 0, sizeof(WNDCLASSA));
    wlc.lpszClassName = "window";
    wlc.lpfnWndProc = (WNDPROC)WndProc;
    wlc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 252, 231));
    wlc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wlc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
    RegisterClassA(&wlc);

    HWND hwnd;
    hwnd = CreateWindow(
        "window", "Ratio checker", WS_OVERLAPPEDWINDOW,
        (rctScr[0].right - 606) / 2, (rctScr[0].bottom - 419) / 2,
        606, 419, NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    DragAcceptFiles(hwnd, TRUE);
    free(rctScr);

    MSG msg;

    if (argc != 1)
        readFile(argv[1]);

    HWND sample_name_name_hwnd = CreateWindow(
        "static", "Sample name", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 20, 100, 20, hwnd, NULL, NULL, NULL);
    sample_name_hwnd = CreateWindow(
        "static", sample_name, WS_VISIBLE | WS_CHILD | WS_BORDER,
        120, 20, 180, 20, hwnd, NULL, NULL, NULL);

    HWND acid_name = CreateWindow(
        "static", "Acid", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 60, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c120_name = CreateWindow(
        "static", "C12:0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 80, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c140_name = CreateWindow(
        "static", "C14:0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 100, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c160_name = CreateWindow(
        "static", "C16:0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 120, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c180_name = CreateWindow(
        "static", "C18:0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 140, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c181_name = CreateWindow(
        "static", "C18:1", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 160, 80, 20, hwnd, NULL, NULL, NULL);
    HWND c182_name = CreateWindow(
        "static", "C18:2", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 180, 80, 20, hwnd, NULL, NULL, NULL);
    HWND area_name = CreateWindow(
        "static", "Area", WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 60, 100, 20, hwnd, NULL, NULL, NULL);
    HWND ratio_name = CreateWindow(
        "static", "Ratio", WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 220, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio1_name = CreateWindow(
        "static", "C16:0 : C12:0",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 240, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio2_name = CreateWindow(
        "static", "C18:0 : C12:0",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 260, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio3_name = CreateWindow(
        "static", "C18:1 : C14:0",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 280, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio4_name = CreateWindow(
        "static", "C18:2 : C14:0",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        20, 300, 310, 20, hwnd, NULL, NULL, NULL);
    HWND ratio5_name = CreateWindow(
        "static", "C18:1 + C18:2 : C12:0 + C14:0 + C16:0 + C18:0",
        WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 320, 310, 20,
        hwnd, NULL, NULL, NULL);
    HWND range_name = CreateWindow(
        "static", "Range", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 220, 80, 20, hwnd, NULL, NULL, NULL);
    HWND range1_name = CreateWindow(
        "static", "5.8 - 14.5", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 240, 80, 20, hwnd, NULL, NULL, NULL);
    HWND range2_name = CreateWindow(
        "static", "1.9 - 5.9", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 260, 80, 20, hwnd, NULL, NULL, NULL);
    HWND range3_name = CreateWindow(
        "static", "1.6 - 3.6", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 280, 80, 20, hwnd, NULL, NULL, NULL);
    HWND range4_name = CreateWindow(
        "static", "0.1 - 0.5", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 300, 80, 20, hwnd, NULL, NULL, NULL);
    HWND range5_name = CreateWindow(
        "static", "0.4 - 0.7", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        330, 320, 80, 20, hwnd, NULL, NULL, NULL);
    HWND result_name = CreateWindow(
        "static", "Result", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 220, 80, 20, hwnd, NULL, NULL, NULL);
    HWND comment_name = CreateWindow(
        "static", "Comment", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 220, 80, 20, hwnd, NULL, NULL, NULL);

    c120_area = CreateWindow(
        "static", doubleToChar(c120), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 80, 100, 20, hwnd, NULL, NULL, NULL);
    c140_area = CreateWindow(
        "static", doubleToChar(c140), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 100, 100, 20, hwnd, NULL, NULL, NULL);
    c160_area = CreateWindow(
        "static", doubleToChar(c160), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 120, 100, 20, hwnd, NULL, NULL, NULL);
    c180_area = CreateWindow(
        "static", doubleToChar(c180), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 140, 100, 20, hwnd, NULL, NULL, NULL);
    c181_area = CreateWindow(
        "static", doubleToChar(c181), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 160, 100, 20, hwnd, NULL, NULL, NULL);
    c182_area = CreateWindow(
        "static", doubleToChar(c182), WS_VISIBLE | WS_CHILD | WS_BORDER,
        100, 180, 100, 20, hwnd, NULL, NULL, NULL);

    calculateRatios();

    ratio1_result = CreateWindow(
        "static", doubleToChar(ratio1),
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 240, 80, 20, hwnd, NULL, NULL, NULL);
    ratio2_result = CreateWindow(
        "static", doubleToChar(ratio2),
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 260, 80, 20, hwnd, NULL, NULL, NULL);
    ratio3_result = CreateWindow(
        "static", doubleToChar(ratio3),
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 280, 80, 20, hwnd, NULL, NULL, NULL);
    ratio4_result = CreateWindow(
        "static", doubleToChar(ratio4),
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 300, 80, 20, hwnd, NULL, NULL, NULL);
    ratio5_result = CreateWindow(
        "static", doubleToChar(ratio5),
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        410, 320, 80, 20, hwnd, NULL, NULL, NULL);

    strcpy(
        ratio1_pass, (ratio1 >= 5.8 && ratio1 <= 14.5) ? "Pass" : "Not pass");
    strcpy(ratio2_pass, (ratio2 >= 1.9 && ratio2 <= 5.9) ? "Pass" : "Not pass");
    strcpy(ratio3_pass, (ratio3 >= 1.6 && ratio3 <= 3.6) ? "Pass" : "Not pass");
    strcpy(ratio4_pass, (ratio4 >= 0.1 && ratio4 <= 0.5) ? "Pass" : "Not pass");
    strcpy(ratio5_pass, (ratio5 >= 0.4 && ratio5 <= 0.7) ? "Pass" : "Not pass");
    ratio1_comment = CreateWindow(
        "static", ratio1_pass, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 240, 80, 20, hwnd, NULL, NULL, NULL);
    ratio2_comment = CreateWindow(
        "static", ratio2_pass, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 260, 80, 20, hwnd, NULL, NULL, NULL);
    ratio3_comment = CreateWindow(
        "static", ratio3_pass, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 280, 80, 20, hwnd, NULL, NULL, NULL);
    ratio4_comment = CreateWindow(
        "static", ratio4_pass, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 300, 80, 20, hwnd, NULL, NULL, NULL);
    ratio5_comment = CreateWindow(
        "static", ratio5_pass, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        490, 320, 80, 20, hwnd, NULL, NULL, NULL);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    return 0;
}