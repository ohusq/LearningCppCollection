#include <iostream>
#include <windows.h>
#include <filesystem>

#include "passwordGen.h" // Password generator header (generatePassword function)
#include "restartExplorerHeader.h" // Restart explorer header (restartExplorer function
#include "DLL_Functions.h" // DLL injector header (initializeKernel32, inject_dll functions)
#include "taskkill.h" // Taskkill header (taskkill_force function)
#include "autoclicker.h" // Autoclicker header (autoclicker function)

// Pre-declare functions
std::string generatePassword(int length);
bool restartExplorer(); // Restart explorer.exe
bool initializeKernel32(HMODULE& kernel32Module, LPVOID& loadLibraryAddress); // Initialize kernel32.dll and LoadLibraryA
int inject_dll(const std::string& dllPath, DWORD processID, LPVOID loadLibraryAddress); // Inject DLL into target process
int autoclicker(bool enabled, int cps); // Autoclicker (enabled = true/false, cps = clicks per second


int loadMenu(int totalOptions) {
    std::cout << "Welcome to ohusq's learning project" << std::endl << "Please select an option from below." << std::endl;

    std::cout << "1. DLL Injector" << std::endl;
    std::cout << "2. Restart explorer" << std::endl;
    std::cout << "3. Password generator" << std::endl;
    std::cout << "4. Taskkill (PROC NAME)" << std::endl;
    std::cout << "5. Taskkill (PID)" << std::endl;
    std::cout << "6. Autoclicker" << std::endl;

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

int main()
{
    int totalOptions = 6; // Total options in menu
    int choice = loadMenu(totalOptions); // Load menu and get user input

    std::string dllPath;

    HMODULE kernel32Module = NULL; // Handle to kernel32.dll
    LPVOID loadLibraryAddress = NULL; // Address of LoadLibraryA
    bool initializationSuccessful = false; // Flag to check if initialization was successful
    std::string procName; // Process name (string)
    std::string pid;

    switch (choice) {
    case 1: // DLL Injector
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
    case 2: // Restart explorer
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
    case 3: // Password generator
        std::cout << "You chose option 3" << std::endl;
        int length;
        std::cout << "Enter the length of the password: ";
        std::cin >> length;
        std::cout << "Your password is: " << generatePassword(length) << std::endl;
		break;
    case 4: // Taskkill (PROC NAME)
		std::cout << "You chose option 4" << std::endl;
		std::cout << "Enter the name of the process you want to kill: ";
		std::cin >> procName;
		taskkill_force(procName);
		break;
    case 5: // Taskkill (PID)
        std::cout << "You chose option 5" << std::endl;
		std::cout << "Enter the PID of the process you want to kill: ";
        
		std::cin >> pid;
		taskkill_force(pid);
		break;
    case 6: // Autoclicker
		std::cout << "You chose option 6" << std::endl;
		int cps;
		std::cout << "Enter the CPS (Clicks per second): ";
		std::cin >> cps;
        std::cout << "Autoclicker has been enabled!\nDisable key: 'CTRL + L'" << std::endl;
		autoclicker(true, cps);
		break;
    default:
        std::cout << "You chose an invalid option" << std::endl;
        exit(0);
        break;
    }
    return 0;
}