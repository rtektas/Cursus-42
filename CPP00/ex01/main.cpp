#include <iostream>
#include <string>
#include "PhoneBook.hpp"

int main()
{
    PhoneBook pb;
    std::string input;

    std::cout << "Welcome to your PhoneBook !" << std::endl;

    while (true)
    {
        std::cout << "\nEnter command (ADD, SEARCH, EXIT): ";
        if (!std::getline(std::cin, input))
            break;

        if (input == "ADD")
            pb.addContact();
        else if (input == "SEARCH")
            pb.searchContact();
        else if (input == "EXIT")
        {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else if (!input.empty())
            std::cout << "Command invalid. Try: ADD, SEARCH or EXIT." << std::endl;
    }

    return 0;
}