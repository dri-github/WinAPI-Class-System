#ifndef WINAPI_LISTBOX_H
#define WINAPI_LISTBOX_H

#include "../Window.h"

WINAPI_BEGIN
class WINAPI_CLASS_SYSTEM ListBox : public Window
{
public:
	ListBox(CreateStruct createStruct) : Window(createStruct.setClassName(WC_LISTBOXA).setStyle(WS_VISIBLE | WS_CHILD | WS_VSCROLL)) {}
	~ListBox() {}

	void add(const char* name, void* data = nullptr) { SendMessage(m_hWnd, LB_SETITEMDATA, SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)name), (LPARAM)data); }
	void deleteString(int index) { SendMessage(m_hWnd, LB_DELETESTRING, index, NULL); }

	int getCurrentIndex() const { return SendMessage(m_hWnd, LB_GETCURSEL, NULL, NULL); }
	char* getCurrentString() const {
		int index = getCurrentIndex();
		if (index == LB_ERR)
			return const_cast<char*>("");
		LPTSTR text = new char[SendMessage(m_hWnd, LB_GETTEXTLEN, index, NULL)];
		SendMessage(m_hWnd, LB_GETTEXT, index, (LPARAM)text);
		return text;
	}
	void* getCurrentData() const { return (void*)SendMessage(m_hWnd, LB_GETITEMDATA, getCurrentIndex(), NULL); }
};
WINAPI_END

#endif // !WINAPI_LISTBOX_H