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

int loadMenu(int totalOptions) {
    std::cout << "Welcome to ohusq's learning project" << std::endl << "Please select an option from below." << std::endl;

    std::cout << "1. Option 1 (DLL Injector)" << std::endl;
    std::cout << "2. Option 2 (Restart explorer)" << std::endl;

    int x{};
    std::cout << "Enter a number: ";
    std::cin >> x;

    if (x < 1 || x > totalOptions) {
        std::cout << "Please input a valid number!" << std::endl << "Quitting...";
        exit(0); // Quit program
    }
    else {
        return x; // Return user input (used for switch statement in function main:29)
    }
}

bool restartExplorer() {
	std::cout << "Restarting explorer.exe..." << std::endl;
	system("taskkill /f /im explorer.exe");
	system("start explorer.exe");
    if (system("explorer.exe")) {
		std::cout << "Explorer restarted successfully!" << std::endl;
        return true;
	}
    else {
		std::cout << "Failed to restart explorer.exe!" << std::endl;
        return false;
	}
}

int main()
{
    int totalOptions = 2;
    int choice = loadMenu(totalOptions); // Load menu and get user input

    std::string dllPath;

    HMODULE kernel32Module = NULL; // Handle to kernel32.dll
    LPVOID loadLibraryAddress = NULL; // Address of LoadLibraryA
    bool initializationSuccessful = false; // Flag to check if initialization was successful

    switch (choice) {
    case 1:
        std::cout << "You chose option 1 (DLL Injector)" << std::endl;

        DWORD processID; // Variable to store the target process ID
        std::cout << "Enter the PID of the target process: ";
        std::cin >> processID;

        std::cout << "Enter the DLL path (Full DLL path): ";
        std::cin.ignore(); // Ignore newline from previous input
        std::getline(std::cin, dllPath);

        initializationSuccessful = initializeKernel32(kernel32Module, loadLibraryAddress);
        if (!initializationSuccessful) {
            std::cout << "Initialization failed!";
            exit(1); // Exit with an error code
        }

        inject_dll(dllPath, processID, loadLibraryAddress); // Pass processID instead of program name
        break;
    case 2:
        std::cout << "You chose option 2" << std::endl;
        bool explorerInt;
        explorerInt = restartExplorer(); // true = success, false = fail

        if (explorerInt) {
			std::cout << "Succes.." << std::endl;
		}
        else {
			std::cout << "It seems explorer.exe failed to start, you can restart it with:\nWIN + R and type 'explorer.exe' and hit run!" << std::endl;
		}
        break;
    default:
        std::cout << "You chose an invalid option" << std::endl;
        exit(0);
        break;
    }

    return 0;
}