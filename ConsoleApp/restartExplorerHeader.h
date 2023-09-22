#pragma once
#include <iostream> // std::cout, std::cerr, std::endl, system()

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