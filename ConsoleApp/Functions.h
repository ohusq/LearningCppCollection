#pragma once // Ensures that this header file is only included once

#include <iostream> // For std::cout, std::cin, std::endl (minimum version C++11)
#include <thread> // For std::this_thread::sleep_for (minimum version C++11)
#include <chrono> // For std::this_thread::sleep_for (minimum version C++11)
#include <filesystem> // For std::filesystem::exists (minimum version C++17)
#include <windows.h> // For HANDLE (minimum version Windows XP)

template <typename Duration>
void sleep(const Duration& duration) {
    std::this_thread::sleep_for(duration);
}

int inject_dll(const char* dllPath, const char* program) {
    std::cout << "Injecting " << dllPath << " into " << program << "..." << std::endl;

    // Check if the DLL file exists in the specified path
    if (!std::filesystem::exists(dllPath)) {
        std::cerr << "DLL not found!" << std::endl;
        return 1;
    }

    // Check if the program is running
    HANDLE hMutex = OpenMutexA(MUTEX_MODIFY_STATE, false, program);
    if (hMutex == NULL) {
        std::cerr << "Program is not running or access denied!" << std::endl;
        return 1;
    }

    // Get process ID and open process
    DWORD processID;
    GetWindowThreadProcessId(FindWindowA(NULL, program), &processID);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    if (hProcess == NULL) {
        std::cerr << "Failed to open the target process!" << std::endl;
        CloseHandle(hMutex);
        return 1;
    }

    // Allocate memory for the DLL path in the target process
    size_t dllPathLength = strlen(dllPath) + 1;
    LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, dllPathLength, MEM_COMMIT, PAGE_READWRITE);
    if (dllPathAddress == NULL) {
        std::cerr << "Failed to allocate memory in the target process!" << std::endl;
        CloseHandle(hProcess);
        CloseHandle(hMutex);
        return 1;
    }

    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, dllPathAddress, dllPath, dllPathLength, NULL)) {
        std::cerr << "Failed to write DLL path to the target process!" << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hMutex);
        return 1;
    }

    // Get the address of LoadLibraryA in the target process
    LPVOID loadLibraryAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (loadLibraryAddress == NULL) {
        std::cerr << "Failed to get the address of LoadLibraryA!" << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hMutex);
        return 1;
    }

    // Create a remote thread to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddress, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create a remote thread in the target process!" << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hMutex);
        return 1;
    }

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup and close handles
    VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    CloseHandle(hMutex);

    std::cout << "DLL injected successfully!" << std::endl;
    return 0; // returns 0
}

