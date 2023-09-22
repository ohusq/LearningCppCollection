#pragma once
#include <iostream> // std::cout, std::cerr, std::endl, system()
#include "taskkill.h" // taskkill_force, restart_task

bool restartExplorer() {
	int exitCode = restart_task("explorer.exe"); // Restart explorer.exe
	if (exitCode == 0) { // If explorer.exe was restarted successfully
			std::cout << "Explorer.exe was restarted successfully!" << std::endl;
			return true;
		}
	else {
		std::cout << "Failed to restart explorer.exe!" << std::endl;
		return false;
	}
}