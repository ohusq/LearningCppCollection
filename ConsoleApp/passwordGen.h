#pragma once
#include <string>
#include <iostream>

char alphabet_lower[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char alphabet_upper[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S' ,'T','U','V','W','X','Y','Z' };
char numbers[10] = { '0','1','2','3','4','5','6','7','8','9' };

std::string generatePassword(int length) {
	std::string password = "";
	for (int i = 0; i < length; i++) {
		int random = rand() % 3;
		if (random == 0) {
			password += alphabet_lower[rand() % 26];
		}
		else if (random == 1) {
			password += alphabet_upper[rand() % 26];
		}
		else {
			password += numbers[rand() % 10];
		}
	}
	return password;
}