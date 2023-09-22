#pragma once
#include <iostream>

int taskkill_force(const std::string& processName) {
	std::string command = "taskkill /f /im " + processName;
	return system(command.c_str()); // Execute command and return exit code (0 = success, 1 = error)
}

int restart_task(const std::string& processName) {
	int exitCode = taskkill_force(processName); // Kill process
	if (exitCode == 0) { // If process was killed successfully
			std::cout << "Process " << processName << " was killed successfully!" << std::endl;
			std::cout << "Restarting process... (if you're stuck on here its finished)" << std::endl;
			return system(processName.c_str()); // Restart process
		}
	else {
		std::cout << "Failed to kill process " << processName << "!" << std::endl;
		return exitCode; // Return exit code from taskkill_force
	}
}