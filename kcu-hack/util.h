#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <string>

DWORD getProcessID(const std::wstring& processName);
DWORD check_pid(const std::wstring& process_name);
uint32_t get_base_address(DWORD pid, const std::wstring& moduleName);

#endif // UTIL_H