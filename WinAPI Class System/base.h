#pragma once

#define WINAPI_BEGIN namespace winapi {
#define WINAPI_END }

#if 1
#define WINAPI_CLASS_SYSTEM __declspec(dllexport)
#else
#define WINAPI_CLASS_SYSTEM __declspec(dllimport)
#endif

#undef UNICODE
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

#include <typeinfo>
#define TYPE(x) typeid(x).name()