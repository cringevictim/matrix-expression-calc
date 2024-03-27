#include <windows.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

#include "Parser.h"
//#include "Matrix.h"
//#include "Vector.h"

// Button id's
#define BUTTON_VECTOR_OPERATIONS 1
#define BUTTON_MATRIX_OPERATIONS 2
#define BUTTON_HELP 3
#define BUTTON_CALCULATE_MATRIX 4
#define BUTTON_CALCULATE_VECTOR 5

// Textbox id's
#define TEXTBOX_MATRIX 11
#define TEXTBOX_VECTOR 12



// Window procedure function
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Main function
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    
    WNDCLASSW wc = { 0 };

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"myWindowClass", L"Vectors and Matrices", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, NULL, NULL, NULL, NULL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

std::wstring matrixToString(const std::vector<std::vector<int>>& matrix) {
    std::wstringstream ss;
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            ss << element << L" ";
        }
        ss << L"\n"; // Новий рядок для кожного ряду матриці
    }
    return ss.str();
}


// Window procedure function implementation
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        // Створення кнопок
        CreateWindowW(L"Button", L"Векторні операції", WS_VISIBLE | WS_CHILD, 50, 50, 200, 40, hWnd, (HMENU)BUTTON_VECTOR_OPERATIONS, NULL, NULL);
        CreateWindowW(L"Button", L"Матричні операції", WS_VISIBLE | WS_CHILD, 50, 100, 200, 40, hWnd, (HMENU)BUTTON_MATRIX_OPERATIONS, NULL, NULL);
        CreateWindowW(L"Button", L"Довідка", WS_VISIBLE | WS_CHILD, 50, 150, 200, 40, hWnd, (HMENU)BUTTON_HELP, NULL, NULL);
        //CreateMatrixForm(hWnd, 3,110, 100, 280);
        CreateWindowW(L"Edit", std::to_wstring(0).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 280, 200, 30, hWnd, (HMENU)TEXTBOX_MATRIX, NULL, NULL);
        CreateWindowW(L"Button", L"Обрахувати     ", WS_VISIBLE | WS_CHILD, 100, 250, 150, 30, hWnd, (HMENU)BUTTON_CALCULATE_MATRIX, NULL, NULL);
        
        //SendMessage(hStatic, WM_SETFONT, hFontText, 0);
        break;
    case WM_COMMAND:
        switch (wp) {
        case BUTTON_VECTOR_OPERATIONS:
            MessageBoxW(hWnd, L"Інтерфейс для векторних операцій", L"Векторні операції", MB_OK);
            break;
        case BUTTON_MATRIX_OPERATIONS:
            MessageBoxW(hWnd, L"Інтерфейс для матричних операцій", L"Матричні операції", MB_OK);
            break;
        case BUTTON_HELP:
            MessageBoxW(hWnd, L"Довідкова інформація", L"Довідка", MB_OK);
            break;
        case BUTTON_CALCULATE_MATRIX: {
            // Get the length of the text in TEXTBOX_MATRIX to allocate buffer.
            int len = GetWindowTextLength(GetDlgItem(hWnd, TEXTBOX_MATRIX)) + 1;
            // Allocate buffer to hold text.
            wchar_t* text = new wchar_t[len];
            // Retrieve text from TEXTBOX_MATRIX.
            GetWindowText(GetDlgItem(hWnd, TEXTBOX_MATRIX), text, len);
            // Convert from wide char to narrow char array
            char ch[256];
            char DefChar = ' ';
            WideCharToMultiByte(CP_ACP, 0, text, -1, ch, 260, &DefChar, NULL);
            // Convert char array to string
            std::string matrixStr(ch);
            // Free text buffer.
            delete[] text;
            try {
                //[[1,2,3],[4,5,6],[7,8,9]]
                //([[1,2,3],[4,5,6],[7,8,9]]+[[1,2,3],[4,5,6],[7,8,9]])*[[1,2,3],[4,5,6],[7,8,9]]

                //
                Matrix result = evaluateMatrixExpression(matrixStr);
                std::string result_str = matrixToString(result);
                std::wstring matrixString = std::wstring(result_str.begin(), result_str.end()); // Конвертуємо матрицю до рядка
                MessageBoxW(hWnd, matrixString.c_str(), L"Матриця", MB_OK); // Виводимо матрицю у вікні повідомлення
            }
            catch (const std::exception& e) {
                MessageBoxW(hWnd, L"Failed", L"Error", MB_OK);
            }
        }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }

    return 0;
}