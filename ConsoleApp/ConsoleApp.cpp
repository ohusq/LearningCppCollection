#include <iostream>

int loadMenu(int totalOptions) {
    std::cout << "Welcome to ohusq's learning project" << std::endl << "Please select an option from below." << std::endl;

    for (int i = 1; i <= totalOptions; ++i) {
        std::cout << i << ". Option " << i << std::endl;
    }

    int x{};
    std::cout << "Enter a number: ";
    std::cin >> x;

    if (x < 1 || x > totalOptions) {
        std::cout << "Please input a valid number!" << std::endl;
    }
    else {
        std::cout << "Valid number! | " << x << std::endl;
    }
    return x;
}

int main()
{
    int totalOptions = 2; // You can change this value to set the total number of options.
    loadMenu(totalOptions);


    return 0;
}
