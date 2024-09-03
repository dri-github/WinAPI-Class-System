#include "Window.h"

winapi::Window::Window(HWND hWnd) : m_parentWindow(nullptr), m_minSize({ 0, 0 }), m_maxSize({ 1920, 1080 }) {
	m_hWnd = hWnd;
	if (!m_hWnd) {
		delete this;
		return;
	}

	HWND parent = GetParent(m_hWnd);
	if (parent != NULL)
		m_parentWindow = new Window(parent);

	MINMAXINFO mmi = { 0 };
	SendMessage(hWnd, WM_GETMINMAXINFO, NULL, (LPARAM)&mmi);
	m_maxSize = mmi.ptMaxSize;
	m_minSize = mmi.ptMinTrackSize;
}

winapi::Window::~Window() {
	destroy();
}

winapi::Window::Window(CreateStruct createStruct) : m_minSize({ 0, 0 }), m_maxSize({ 1920, 1080 }) {
	m_parentWindow = createStruct.parent;
	m_hWnd = CreateWindowEx(WS_EX_LEFT, createStruct.className, "", createStruct.style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_parentWindow != NULL ? m_parentWindow->getHWnd() : NULL, NULL, GetModuleHandle(NULL), this);

	if (!m_hWnd) {
		delete this;
		return;
	}

	setParent(m_parentWindow);

	SetWindowLongPtr(m_hWnd, GWLP_ID, createStruct.id);
}

void winapi::Window::setTitle(LPCSTR title) {
	SetWindowText(m_hWnd, title);
}

void winapi::Window::setParent(Window* parent) {
	m_parentWindow = parent;
	SetParent(m_hWnd, m_parentWindow != NULL ? m_parentWindow->getHWnd() : NULL);
}

void winapi::Window::setPosition(POINT position) {
	setPosition(position.x, position.y);
}

void winapi::Window::setPosition(int x, int y) {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, NULL, x, y, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE);
}

void winapi::Window::setSize(SIZE size) {
	setSize(size.cx, size.cy);
}

void winapi::Window::setSize(int width, int height) {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, NULL, rect.left, rect.right, width, height, SWP_NOMOVE);
}

void winapi::Window::setMinSize(int width, int height) {
	m_minSize = { width, height };
}

void winapi::Window::setMaxSize(int width, int height) {
	m_maxSize = { width, height };
}

LPSTR winapi::Window::getTitle() const
{
	int length = GetWindowTextLength(m_hWnd) + 1;
	LPSTR title = new char[length];
	GetWindowText(m_hWnd, title, length);
	return title;
}

winapi::Window* winapi::Window::getParent() const {
	return m_parentWindow;
}

POINT winapi::Window::getPosition() const {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return { rect.left, rect.top };
}

SIZE winapi::Window::getSize() const {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

SIZE winapi::Window::getClientSize() const
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

int winapi::Window::getId() const {
	return GetWindowLongPtr(m_hWnd, GWLP_ID);
}

HWND winapi::Window::getHWnd() const {
	return m_hWnd;
}

winapi::Window* winapi::Window::getObjectPtr(HWND hwnd) {
	return (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

void winapi::Window::show() {
	ShowWindow(m_hWnd, SW_SHOW);
}

void winapi::Window::hide() {
	ShowWindow(m_hWnd, SW_HIDE);
}

void winapi::Window::destroy() {
	DestroyWindow(m_hWnd);
}

LRESULT winapi::Window::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_GETMINMAXINFO: {
		MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
		POINT min_size = { m_minSize.x, m_minSize.y };
		POINT max_size = { m_maxSize.x, m_maxSize.y };

		pInfo->ptMinTrackSize = min_size;
		pInfo->ptMaxTrackSize = max_size;
		return NULL;
	}
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}

	return 0;
}