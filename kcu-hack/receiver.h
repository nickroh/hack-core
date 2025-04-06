#ifndef RECEIVER_H  // Include guard to prevent multiple inclusions
#define RECEIVER_H

#include <Windows.h>
#include "SharedMemory.h"
#include "hack-core.h"

// HackCore class definition
class Receiver  {
public:
    // Default constructor
    Receiver();  // Default constructor

    // Destructor
    ~Receiver();  // Destructor

    // Public data members (variables)

    // Public methods
    void start();
    //void setModule(LPVOID mod);

private:
    SharedMemoryHandler* sh;
    HackCore* hack;
    HANDLE fullEvent;
    HANDLE emptyEvent;
    HMODULE dll;

    bool initialize();
    //void unload();
};

#endif // HACKCORE_H#pragma once
