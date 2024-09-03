#ifndef WINAPI_RENDERWINDOW_H
#define WINAPI_RENDERWINDOW_H

#include "TargetWindow.h"

WINAPI_BEGIN
class WINAPI_CLASS_SYSTEM RenderWindow : public TargetWindow {
public:
	RenderWindow(LPCSTR class_name);

	bool isOpen() const;
	void processEvents();

protected:
	BEGIN_MSG_MAP(RenderWindow, TargetWindow)
	END_MSG_MAP()

	LPCSTR registerClassName(LPCSTR class_name);
	static LRESULT CALLBACK BaseWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	MSG m_msg;
};
WINAPI_END

#endif // !WINAPI_RENDERWINDOW_H