#include "Memory.h"
#include <iostream>
#include "util.h"

Memory::Memory() {
    std::cout << "Memory instance created." << std::endl;
}

Memory::~Memory() {
    std::cout << "Memory instance destroyed." << std::endl;
}

// Memory::Trampoline 생성자
Memory::Trampoline::Trampoline() {
    std::cout << "Trampoline instance created." << std::endl;
}

Memory::Trampoline::~Trampoline() {
    std::cout << "Trampoline instance destroyed." << std::endl;
}

bool Memory::Trampoline::HookFunction(BYTE* src, BYTE* dst, const size_t len) {
    if (len < 5) return false;

    DWORD curProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

    memset(src, 0x90, len); // NOP로 덮기
    uintptr_t relAddr = (uintptr_t)(dst - src - 5);

    src[0] = 0xE9; // JMP opcode
    *(uintptr_t*)(src + 1) = relAddr;

    DWORD temp;
    VirtualProtect(src, len, curProtection, &temp);
    return true;
}

BYTE* Memory::Trampoline::CreateTrampoline(BYTE* src, BYTE* dst, const size_t len) {
    if (len < 5) {
        Log("CreateTrampoline failed: len < 5");
        return nullptr;
    }

    Log("Trampoline setup entered");

    // Allocate memory for trampoline
    BYTE* gateway = (BYTE*)VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!gateway) {
        Log("CreateTrampoline failed: VirtualAlloc returned nullptr");
        return nullptr;
    }

    Log("Allocated gateway memory");

    // Copy original instructions to trampoline
    memcpy(gateway, src, len);
    Log("Copied original bytes to gateway");

    // Compute relative jump from end of trampoline back to original function
    uintptr_t jumpBackAddr = (uintptr_t)((src + len) - (gateway + len + 5));

    gateway[len] = 0xE9; // JMP opcode
    *(uintptr_t*)(gateway + len + 1) = jumpBackAddr;

    Log("Wrote jump back from trampoline to original function");

    // Hook the original function
    if (!HookFunction(src, dst, len)) {
        Log("CreateTrampoline failed: HookFunction returned false");
        return nullptr;
    }

    Log("HookFunction succeeded, trampoline ready");
    return gateway;
}

bool Memory::Trampoline::UnhookFunction(BYTE* src, BYTE* originalBytes, size_t len) {
    DWORD oldProtect;
    if (!VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect))
        return false;

    memcpy(src, originalBytes, len);  // 원래 코드로 복원

    VirtualProtect(src, len, oldProtect, &oldProtect);
    FlushInstructionCache(GetCurrentProcess(), src, len); // 캐시 정리

    return true;
}