#include <windows.h>

#define ID_READ_ALARM_A 1
#define ID_READ_ALARM_B 2
#define ID_SET_ALARM_A 3
#define ID_SET_ALARM_B 4
#define ID_DAY_OF_WEEK 5

// Global variables for alarm settings
int alarmA_Hours, alarmA_Minutes, alarmA_Seconds, alarmA_Day;
int alarmB_Hours, alarmB_Minutes, alarmB_Seconds, alarmB_Day;
BOOL isDayOfWeek;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        CreateWindow("STATIC", "Alarm A", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("STATIC", "Hours:", WS_VISIBLE | WS_CHILD, 20, 50, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 50, 30, 20, hwnd, (HMENU)ID_SET_ALARM_A, NULL, NULL);

        CreateWindow("STATIC", "Minutes:", WS_VISIBLE | WS_CHILD, 120, 50, 60, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 50, 30, 20, hwnd, (HMENU)ID_SET_ALARM_A, NULL, NULL);

        CreateWindow("STATIC", "Seconds:", WS_VISIBLE | WS_CHILD, 220, 50, 60, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 280, 50, 30, 20, hwnd, (HMENU)ID_SET_ALARM_A, NULL, NULL);

        CreateWindow("STATIC", "Day of Month:", WS_VISIBLE | WS_CHILD, 320, 50, 90, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER, 420, 50, 30, 20, hwnd, (HMENU)ID_SET_ALARM_A, NULL, NULL);

        CreateWindow("BUTTON", "Read Alarm A", WS_VISIBLE | WS_CHILD, 20, 80, 100, 30, hwnd, (HMENU)ID_READ_ALARM_A, NULL, NULL);
        CreateWindow("BUTTON", "Set Alarm A", WS_VISIBLE | WS_CHILD, 140, 80, 100, 30, hwnd, (HMENU)ID_SET_ALARM_A, NULL, NULL);

        // Alarm B
        CreateWindow("STATIC", "Alarm B", WS_VISIBLE | WS_CHILD, 20, 120, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("STATIC", "Hours:", WS_VISIBLE | WS_CHILD, 20, 150, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 150, 30, 20, hwnd, (HMENU)ID_SET_ALARM_B, NULL, NULL);

        CreateWindow("STATIC", "Minutes:", WS_VISIBLE | WS_CHILD, 120, 150, 60, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 180, 150, 30, 20, hwnd, (HMENU)ID_SET_ALARM_B, NULL, NULL);

        CreateWindow("STATIC", "Seconds:", WS_VISIBLE | WS_CHILD, 220, 150, 60, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "0", WS_VISIBLE | WS_CHILD | WS_BORDER, 280, 150, 30, 20, hwnd, (HMENU)ID_SET_ALARM_B, NULL, NULL);

        CreateWindow("STATIC", "Day of Month:", WS_VISIBLE | WS_CHILD, 320, 150, 90, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER, 420, 150, 30, 20, hwnd, (HMENU)ID_SET_ALARM_B, NULL, NULL);

        CreateWindow("BUTTON", "Read Alarm B", WS_VISIBLE | WS_CHILD, 20, 180, 100, 30, hwnd, (HMENU)ID_READ_ALARM_B, NULL, NULL);
        CreateWindow("BUTTON", "Set Alarm B", WS_VISIBLE | WS_CHILD, 140, 180, 100, 30, hwnd, (HMENU)ID_SET_ALARM_B, NULL, NULL);

        CreateWindow("BUTTON", "Is Day of the Week", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 20, 220, 150, 30, hwnd, (HMENU)ID_DAY_OF_WEEK, NULL, NULL);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_READ_ALARM_A) {
            // Handle read alarm A
            MessageBox(hwnd, "Reading Alarm A...", "Info", MB_OK);
        }
        else if (LOWORD(wParam) == ID_READ_ALARM_B) {
            // Handle read alarm B
            MessageBox(hwnd, "Reading Alarm B...", "Info", MB_OK);
        }
        else if (LOWORD(wParam) == ID_SET_ALARM_A) {
            // Handle setting Alarm A
            MessageBox(hwnd, "Setting Alarm A...", "Info", MB_OK);
        }
        else if (LOWORD(wParam) == ID_SET_ALARM_B) {
            // Handle setting Alarm B
            MessageBox(hwnd, "Setting Alarm B...", "Info", MB_OK);
        }
        else if (LOWORD(wParam) == ID_DAY_OF_WEEK) {
            // Handle day of the week checkbox
            isDayOfWeek = !isDayOfWeek;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char CLASS_NAME[] = "AlarmClockClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Alarm Clock Interface", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 300, NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nShowCmd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
