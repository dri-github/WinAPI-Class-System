#ifndef WINAPI_PROGRESSBAR_H
#define WINAPI_PROGRESSBAR_H

#include "../Window.h"

WINAPI_BEGIN
class WINAPI_CLASS_SYSTEM ProgressBar : public Window
{
public:
	enum States
	{
		Normal = PBST_NORMAL,
		Error = PBST_ERROR,
		Paused = PBST_PAUSED
	};

public:
	ProgressBar(CreateStruct createStruct) : Window(createStruct.setClassName(PROGRESS_CLASS).setStyle(WS_CHILD | WS_VISIBLE))
	{}

	//Задаёт предел значений ProgressBar
	void setRange(int min_value, int max_value)
	{
		SendMessageA(m_hWnd, PBM_SETRANGE32, min_value, max_value);
	}
	//Задаёт шаг
	void setStep(int step)
	{
		SendMessageA(m_hWnd, PBM_SETSTEP, step, NULL);
	}
	//Сдвинуть на шаг
	void step()
	{
		SendMessageA(m_hWnd, PBM_STEPIT, NULL, NULL);
	}
	//Задаёт статус
	void setState(States states)
	{
		SendMessageA(m_hWnd, PBM_SETSTATE, states, NULL);
	}
	//Задаёт значение прогресса
	void setProgress(int progress)
	{
		SendMessageA(m_hWnd, PBM_SETPOS, progress, NULL);
	}
	//Переместить прогресс на delta
	void moveDelta(int delta)
	{
		SendMessageA(m_hWnd, PBM_DELTAPOS, delta, NULL);
	}

	//Возвращает значение шага
	int getStep() const
	{
		return SendMessageA(m_hWnd, PBM_GETSTEP, NULL, NULL);
	}
	//Возвращает статус
	States getStatus() const
	{
		return (States)SendMessageA(m_hWnd, PBM_GETRANGE, NULL, NULL);
	}
	//Возврощает прогресс
	int getProgress() const
	{
		return SendMessageA(m_hWnd, PBM_GETPOS, NULL, NULL);
	}
};
WINAPI_END

#endif // !WINAPI_PROGRESSBAR_H