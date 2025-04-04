#include "util.h"
#include <iostream>

bool dll_injection(
    __in DWORD pid,
    __in const std::wstring& dll_name
) {
    bool result = false;
    HANDLE process_handle = nullptr;
    HANDLE thread_handle = nullptr;
    LPVOID remote_buffer = nullptr;
    HMODULE module = {};
    LPTHREAD_START_ROUTINE thread_start_routine = nullptr;
    do {
        process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        // get handle for process with given pid
        if (process_handle == nullptr) {
            break;
        }
        // allocate mem for given dll at VAS
        remote_buffer = VirtualAllocEx(
            process_handle,
            nullptr,
            dll_name.size(),
            MEM_COMMIT, // Allocates memory charges (from the overall size of memory and the paging files on disk) for the specified reserved memory pages.
            PAGE_READWRITE // protection
        );
        
        if (!remote_buffer) {
            break;
        } // when failed

        // dll injection (write dll path string)
        if (!WriteProcessMemory(
            process_handle,
            remote_buffer,
            dll_name.c_str(),
            dll_name.size() * sizeof(wchar_t),
            nullptr)
            ) {
            break;
        }

        // get kernel32.dll 's handle
        module = GetModuleHandle(L"kernel32.dll");

        // load and store pointer to LoadLibrary function in Kernel32.dll
        thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");
        

        // CreateRemoteThread() 함수는 스레드를 생성하는 함수인데, 
        // 저 함수를 호출하는 프로세스가 아닌 다른 프로세스에 스레드를 생성하는 기능을 가지고 있다
        // 여기서 재미있는 점은 DLL을 읽어서 실행가능한 영역에 적재하는 함수인
        // LoadLibrary() 함수가 스레드 프로시저와 비슷한 모양을 가지고 있다는 것이다. CreateRemoteThread()를 활용하는 DLL injection은, 
        // 바로 이 점을 이용해서 원하는 DLL을 적재하는 스레드를 생성시켜 대상 프로세스에 나의 코드를 주입하는 방법이다
        //
        // create a thread that runs in the virtual address space of another process and optionally specify extended attributes.
        // use LoadLibrary() function to load our dll to target process's runtime
        thread_handle = CreateRemoteThread(
            process_handle,
            nullptr,
            0,
            thread_start_routine,
            remote_buffer,
            0,
            nullptr
        );

        // time out 10 sec
        DWORD result = WaitForSingleObject(thread_handle, 10000);

        switch (result) {
        case WAIT_OBJECT_0:
            std::cout << "WAIT_OBJECT_0: The object is signaled." << std::endl;
            break;

        case WAIT_TIMEOUT:
            std::cout << "WAIT_TIMEOUT: The wait timed out before the object became signaled." << std::endl;
            break;

        case WAIT_ABANDONED:
            std::cout << "WAIT_ABANDONED: A mutex was not released before the owning thread terminated. Check for data consistency." << std::endl;
            break;

        case WAIT_FAILED:
            std::cerr << "WAIT_FAILED: The function failed. Error code: " << GetLastError() << std::endl;
            break;

        default:
            std::cerr << "Unknown return value: " << result << std::endl;
            break;
        }

        result = true;
    } while (false);

    CloseHandle(process_handle);
    CloseHandle(thread_handle);

    return result;
}