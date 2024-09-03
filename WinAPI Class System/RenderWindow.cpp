#include "RenderWindow.h"

winapi::RenderWindow::RenderWindow(LPCSTR class_name) : TargetWindow(CreateStruct().setClassName(registerClassName(class_name)).setStyle(WS_OVERLAPPEDWINDOW)) {
    SetProcessDPIAware();
}

bool winapi::RenderWindow::isOpen() const {
	return m_msg.message != WM_QUIT;
}

void winapi::RenderWindow::processEvents() {
    while (GetMessageA(&m_msg, NULL, 0, 0)) {
        TranslateMessage(&m_msg);
        DispatchMessageA(&m_msg);
    }
}

LPCSTR winapi::RenderWindow::registerClassName(LPCSTR class_name) {
    HMODULE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXA wcex;

    if (GetClassInfoExA(hInstance, class_name, &wcex))
        return class_name;

    wcex.cbSize = sizeof(WNDCLASSEXA);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = BaseWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, "0");
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = class_name;
    wcex.hIconSm = nullptr;

    RegisterClassExA(&wcex);
    return class_name;
}

LRESULT winapi::RenderWindow::BaseWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* window;
    if (message == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        window = static_cast<Window*>(lpcs->lpCreateParams);
        window->m_hWnd = hWnd;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    else {
        window = Window::getObjectPtr(hWnd);
    }

    if (window)
    {
        return window->WndProc(message, wParam, lParam);
    }
    return ::DefWindowProcA(hWnd, message, wParam, lParam);
}