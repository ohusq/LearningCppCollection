#pragma once

// restarts explorer.exe without using #include <taskkill.h>

#include <iostream>
#include <windows.h>

int restartExplorer() {
    // Kill the explorer.exe thread and respawn one.
    const char* commandToKill{ "taskkill /f /im explorer.exe" };
    const char* commandToRestart{ "start explorer.exe" };

    int killExplorer{ system(commandToKill) };

    if (killExplorer == 0) {
        std::cout << "Succesfully killed explorer.exe\nRestarting..." << std::endl;
    }
    else {
        std::cout << "Failed to kill explorer.exe!\nStopping..." << std::endl;
        exit(0);
    }
    
    int startExplorer{ system(commandToRestart) };

    if (startExplorer == 0) {
        std::cout << "Restarted explorer.exe succesfully" << std::endl;
    }
    else {
        std::cout << "Failed to restart explorer.exe!\nPlease start explorer.exe manually" << std::endl;
        Sleep(sizeof(DWORD));
    }

    return 0;
}
