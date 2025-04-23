#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <string>
#include "geometry.h"

DWORD getProcessID(const std::wstring& processName);
DWORD check_pid(const std::wstring& process_name);
uintptr_t get_base_address(DWORD pid, const std::wstring& moduleName);
void Log(const std::string& message);
bool isTeamGame(int mode);
bool WorldToScreen(vec pos, vec& screen, float matrix[16], int windowWidth, int windowHeight);

#endif // UTIL_H