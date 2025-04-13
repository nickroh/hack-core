#include "Memory.h"
#include <iostream>

Memory::Memory() {
    std::cout << "Memory instance created." << std::endl;
}

Memory::~Memory() {
    std::cout << "Memory instance destroyed." << std::endl;
}

// Memory::Trampoline »ý¼ºÀÚ
Memory::Trampoline::Trampoline() {
    std::cout << "Trampoline instance created." << std::endl;
}

Memory::Trampoline::~Trampoline() {
    std::cout << "Trampoline instance destroyed." << std::endl;
}

bool Memory::Trampoline::HookFunction(BYTE* src, BYTE* dst, const size_t len) {

}

BYTE* Memory::Trampoline::CreateTrampoline(BYTE* src, BYTE* dst, const size_t len) {

}