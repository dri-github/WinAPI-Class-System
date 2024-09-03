#ifndef WINAPI_TARGETWINDOW_H
#define WINAPI_TARGETWINDOW_H

#include "Window.h"
#include <vector>

WINAPI_BEGIN
class WINAPI_CLASS_SYSTEM TargetWindow : public Window {
public:
	TargetWindow(CreateStruct createStruct);

	template<typename T, class = std::enable_if_t<std::is_base_of<Window, T>::value>>
	T* getWindow() {
		CreateStruct createStruct;
		char className[MAX_CLASS_NAME];
		GetClassName(m_hWnd, className, MAX_CLASS_NAME);
		createStruct.className = className;
		createStruct.parent = this;
		createStruct.id = m_childrens.size();
		//createStruct.style = WS_OVERLAPPEDWINDOW;

		Window* window = new T(createStruct);
		m_childrens.push_back(window);
		return static_cast<T*>(window);
	}
	
protected:
	BEGIN_MSG_MAP(TargetWindow, Window)
	case WM_COMMAND: {
		if (LOWORD(wParam) > -1 && LOWORD(wParam) < m_childrens.size()) {
			m_childrens[LOWORD(wParam)]->messageCommand(wParam, lParam);
		}
		OnCommand(wParam, lParam);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		OnPaint(BeginPaint(m_hWnd, &ps), lParam);
		EndPaint(m_hWnd, &ps);
		break;
	}
	case WM_SIZE: {
		OnResize(static_cast<ResizeState>(wParam), { LOWORD(lParam), HIWORD(lParam) });
		break;
	}
	END_MSG_MAP()

	virtual void OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hdc, LPARAM lParam);

	enum ResizeState : short { Restored = SIZE_RESTORED, Minimized = SIZE_MINIMIZED, Maximized = SIZE_MAXIMIZED, MaxShow = SIZE_MAXSHOW, MaxHide = SIZE_MAXHIDE };
	virtual void OnResize(ResizeState resizeState, SIZE newSize);

private:
	std::vector<Window*> m_childrens;
};
WINAPI_END

#endif // !WINAPI_TARGETWINDOW_H