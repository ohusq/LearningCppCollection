#pragma once
#include <iostream>
#include <windows.h>
#include <filesystem>

bool initializeKernel32(HMODULE& kernel32Module, LPVOID& loadLibraryAddress) {
    kernel32Module = GetModuleHandleA("kernel32.dll");
    if (kernel32Module == NULL) {
        std::cerr << "Failed to get the handle of kernel32.dll!" << std::endl;
        return false;
    }

    loadLibraryAddress = GetProcAddress(kernel32Module, "LoadLibraryA");
    if (loadLibraryAddress == NULL) {
        std::cerr << "Failed to get the address of LoadLibraryA!" << std::endl;
        return false;
    }

    return true;
}

int inject_dll(const std::string& dllPath, DWORD processID, LPVOID loadLibraryAddress) {
    // Convert DLL path to C-style string
    const char* dllPathStr = dllPath.c_str();

    std::cout << "Injecting " << dllPath << " into process with PID " << processID << "..." << std::endl;

    // Check if the DLL file exists in the specified path
    if (!std::filesystem::exists(dllPath)) {
        std::cerr << "DLL not found!" << std::endl;
        return 1;
    }

    // Open the target process by PID
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    if (hProcess == NULL) {
        std::cerr << "Failed to open the target process!" << std::endl;
        return 1;
    }

    // Allocate memory for the DLL path in the target process
    size_t dllPathLength = strlen(dllPathStr) + 1;
    LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, dllPathLength, MEM_COMMIT, PAGE_READWRITE);
    if (dllPathAddress == NULL) {
        std::cerr << "Failed to allocate memory in the target process!" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, dllPathAddress, dllPathStr, dllPathLength, NULL)) {
        std::cerr << "Failed to write DLL path to the target process!" << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddress, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create a remote thread in the target process!" << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup and close handles
    VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << "DLL injected successfully!" << std::endl;
    return 0;
}
