#include "WinAPI Class System/base.h"

#include <map>
#include <string>

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:  // Подключение DLL
        // Выполняем все необходимые
        // действия по инициализации

        // если инициализация прошла
        // успешно возвращаем TRUE
        // в противном случае – FALSE
        return 1;

    case DLL_PROCESS_DETACH: // Отключение DLL
        // Выполняем все действия
        // по деинициализации
        break;

    case DLL_THREAD_ATTACH: // Создание нового потока
        // Переходим на многопоточную версию,
        // если необходимо
        break;

    case DLL_THREAD_DETACH: // Завершение потока
        // Освобождаем переменные, связанные с потоком
        break;
    }
    return TRUE;  // Возвращаем что-нибудь (все равно
    // код возврата игнорируется)
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