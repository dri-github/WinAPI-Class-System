#ifndef WINAPI_BUTTON_H
#define WINAPI_BUTTON_H

#include "../Window.h"

//	
// ���������� ���� �� �������:
// BCN_HOTITEMCHANGE - ���������� ��������� �������� ���������� "������" � ���, ��� ���� ������ ��� �������� ���������� ������� �������� ���������� "������". WM_NOTIFY
// BN_SETFOCUS - ������������, ����� ������ �������� ����� ����������. ������ ������ ����� ����� BS_NOTIFY ��� �������� ����� ���� �����������. WM_COMMAND
// BN_KILLFOCUS - ������������, ����� ������ ������ ����� ����������. ������ ������ ����� ����� BS_NOTIFY ��� �������� ����� ���� �����������. WM_COMMAND
//	

#ifndef WC_BUTTON
#define WC_BUTTONA "Button"
#define WC_BUTTONW L"Button"
#ifdef UNICODE
#define WC_BUTTON WC_BUTTONW
#else
#define WC_BUTTON WC_BUTTONA
#endif
#endif

WINAPI_BEGIN
////////////////////////////////////////////////////////////
/// \brief Base class for buttons
///
////////////////////////////////////////////////////////////
template <DWORD style = BS_DEFPUSHBUTTON>
class WINAPI_CLASS_SYSTEM Button : public Window {
public:
	Button(CreateStruct createStruct) : Window(createStruct.setClassName(WC_BUTTON).setStyle(style | BS_NOTIFY | WS_VISIBLE | WS_CHILD)) {}
	~Button() {}

	//������ ������
	void click() { SendMessage(m_hWnd, BM_CLICK, 0, 0); }
	//���������� ��������� ������
	bool getCheck() { return SendMessage(m_hWnd, BM_GETCHECK, 0, 0); }

	//���������� ������ �� ������
	virtual bool isClick() const {
		if ((style & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)
			return IsDlgButtonChecked(getParent()->getHWnd(), getId()) == BST_CHECKED;
		else
			return m_command == BN_CLICKED;
	}
	//������ ���� ������ ������� �������
	virtual bool isDoubleClick() const { return m_command == BN_DBLCLK; }
};
WINAPI_END

#endif // !WINAPI_BUTTON_H