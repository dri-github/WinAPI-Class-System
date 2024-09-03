#include "WinAPI Class System/base.h"

#include <map>
#include <string>

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:  // ����������� DLL
        // ��������� ��� �����������
        // �������� �� �������������

        // ���� ������������� ������
        // ������� ���������� TRUE
        // � ��������� ������ � FALSE
        return 1;

    case DLL_PROCESS_DETACH: // ���������� DLL
        // ��������� ��� ��������
        // �� ���������������
        break;

    case DLL_THREAD_ATTACH: // �������� ������ ������
        // ��������� �� ������������� ������,
        // ���� ����������
        break;

    case DLL_THREAD_DETACH: // ���������� ������
        // ����������� ����������, ��������� � �������
        break;
    }
    return TRUE;  // ���������� ���-������ (��� �����
    // ��� �������� ������������)
}

extern "C" {
    WINAPI_CLASS_SYSTEM const char* getName() { return "testdll"; }
    WINAPI_CLASS_SYSTEM std::map<std::string, std::string>* getParametrs() {
        
        
        return new std::map<std::string, std::string>{ 
            { "Name", TYPE(std::string) },
            { "size", TYPE(int) }
        };
    }
}