#ifndef WINAPI_STATIC_H
#define WINAPI_STATIC_H

#include "../Window.h"

WINAPI_BEGIN
template<DWORD style = NULL>
class WINAPI_CLASS_SYSTEM Static : public Window
{
public:
	Static(CreateStruct createStruct) : Window(createStruct.setClassName(WC_STATIC).setStyle(style | SS_NOTIFY | WS_VISIBLE | WS_CHILD)) {}
	~Static() {}

	//Одиночное нажатие
	bool isClick() const { return m_command == STN_CLICKED; }
	//Двойное нажатие
	bool isDoubleClick() const { return m_command == STN_DBLCLK; }
};
WINAPI_END

#endif // !WINAPI_STATIC_H