#ifndef MEMORY_H
#define MEMORY_H

#include <Windows.h>
#include <vector>

class Memory {
public:
    Memory();   // Constructor
    ~Memory();  // Destructor
    class Trampoline {
    public:
        Trampoline();   // Constructor
        ~Trampoline();  // Destructor 

        bool HookFunction(BYTE* src, BYTE* dst, const size_t len);
        BYTE* CreateTrampoline(BYTE* src, BYTE* dst, const size_t len);

    private:

    };

private:

};

#endif // MEMORY_H
