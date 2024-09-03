#ifndef WINAPI_BUTTON_H
#define WINAPI_BUTTON_H

#include "../Window.h"

//	
// Применение пока не найдено:
// BCN_HOTITEMCHANGE - Уведомляет владельца элемента управления "кнопка" о том, что мышь входит или покидает клиентскую область элемента управления "кнопка". WM_NOTIFY
// BN_SETFOCUS - Отправляется, когда кнопка получает фокус клавиатуры. Кнопка должна иметь стиль BS_NOTIFY для отправки этого кода уведомления. WM_COMMAND
// BN_KILLFOCUS - Отправляется, когда кнопка теряет фокус клавиатуры. Кнопка должна иметь стиль BS_NOTIFY для отправки этого кода уведомления. WM_COMMAND
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

	//Нажать кнопку
	void click() { SendMessage(m_hWnd, BM_CLICK, 0, 0); }
	//Возвращает состояние флажка
	bool getCheck() { return SendMessage(m_hWnd, BM_GETCHECK, 0, 0); }

	//Возвращает нажата ли кнопка
	virtual bool isClick() const {
		if ((style & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)
			return IsDlgButtonChecked(getParent()->getHWnd(), getId()) == BST_CHECKED;
		else
			return m_command == BN_CLICKED;
	}
	//Кнопка была нажата двойным щелчком
	virtual bool isDoubleClick() const { return m_command == BN_DBLCLK; }
};
WINAPI_END

#endif // !WINAPI_BUTTON_H