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

	//����� ������ �������� ProgressBar
	void setRange(int min_value, int max_value)
	{
		SendMessageA(m_hWnd, PBM_SETRANGE32, min_value, max_value);
	}
	//����� ���
	void setStep(int step)
	{
		SendMessageA(m_hWnd, PBM_SETSTEP, step, NULL);
	}
	//�������� �� ���
	void step()
	{
		SendMessageA(m_hWnd, PBM_STEPIT, NULL, NULL);
	}
	//����� ������
	void setState(States states)
	{
		SendMessageA(m_hWnd, PBM_SETSTATE, states, NULL);
	}
	//����� �������� ���������
	void setProgress(int progress)
	{
		SendMessageA(m_hWnd, PBM_SETPOS, progress, NULL);
	}
	//����������� �������� �� delta
	void moveDelta(int delta)
	{
		SendMessageA(m_hWnd, PBM_DELTAPOS, delta, NULL);
	}

	//���������� �������� ����
	int getStep() const
	{
		return SendMessageA(m_hWnd, PBM_GETSTEP, NULL, NULL);
	}
	//���������� ������
	States getStatus() const
	{
		return (States)SendMessageA(m_hWnd, PBM_GETRANGE, NULL, NULL);
	}
	//���������� ��������
	int getProgress() const
	{
		return SendMessageA(m_hWnd, PBM_GETPOS, NULL, NULL);
	}
};
WINAPI_END

#endif // !WINAPI_PROGRESSBAR_H