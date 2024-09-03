#ifndef WINAPI_EDIT_H
#define WINAPI_EDIT_H

#include "../Window.h"

#ifndef WC_EDIT
#define WC_EDITA "Edit"
#define WC_EDITW L"Edit"
#ifdef UNICODE
#define WC_EDIT WC_EDITW
#else
#define WCEDIT WC_EDITA
#endif
#endif

WINAPI_BEGIN
template<DWORD style = ES_LEFT>
class WINAPI_CLASS_SYSTEM Edit : public Window
{
public:
    Edit(CreateStruct createStruct) : Window(createStruct.setClassName(WC_EDIT).setStyle(style | WS_VISIBLE | WS_CHILD)) {}
    ~Edit() {}

    void setLimitText(int count) { SendMessage(m_hWnd, EM_SETLIMITTEXT, count, NULL); }

    //Введенный текст
    LPSTR getData() const
    {
        int lenght = GetWindowTextLength(m_hWnd) + 1;
        LPSTR text = new char[lenght];
        GetWindowText(m_hWnd, text, lenght);
        return text;
    }

    //Возвращает произошло ли изменение текста
    virtual bool isChange() const { return m_command == EN_CHANGE; }
};

template<DWORD style>
class IntEdit : public Edit<style>
{
public:
    IntEdit(CreateStruct createStruct) : Edit<style>(createStruct)
    {
        SetWindowLongPtr(Window::m_hWnd, GWLP_USERDATA, SetWindowLongPtr(Window::m_hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc));
    }

    int getData() const { return atoi(Edit<style>::getData()); }

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_CHAR)
        {
            if (!((wParam >= '0' && wParam <= '9')
                || wParam == '-'
                || wParam == VK_RETURN
                || wParam == VK_BACK))
            {
                return 0;
            }
        }
        return CallWindowProc((WNDPROC)GetWindowLongPtr(hWnd, GWLP_USERDATA), hWnd, message, wParam, lParam);
    }
};
WINAPI_END

#endif // !WINAPI_EDIT_H