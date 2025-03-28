#include <iostream>
#include <Windows.h>
#include <conio.h> // For _getch()
#include "SharedMemory.h"
#include <tchar.h>
#include "receiver.h"
#include "hack-core.h"

using namespace std;

Receiver::Receiver() {
    bool result = initialize();
	if (!result) {
		cout << "initialization failed" << endl;
	}
}

Receiver::~Receiver() {
	if (sh) {
		delete sh;
	}
    if (hack) {
        delete hack;
    }
}

void Receiver::start() {
    bool end = false;
    cout << "Receiving Loop entered" << endl;
    while (true) {
        DWORD result = WaitForSingleObject(fullEvent, INFINITE); // Wait for 10 seconds

        if (result == WAIT_OBJECT_0) {
            // The event was signaled
            std::cout << "Event signaled!" << std::endl;
            // Continue processing as normal
        }

        SharedMemory* sm = sh->getMessage();

        int cmd = sm->cmd;
        int option = sm->option;

        cout << "SetMSG with cmd: " << cmd << ", option: " << option << endl;

        switch (cmd)
        {
            case 1:  // hp hack
                hack->setHP(option);
                break;
            case 2:  // ammo hack
                hack->setAmmo(option);
                break;
            case 3:
                hack->setArmor(option);
                break;
            case 4:
                hack->setRecoil(option);
                break;
            case 0:
                end = true;
                break;
            default:
                // Code to execute if cmd doesn't match any case
                break;
        }
        if (end) { break; }

        ResetEvent(fullEvent);
        SetEvent(emptyEvent);

    }
    //for (int i = 0; i < 3; ++i) {
    //    MessageBox(NULL, L"MSG arrived", L"Info", MB_OK);
    //    Sleep(3000); // Wait for 3 seconds
    //}
}

bool Receiver::initialize() {
    hack = new HackCore();
    sh = new SharedMemoryHandler(_T("NUTRI-IPC"), 0);
	fullEvent = sh->getFullEvent();
	emptyEvent = sh->getEmptyEvent();

    if (!hack) {
        cout << "Error: hack is null!" << endl;
        return false;
    }

    if (!sh) {
        cout << "Error: SharedMemoryHandler (sh) is null!" << endl;
        return false;
    }

    if (!fullEvent) {
        cout << "Error: fullEvent is null!" << endl;
        return false;
    }

    if (!emptyEvent) {
        cout << "Error: emptyEvent is null!" << endl;
        return false;
    }

    HANDLE hEventInit = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"COREINIT");
    SetEvent(hEventInit);

    return true;
}

