#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <string>

DWORD getProcessID(const std::wstring& processName);
DWORD check_pid();
uintptr_t get_base_address(DWORD pid, const std::wstring& moduleName);

#endif // UTIL_H